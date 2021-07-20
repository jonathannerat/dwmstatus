#include "util.h"
/** copies at most size-1 bytes from src to dest, ending with a null byte
 */
int
xstrncpy(char* dest, char* src, unsigned long size)
{
	int written = 0;
	char *pdest = dest, *psrc = src;

	while (written < size - 1 && psrc && *psrc != '\0') {
		*pdest++ = *psrc++;
		++written;
	}

	*pdest = '\0';

	return written;
}
