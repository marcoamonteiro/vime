CC=gcc
CFLAGS=
OBJ=vime_ui.o vime_buffer.o
DEPS=vime_ui.h vime_buffer.h util.h
LIBS=-lncurses

all: vime buffer_test

vime: vime.c $(OBJ)
	$(CC) $^ -o $@ $(CFLAGS) $(LIBS)

buffer_test: buffer_test.c $(OBJ)
	$(CC) $^ -o $@ $(CFLAGS) $(LIBS)

%.o: %.c $(DEPS)
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(OBJ) $(OUT)

