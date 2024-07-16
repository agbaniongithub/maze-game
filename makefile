# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -Iheaders -std=c99

# Source and header files
SRC = src/main.c src/raycasting.c src/world_map.c
HEADERS = headers/raycasting.h headers/world_map.h

# Output executable
TARGET = maze_game

# SDL2 flags
SDL2_CFLAGS = $(shell sdl2-config --cflags)
SDL2_LDFLAGS = $(shell sdl2-config --libs)

# Build target
$(TARGET): $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) $(SDL2_CFLAGS) -o $(TARGET) $(SRC) $(SDL2_LDFLAGS)

# Clean target
clean:
	rm -f $(TARGET)

.PHONY: clean
