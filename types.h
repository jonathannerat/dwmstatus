#ifndef TYPES_H
#define TYPES_H

typedef union {
	int i;
	float f;
	const void *v;
} Arg;

typedef enum {
	CtString,
	CtCommand,
	CtFunction
} ContentType;

typedef union {
	char* s;
	char* c;
	struct {
		int (*func)(const Arg*);
		const Arg arg;
	} f;
} Content;

typedef struct {
	char *p;
	ContentType ct;
	Content c;
	char *s;
	int i;
	int S;
} Block;

#endif // TYPES_H
