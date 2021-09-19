#include "animators.h"

#include <stdio.h>
#include <string.h>

#include "../util.h"

static unsigned int i = 0;

int carousel(char *output, unsigned int size, const Arg *arg) {
	const CarouselArg *carg = arg->v;
	const CarouselItem *item = carg->items + i;
	FILE *cmdout;
	int written = 0;

	i = (i + 1) % carg->size;

	switch (item->type) {
	case CtString:
		if (item->content.s)
			written = xstrncpy(output, item->content.s, size);
		break;
	case CtCommand:
		cmdout = popen(item->content.c, "r");
		if (!cmdout) {
			written = xstrncpy(output, "☠  popen error!", size);
			break;
		}

		fgets(output, size, cmdout);
		written = strlen(output);
		if (output[written - 1] == '\n')
			output[--written] = '\0';
		pclose(cmdout);
		break;
	case CtFunction:
		written = item->content.f.func(output, size, &item->content.f.arg);
		break;
	}

	return written;
}
