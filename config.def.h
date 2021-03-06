#include <stdio.h>

#include "components/animators.h"
#include "components/battery.h"
#include "components/mpd.h"
#include "types.h"
#include "util.h"

#define MAX_BLOCK_LEN 50
#define MAX_STATUS_LEN 256

/* Declare a list of "blocks" (only content type and content, see types.h) to
 * be iterated over in the carousel. This declares a disk_usage CarouselArg
 * type, which should be passed as an argument to the carousel FUNC definition
 * below */
CAROUSEL(disk_usage,
         {
				{CMD("df /     | tail -n+2 | awk '{print $4 \"Ki\"}' | numfmt "
                 "--from=auto --to=iec --format=\"ROOT %.1f\"")},
				{CMD("df /home | tail -n+2 | awk '{print $4 \"Ki\"}' | numfmt "
                 "--from=auto --to=iec --format=\"HOME %.1f\"")},
			})

// clang-format off
/* Declare your blocks here. Use the following macros:
 * - CMD(cmd) to declare a command block, which executes the command with
 *   /bin/sh, and uses the first line as the block text
 * - FUNC(func, args) to declare a function block, which takes a function func
 *   with a specific signature and an Arg structure (see types.h)
 * - STR(text) to declare a static text block */
static const Block blocks[] = {
	/* Command block for oneliner weather status */
	{ CMD("curl -sN https://wttr.in/?format=1"),
		.prefix = "Wth: ",
		.interval = 30 MINUTES
	},
	/* Function block, that takes other "blocks" as arguments */
	{ FUNC(carousel, {.v = &disk_usage}),
		.prefix = " ",
		.interval = 1 MINUTES
	},
	/* Uncomment the next block for battery status. Receives an RTMIN+2 signal,
    * to update status on AC plug/unplug events, which can be triggered with
    * udev rules */
	/* { FUNCC(battery_status, {}),
		.signal = 2,
		.interval = 1 MINUTES
	}, */
	/* Another command block */
	{ CMD("date \"+ %Y-%m-%d  %H:%M\""),
		.interval = 10
	},
};
// clang-format on

static const char *delim = " │ ";
