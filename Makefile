
CFLAGS=-Wall -g
BIN=towergen

all: $(BIN)


$(BIN): main.c
	gcc $(CFLAGS) -o $@ $^ 

clean:
	rm -rf $(BIN)
