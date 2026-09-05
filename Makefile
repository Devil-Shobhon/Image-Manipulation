CC = gcc

CFLAGS = -Wall -Wextra -I./include -I./iup/include -I./im/include
LDFLAGS = -L./iup -L./im
LIBS = -liup -liupim -lim -lm

TARGET = app

SRC = $(wildcard src/*.c)

all:
	$(CC) $(SRC) $(CFLAGS) $(LDFLAGS) $(LIBS) -o $(TARGET)

run: all
	LD_LIBRARY_PATH=./iup:./im ./$(TARGET)

clean:
	rm -f $(TARGET)
