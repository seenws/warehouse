CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Werror -std=c99 -D_POSIX_C_SOURCE=199309L
RAYLIB_CFLAGS = -I/usr/local/include
RAYLIB_LFLAGS = -L/usr/local/lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

SRC = src/main.c src/board.c src/robot.c src/sim.c src/gridview.c
TARGET = warehouse

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(RAYLIB_CFLAGS) -Iheaders -o $@ $(SRC) $(RAYLIB_LFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: clean
