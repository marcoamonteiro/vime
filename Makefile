
OUT=vime

CC=gcc
CFLAGS=
OBJ=vime.o vime_ui.o
DEPS=vime_ui.h util.h
LIBS=-lncurses

$(OUT): $(OBJ) $(DEPS)
	$(CC) $(OBJ) -o $@ $(CFLAGS) $(LIBS)

%.o: %.c $(DEPS)
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(OBJ) $(OUT)

