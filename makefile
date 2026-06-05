CXX = g++
TARGET = main.exe
SRC = main.cpp

CXXFLAGS = -Wall -Wextra -std=c++20
RAYLIB_FLAGS = $(shell pkg-config --cflags --libs raylib 2>/dev/null || echo -lraylib)
SYS_LIBS = -lGL -lm -lpthread -ldl -lrt -lX11

.PHONY: all run clear clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(RAYLIB_FLAGS) $(SYS_LIBS)

run: $(TARGET)
	./$(TARGET)

clear:
	rm -f $(TARGET)

clean: clear
