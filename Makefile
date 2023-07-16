
CFLAGS=-Wall -g -lm
BIN=towergen

all: $(BIN)


$(BIN): main.c
	gcc $(CFLAGS) -o $@ $^ 

clean:
	rm -rf $(BIN)
