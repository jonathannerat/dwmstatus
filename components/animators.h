#ifndef ANIMATORS_H
#define ANIMATORS_H

#include "../types.h"

typedef struct {
	ContentType type;
	Content content;
} CarouselItem;

typedef struct {
	CarouselItem *items;
	unsigned int size;
} CarouselArg;

#define CAROUSEL(name, ...)                                                    \
	CarouselItem name##_items__[] = __VA_ARGS__;                                \
	CarouselArg name = {name##_items__, LENGTH(name##_items__)};

int carousel(char *output, unsigned int size, const Arg *arg);

#endif // ANIMATORS_H
