#include <string.h>
#include <stdio.h>

#include "carousel.h"
#include "util.h"

static unsigned int i = 0;

int
carousel(char *output, unsigned int size, const Arg *arg)
{
	const CarouselArg *carg = arg->v;
	const CarouselItem *item = carg->items + i;
	FILE *cmdout;
	int written = 0;

	i = (i + 1) % carg->size;

	switch (item->ct) {
		case CtString:
			if (item->c.s) written = xstrncpy(output, item->c.s, size);
			break;
		case CtCommand:
			cmdout = popen(item->c.c, "r");
			if (!cmdout) {
				written = xstrncpy(output, "☠  popen error!", size);
				break;
			}
			
			fgets(output, size, cmdout);
			written = strlen(output);
			if (output[written-1] == '\n') output[--written] = '\0';
			pclose(cmdout);
			break;
		case CtFunction:
			written = item->c.f.func(output, size, &item->c.f.arg);
			break;
	}

	return written;
}
