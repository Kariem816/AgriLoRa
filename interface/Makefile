.PHONY: all build

INCLUDE_DIRS := -I. -Ivendor/include

all: build/interface.exe

build/camera.o: camera.cpp camera.h | build
	g++ -c camera.cpp -o build/camera.o $(INCLUDE_DIRS)

build/easing.o: utils/easing.h utils/easing.cpp | build
	g++ -c utils/easing.cpp -o build/easing.o $(INCLUDE_DIRS)

build/main.o: main.cpp | build
	g++ -c main.cpp -o build/main.o $(INCLUDE_DIRS)

UI_HEADERS := $(wildcard ui/*.h)
UI_SOURCES := $(wildcard ui/*.cpp)

UI_OBJS := $(patsubst ui/%.cpp, build/%.o, $(UI_SOURCES))

build/%.o: ui/%.cpp $(UI_HEADERS) | build
	g++ -c $< -o $@ $(INCLUDE_DIRS)

build/interface.exe: build/main.o build/camera.o build/easing.o $(UI_OBJS) | build
	g++ -o build/interface.exe build/main.o build/camera.o build/easing.o $(UI_OBJS) $(INCLUDE_DIRS) -Lvendor/lib -lraylib -lm -lwinmm -lgdi32

build:
	if not exist build mkdir build
