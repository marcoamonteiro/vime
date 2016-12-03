CC=gcc
CFLAGS=
OBJ=vime.o vime_ui.o vime_buffer.o
DEPS=vime_ui.h vime_buffer.h util.h
LIBS=-lncurses

vime: $(OBJ)
	$(CC) $^ -o $@ $(CFLAGS) $(LIBS)

buffer_test: $(OBJ) buffer_test.c
	$(CC) $^ -o $@ $(CFLAGS) $(LIBS)

%.o: %.c $(DEPS)
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(OBJ) $(OUT)

