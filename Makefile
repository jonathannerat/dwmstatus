SRC=dwmstatus.c util.c
OBJ=$(SRC:.c=.o)

all: options dwmstatus

options:
	@echo dwmstatus build options:
	@echo "CFLAGS  = $(CFLAGS)"
	@echo "LDFLAGS = $(LDFLAGS)"
	@echo "CC      = $(CC)"

.c.o:
	$(CC) $(CFLAGS) -c $<

$(OBJ): config.h types.h arg.h

dwmstatus: $(OBJ)
	$(CC) -o $@ $(OBJ)

debug: CFLAGS += -g -DDEBUG
debug: options dwmstatus

clean:
	-rm dwmstatus *.o

.PHONY = all options debug clean
