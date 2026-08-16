CC = gcc
CFLAGS = -Wall -O2
LDFLAGS = -lncurses
TARGET = snake

all: $(TARGET)

$(TARGET): snake.c
	$(CC) $(CFLAGS) -o $(TARGET) snake.c $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean