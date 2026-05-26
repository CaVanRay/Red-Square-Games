# Compiler
CXX = g++

# Use sdl2-config to get the correct compiler and linker flags
CFLAGS = $(shell sdl2-config --cflags)
LDFLAGS = $(shell sdl2-config --libs)

# Source file
SRC = src/Pong.cpp

# Executable name
TARGET = Pong

# Default target
all:	$(TARGET)

# Build rule
$(TARGET): $(SRC)
	$(CXX) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

# Clean build files
clean:
	rm -f $(TARGET)

.PHONY: all clean
