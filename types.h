#ifndef TYPES_H
#define TYPES_H

enum {
	CT_STRING,
	CT_COMMAND,
	CT_FUNCTION
};

typedef union {
	char* s;
	char* c;
	char* (*f)(const void*);
} Content;

typedef struct {
	char *prefix;
	char *suffix;
	Content content;
	int content_type;
	int interval;
	int signal;
} Block;

#endif // TYPES_H
