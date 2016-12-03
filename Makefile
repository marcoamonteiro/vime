
OUT=vime

CC=gcc
CFLAGS=
OBJ=vime.o
LIBS=-lncurses

$(OUT): $(OBJ)
	$(CC) -c $^ -o $@ $(CFLAGS) $(LIBS)

%.o: %.c
	$(CC) $< -o $@ $(CFLAGS)

clean:
	rm -f $(OBJ) $(OUT)

