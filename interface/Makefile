.PHONY: all build

all: build/ui.exe

build/camera.o: camera.cpp camera.h | build
	g++ -c camera.cpp -o build/camera.o -Ivendor/include

build/easing.o: utils/easing.h utils/easing.cpp | build
	g++ -c utils/easing.cpp -o build/easing.o -Ivendor/include

build/main.o: main.cpp | build
	g++ -c main.cpp -o build/main.o -Ivendor/include

build/ui.exe: build/main.o build/camera.o build/easing.o | build
	g++ -o build/ui.exe build/main.o build/camera.o build/easing.o -Ivendor/include -Lvendor/lib -lraylib -lm -lwinmm -lgdi32

build:
	if not exist build mkdir build
