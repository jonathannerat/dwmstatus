#include <stdio.h>

#include "types.h"
#include "util.h"

#define MAX_BLOCK_LEN 50
#define MAX_STATUS_LEN 256

#define FUNC(func, args) .ct=CtFunction, .c.f={&(func), args}
#define STR(str) .ct=CtString, .c.s=str
#define CMD(cmd) .ct=CtCommand, .c.s=cmd

/* write this from your custom functions */
static char funcbuf[MAX_BLOCK_LEN];

int numberofblocks(const Arg* arg);

static const Block blocks[] = {
	{ .p = "# ", FUNC(numberofblocks, { .v = "%d" }), .s = " blocks" },
	{ .p = " ", CMD("date +%T"), .i = 10 },
	{ .p = " ", STR("Jonathan") },
};

static const char delim[] = " | ";

int
numberofblocks(const Arg* arg)
{
	int nblocks = LENGTH(blocks);
	snprintf(funcbuf, MAX_BLOCK_LEN, (char*) arg->v, nblocks);

	return 1;
}
