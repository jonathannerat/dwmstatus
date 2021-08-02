#include <stdio.h>

#include "types.h"
#include "util.h"
#include "battery.h"
#include "carousel.h"

#define MAX_BLOCK_LEN 50
#define MAX_STATUS_LEN 256

CAROUSEL(disk_usage, {
	{ CMD("df /     | tail -n+2 | awk '{print $4 \"Ki\"}' | numfmt --from=auto --to=iec --format=\"/ %.2f\"") },
	{ CMD("df /home | tail -n+2 | awk '{print $4 \"Ki\"}' | numfmt --from=auto --to=iec --format=\"/home %.2f\"") },
})

static const Block blocks[] = {
	{ CMD("mpc current -f \"[[%artist% - ]%title%]|[%file]\""), .i = 10 },
	{ .p = " ", FUNC(carousel, {.v = &disk_usage}), .i = 1 MINUTES },
	{ FUNCC(battery_status, {}), .sig = 2, .i = 1 MINUTES },
	{ CMD("date \"+  %Y-%m-%d   %H:%M\""), .i = 10 },
};

static const char *delim = " │ ";
