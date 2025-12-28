CC = gcc
CFLAGS = -Wall -Wextra -pthread
SRC = src/main.c src/logger.c src/config.c
TARGET = server

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)
