SRC  = dwmstatus.c util.c battery.c carousel.c
OBJ  = $(SRC:.c=.o)
LIBS = -lX11 `pkg-config --libs dbus-glib-1 gio-2.0` 
CFLAGS = `pkg-config --cflags dbus-glib-1 gio-2.0` 
LDFLAGS = $(LIBS)

all: options dwmstatus

options:
	@echo dwmstatus build options:
	@echo "CFLAGS  = $(CFLAGS)"
	@echo "LDFLAGS = $(LDFLAGS)"
	@echo "CC      = $(CC)"

dwmstatus: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ)

$(OBJ): config.h types.h arg.h

.c.o:
	$(CC) $(CFLAGS) -c $<

debug: CFLAGS += -g -DDEBUG
debug: options dwmstatus

clean:
	-rm dwmstatus *.o

.PHONY = all options debug clean
