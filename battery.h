#ifndef BATTERY_H
#define BATTERY_H

#include "types.h"

int battery_status(char *output, unsigned int size, const Arg *arg);
void battery_status_clean(void *);

#endif // BATTERY_H
