#ifndef UTIL_H
#define UTIL_H

#define LENGTH(X) (sizeof(X) / sizeof(X[0]))

int xstrncpy(char* dest, char* src, unsigned long size);
void die(const char *fmt, ...);

#endif // UTIL_H
