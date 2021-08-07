#ifndef UTIL_H
#define UTIL_H

#define FUNC(func, args) .type = CtFunction, .content.f = {&(func), args}
#define FUNCC(func, args)                                                      \
	.type = CtFunction, .content.f = {&(func), args, &(func##_clean)}
#define STR(str) .type = CtString, .content.s = str
#define CMD(cmd) .type = CtCommand, .content.s = cmd

#define LENGTH(X) (sizeof(X) / sizeof(X[0]))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define MINUTES *60
#define HOURS *60 MINUTES

int xstrncpy(char *dest, char *src, unsigned long size);
void die(const char *fmt, ...);

#endif // UTIL_H
