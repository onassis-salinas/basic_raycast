
CC = gcc
CFLAGS = -Wall -Wextra -std=c17
LDLIBS = -lglfw -lGL -lm

BIN = build/game
SRC = src/main.c

$(BIN): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(BIN) $(LDLIBS)

run: $(BIN)
	./$(BIN)

clean:
	rm -f $(TARGET)

.PHONY: clean
