#include <stdio.h>

#include "types.h"
#include "util.h"
#include "battery.h"

#define MAX_BLOCK_LEN 50
#define MAX_STATUS_LEN 256

#define FUNC(func, args) .ct=CtFunction, .c.f={&(func), args}
#define FUNCC(func, args) .ct=CtFunction, .c.f={&(func), args, &(func##_clean)}
#define STR(str) .ct=CtString, .c.s=str
#define CMD(cmd) .ct=CtCommand, .c.s=cmd

static const Block blocks[] = {
	{ FUNCC(battery_status, {}), .sig = 2 },
	{ .p = " ", CMD("date +%T"), .i = 10 },
	{ .p = " ", STR("Jonathan") },
};

static const char *delim = " | ";
