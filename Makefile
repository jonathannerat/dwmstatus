SRC=dwmstatus.c
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

dwmstatus: dwmstatus.o
	$(CC) -o $@ $(OBJ)

clean:
	-rm dwmstatus *.o
