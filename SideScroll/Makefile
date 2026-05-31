# Compiler
CXX = g++

# Use pkg-config for SDL2, SDL2_mixer, and SDL2_ttf
# This handles both CFLAGS (-I...) and LDFLAGS (-l...) correctly
PKG_CONFIG := pkg-config
CFLAGS := $(shell $(PKG_CONFIG) --cflags sdl2 SDL2_mixer SDL2_ttf)
LDFLAGS := $(shell $(PKG_CONFIG) --libs sdl2 SDL2_mixer SDL2_ttf)

# Source file
SRC = src/SideScroller.cpp

# Executable name
TARGET = SideScroller

# Default target
all: $(TARGET)

# Build rule
$(TARGET): $(SRC)
	$(CXX) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

# Clean build files
clean:
	rm -f $(TARGET)

.PHONY: all clean
