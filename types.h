#ifndef TYPES_H
#define TYPES_H

typedef union {
	int i;
	float f;
	const void *v;
} Arg;

typedef enum { CtString, CtCommand, CtFunction } ContentType;

typedef union {
	char *s;
	char *c;
	struct {
		int (*func)(char *, unsigned int, const Arg *);
		const Arg arg;
		void (*cleanfunc)(void *);
	} f;
} Content;

typedef struct {
	char *p;
	ContentType ct;
	Content c;
	char *s;
	int i;
	int sig;
} Block;

#endif // TYPES_H
