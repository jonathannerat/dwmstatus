#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <X11/Xlib.h>

char* argv0;
#include "arg.h"
#include "types.h"
#include "config.h"
#include "util.h"



int blockintervalgcd();
void cacheblock(const Block *b, char *output);
void parseargs(int argc, char* argv[]);
void printstdout(void);
void run(void);
int statuschanged();
void usage(void);
void setroot(void);
void writeblocks(char *buffer);



static Display* dpy;
static int screen;
static Window root;
static void (*writestatus)(void) = setroot;
static int stop = 0;
static int time = 0;
static char cachedblocks[LENGTH(blocks)][MAX_BLOCK_LEN] = {0};
static char cachedstatuses[2][MAX_STATUS_LEN];



int
main(int argc, char *argv[])
{
	parseargs(argc, argv);

	run();
	return 0;
}



int
blockintervalgcd()
{
	int gcd = 0, a, b, tmp;
	const Block* current;


	for (int i = 0; i < LENGTH(blocks); i++) {
		current = blocks + i;

		if (current->i != 0) {
			if (!gcd) gcd = current->i;
			else {
				a = gcd;
				b = current->i;

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

void
cacheblock(const Block *b, char *output)
{
	int offset = 0, update = 1;
	char *cmd, newoutput[MAX_BLOCK_LEN] = "";
	FILE *cmdout;

	if (b->p)
		offset += xstrncpy(newoutput, b->p, MAX_BLOCK_LEN);

	switch (b->ct) {
		case CtString:
			if (b->c.s)
				offset += xstrncpy(newoutput + offset, b->c.s, MAX_BLOCK_LEN - offset);
			break;
		case CtCommand:
			cmd = b->c.c;
			cmdout = popen(cmd, "r");
			if (!cmdout) sprintf(newoutput, "☠ ");
			
			fgets(newoutput + offset, MAX_BLOCK_LEN - offset, cmdout);
			pclose(cmdout);

			/*
			 * fgets stops reading at a newline and stores it
			 * remove it to display statusline correctly
			 */
			offset = strlen(newoutput);
			if (newoutput[offset-1] == '\n') newoutput[--offset] = '\0';
			break;
		case CtFunction:
			// returns written bytes to output
			update = b->c.f.func(newoutput + offset, MAX_BLOCK_LEN - offset, &b->c.f.arg);
		 	offset += update;
			break;
		default:
			fprintf(stderr, "Invalid c type: %d. Read config.h", b->ct);
			exit(1);
	}

	if (b->s)
		offset += xstrncpy(newoutput + offset, b->s, MAX_BLOCK_LEN - offset);

	if (update) {
		xstrncpy(output, newoutput, MAX_BLOCK_LEN);
	}
}

void
parseargs(int argc, char* argv[])
{
	ARGBEGIN {
		case 'p':
			writestatus = printstdout;
			break;
		default:
			usage();
	} ARGEND;
}

void
printstdout(void)
{
	printf("%s\n", cachedstatuses[0]);
}

void
run(void)
{
	int i;
	const Block *b;
	int sleeptime = blockintervalgcd();

	while (!stop) {
		for (i = 0, b = blocks; i < LENGTH(blocks); i++, b++)
			if ((!b->i && !time) ||
			    (b->i != 0 && time % b->i == 0)) {
				cacheblock(b, cachedblocks[i]);
			}

		if (statuschanged()) writestatus();

		if (sleeptime) {
			sleep(sleeptime);
		}
		else {
			pause();
		}
		time += sleeptime;
	}
}

int
statuschanged()
{
	char *current = cachedstatuses[0], *last = cachedstatuses[1];
	strcpy(last, current);

	current[0] = '\0';

	writeblocks(current);

	return strcmp(current, last);
}

void
usage(void)
{
	fprintf(stderr, "usage: %s", argv0);
}

void
setroot(void)
{
	dpy = XOpenDisplay(NULL);
	if (!dpy) die("cannot open display");

	screen = DefaultScreen(dpy);
	root = RootWindow(dpy, screen);
	XStoreName(dpy, root, cachedstatuses[0]);
	XCloseDisplay(dpy);
}

void
writeblocks(char *buffer)
{
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
