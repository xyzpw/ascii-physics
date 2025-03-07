GXX = g++
TARGET = physics
OPTIONS = -o $(TARGET) -Iinclude -lncurses
FILES = src/*.cpp $(wildcard src/**/*.cpp src/core/**/*.cpp)

$(TARGET): $(FILES)
	$(GXX) $(OPTIONS) $(FILES)

