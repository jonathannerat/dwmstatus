#include "util.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/** copies at most size-1 bytes from src to dest, ending with a null byte
 */
int xstrncpy(char *dest, char *src, unsigned long size) {
	int written = 0;
	char *pdest = dest, *psrc = src;

	while (written < size - 1 && psrc && *psrc != '\0') {
		*pdest++ = *psrc++;
		++written;
	}

	*pdest = '\0';

	return written;
}

void die(const char *fmt, ...) {
	va_list ap;

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);

	if (fmt[0] && fmt[strlen(fmt) - 1] == ':') {
		fputc(' ', stderr);
		perror(NULL);
	} else {
		fputc('\n', stderr);
	}

	exit(1);
}
