#ifndef MPD_H
#define MPD_H

#include "../types.h"

int mpd_status(char *output, unsigned int size, const Arg *arg);
void mpd_status_clean(void *);

#endif // MPD_H
