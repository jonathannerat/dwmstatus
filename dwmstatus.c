#include <X11/Xlib.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *argv0;
#include "arg.h"
#include "config.h"
#include "types.h"
#include "util.h"

int blockintervalgcd();
int statuschanged();
void cacheblock(const Block *b, char *output);
void parseargs(int argc, char *argv[]);
void printstdout(void);
void run(void);
void setroot(void);
void setupsignals();
void sighandler(int signum);
void termsighandler(int signum);
void usage(void);
void writeblocks(char *buffer);

static Display *dpy;
static int screen;
static Window root;
static void (*writestatus)(void) = setroot;
static int stop = 0;
static int time = 0;
static int sleeptime = 0;
static char cachedblocks[LENGTH(blocks)][MAX_BLOCK_LEN] = {0};
static char cachedstatuses[2][MAX_STATUS_LEN];

int main(int argc, char *argv[]) {
	parseargs(argc, argv);

	signal(SIGTERM, termsighandler);
	signal(SIGINT, termsighandler);
	setupsignals();

	run();
	return 0;
}

int blockintervalgcd() {
	int gcd = 0, a, b, tmp;
	const Block *current;

	for (int i = 0; i < LENGTH(blocks); i++) {
		current = blocks + i;

		if (current->interval != 0) {
			if (!gcd)
				gcd = current->interval;
			else {
				a = gcd;
				b = current->interval;

				while (a != 0) {
					tmp = a;
					a = b % a;
					b = tmp;
				}

				gcd = b;
			}
		}
	}

	return gcd;
}

void cacheblock(const Block *b, char *output) {
	int offset = 0, update = 1;
	char *cmd, newoutput[MAX_BLOCK_LEN] = "";
	FILE *cmdout;

	if (b->wait && !time) {
		xstrncpy(output, b->meanwhile ? b->meanwhile : BLOCK_NA, MAX_BLOCK_LEN);
		return;
	}

	if (b->prefix)
		offset += xstrncpy(newoutput, b->prefix, MAX_BLOCK_LEN);

	switch (b->type) {
	case CtString:
		if (b->content.s)
			offset +=
				xstrncpy(newoutput + offset, b->content.s, MAX_BLOCK_LEN - offset);
		break;
	case CtCommand:
		cmd = b->content.c;
		cmdout = popen(cmd, "r");
		if (!cmdout) {
			offset +=
				xstrncpy(newoutput, "☠  popen error", MAX_BLOCK_LEN - offset);
			break;
		}

		fgets(newoutput + offset, MAX_BLOCK_LEN - offset, cmdout);
		pclose(cmdout);

		/*
		 * fgets stops reading at a newline and stores it
		 * remove it to display statusline correctly
		 */
		offset = strlen(newoutput);
		if (newoutput[offset - 1] == '\n')
			newoutput[--offset] = '\0';
		break;
	case CtFunction:
		// returns written bytes to output
		update = b->content.f.func(newoutput + offset, MAX_BLOCK_LEN - offset,
		                           &b->content.f.arg);
		offset += update;
		break;
	default:
		fprintf(stderr, "Invalid c type: %d. Read config.h", b->type);
		exit(1);
	}

	if (b->suffix)
		offset += xstrncpy(newoutput + offset, b->suffix, MAX_BLOCK_LEN - offset);

	if (update) {
		xstrncpy(output, newoutput, MAX_BLOCK_LEN);
	}
}

void parseargs(int argc, char *argv[]) {
	ARGBEGIN {
	case 'p':
		writestatus = printstdout;
		break;
	default:
		usage();
	}
	ARGEND;
}

void printstdout(void) { printf("%s\n", cachedstatuses[0]); }

void run(void) {
	int i;
	unsigned int less;
	const Block *b;

	sleeptime = blockintervalgcd();

	while (!stop) {
		for (i = 0, b = blocks; i < LENGTH(blocks); i++, b++)
			if (!time ||
			    ((!b->wait || b->wait < time) && b->interval && time % b->interval == 0) ||
			    (b->wait <= time && time < b->wait + sleeptime)) {
				cacheblock(b, cachedblocks[i]);
			}

		if (statuschanged())
			writestatus();

		if (sleeptime) {
			less = sleeptime;
			while (less) less = sleep(less);
			time += sleeptime;
		} else {
			pause();
		}
	}
}

void setupsignals() {
	struct sigaction sa;
	sa.sa_flags = 0;
	const Block *current;

	// block signals
	for (int i = 0; i < LENGTH(blocks); i++) {
		current = blocks + i;
		if (current->signal > 0) {
			sa.sa_handler = sighandler;
			sigaction(SIGRTMIN + current->signal, &sa, NULL);
		}
	}

	struct sigaction sa_child = {
		.sa_handler = SIG_DFL,
		.sa_flags = SA_NOCLDWAIT
	};

	sigaction(SIGCHLD, &sa_child, NULL);
}

void sighandler(int signum) {
	const Block *b;

	for (int i = 0; i < LENGTH(blocks); i++) {
		b = blocks + i;

		if (b->signal == signum - SIGRTMIN)
			cacheblock(b, cachedblocks[i]);
	}

	if (statuschanged())
		writestatus();
}

int statuschanged() {
	char *current = cachedstatuses[0], *last = cachedstatuses[1];
	strcpy(last, current);

	current[0] = '\0';

	writeblocks(current);

	return strcmp(current, last);
}

void termsighandler(int signum) { stop = 1; }

void usage(void) { fprintf(stderr, "usage: %s", argv0); }

void setroot(void) {
	dpy = XOpenDisplay(NULL);
	if (!dpy)
		die("cannot open display");

	screen = DefaultScreen(dpy);
	root = RootWindow(dpy, screen);
	XStoreName(dpy, root, cachedstatuses[0]);
	XCloseDisplay(dpy);
}

void writeblocks(char *buffer) {
	char *src;
	const char *pdelim;
	int written = 0;

	for (int i = 0; i < LENGTH(blocks); i++) {
		pdelim = delim;
		src = cachedblocks[i];

		while (src && *src != '\0' && written < MAX_STATUS_LEN - 1) {
			*buffer++ = *src++;
			written++;
		}

		if (i != LENGTH(blocks) - 1) {
			while (pdelim && *pdelim != '\0' && written < MAX_STATUS_LEN - 1) {
				*buffer++ = *pdelim++;
				written++;
			}
		}
	}

	*buffer = '\0';
}
