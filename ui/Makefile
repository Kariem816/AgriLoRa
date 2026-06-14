all: build/ui.exe

build/ui.exe: build main.cpp utils/anim.h utils/easing.h
	g++ -o build/ui.exe main.cpp -Ivendor/include -Lvendor/lib -lraylib -lm -lwinmm -lgdi32

build:
	mkdir -p build