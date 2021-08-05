#include <stdio.h>

#include "animators.h"
#include "mpd.h"
#include "types.h"
#include "util.h"

#define MAX_BLOCK_LEN 50
#define MAX_STATUS_LEN 256

CAROUSEL(disk_usage,
			{
				{CMD("df /     | tail -n+2 | awk '{print $4 \"Ki\"}' | numfmt "
					  "--from=auto --to=iec --format=\"ROOT %.2f\"")},
				{CMD("df /home | tail -n+2 | awk '{print $4 \"Ki\"}' | numfmt "
					  "--from=auto --to=iec --format=\"HOME %.2f\"")},
				{CMD("df /srv/media | tail -n+2 | awk '{print $4 \"Ki\"}' | "
					  "numfmt --from=auto --to=iec --format=\"MEDIA %.2f\"")},
			})

static const Block blocks[] = {
	{FUNCC(mpd_status, {}), .sig = 1},
	{.p = "W: ",
	 CMD("curl -sN https://wttr.in/nueva_pompeya?format=1"),
	 .i = 30 MINUTES},
	{.p = " ", FUNC(carousel, {.v = &disk_usage}), .i = 1 MINUTES},
	{CMD("date \"+  %Y-%m-%d   %H:%M\""), .i = 10},
};

static const char *delim = " │ ";
