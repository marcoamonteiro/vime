
OUT=vime

CC=gcc
CFLAGS=
OBJ=vime.o vime_ui.o
LIBS=-lncurses

$(OUT): $(OBJ)
	$(CC) $^ -o $@ $(CFLAGS) $(LIBS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(OBJ) $(OUT)

