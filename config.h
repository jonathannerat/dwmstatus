#include <stdio.h>

#include "types.h"
#include "util.h"

#define MAX_BLOCK_LEN 50
#define MAX_STATUS_LEN 256

#define FUNC(func, args) .ct=CtFunction, .c.f={&(func), args}
#define STR(str) .ct=CtString, .c.s=str
#define CMD(cmd) .ct=CtCommand, .c.s=cmd

int numberofblocks(char* dest, unsigned int size, const Arg* arg);

static const Block blocks[] = {
	{ .p = "# ", FUNC(numberofblocks, {}), .s = " blocks" },
	{ .p = " ", CMD("date +%T"), .i = 10 },
	{ .p = " ", STR("Jonathan") },
};

static const char delim[] = " | ";

int
numberofblocks(char* output, unsigned int size, const Arg* arg)
{
	char b[2];
	int nblocks = LENGTH(blocks);
	snprintf(b, size, "%d", nblocks);

	return xstrncpy(output, b, size);
}
