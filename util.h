#ifndef UTIL_H
#define UTIL_H

#define FUNC(func, args) .ct = CtFunction, .c.f = {&(func), args}
#define FUNCC(func, args)                                                      \
	.ct = CtFunction, .c.f = {&(func), args, &(func##_clean)}
#define STR(str) .ct = CtString, .c.s = str
#define CMD(cmd) .ct = CtCommand, .c.s = cmd

#define LENGTH(X) (sizeof(X) / sizeof(X[0]))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define MINUTES *60
#define HOURS *60 MINUTES

int xstrncpy(char *dest, char *src, unsigned long size);
void die(const char *fmt, ...);

#endif // UTIL_H
