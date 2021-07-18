#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "types.h"
char* argv0;
#include "arg.h"
#include "config.h"

#define LENGTH(X) (sizeof(X) / sizeof(X[0]))
#define MAX_BLOCK_LEN 50

void die(const char *errstr, ...);
void printstdout(void);
void usage(void);
void xsetroot(void);
void parseargs(int argc, char* argv[]);
void run(void);
int cacheblock(const Block *b);

static void (*writestatus)(void) = xsetroot;
static int stop = 0;
static int time = -1;
static char cachedblocks[LENGTH(blocks)][MAX_BLOCK_LEN] = {0};

int
main(int argc, char *argv[])
{
	parseargs(argc, argv);

	run();
	return 0;
}

void
die(const char *errstr, ...)
{
va_list ap;

va_start(ap, errstr);
vfprintf(stderr, errstr, ap);
va_end(ap);
exit(1);
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
	return;
}

void
xsetroot(void)
{
	return;
}

void
run(void)
{
	int i = 0;
	const Block *b;

	while (!stop) {
		for (i = 0, b = blocks; i < LENGTH(blocks); i++, b++) {
			if ((b->interval != 0 && time % b->interval == 0) || time == -1) {
				cacheblock(b);
			}
			sleep(1);
		}
	}
}

void
usage(void)
{
	die("usage: %s", argv0);
}
