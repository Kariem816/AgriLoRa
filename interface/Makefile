.PHONY: all build

INCLUDE_DIRS := -I. -Ivendor/include
FLAGS := -std=c++20 -Wall -Wextra -Wpedantic
# FLAGS += -ggdb
# FLAGS += -O3

all: build/interface.exe

build/camera.o: camera.cpp camera.h | build
	g++ $(FLAGS) -c camera.cpp -o build/camera.o $(INCLUDE_DIRS)

build/easing.o: utils/easing.h utils/easing.cpp | build
	g++ $(FLAGS) -c utils/easing.cpp -o build/easing.o $(INCLUDE_DIRS)

UI_HEADERS := $(wildcard ui/*.h)
UI_SOURCES := $(wildcard ui/*.cpp)
UI_OBJECTS := $(patsubst ui/%.cpp, build/ui/%.o, $(UI_SOURCES))

build/ui/%.o: ui/%.cpp | build/ui
	g++ $(FLAGS) -MMD -MP -c $< -o $@ $(INCLUDE_DIRS)

-include $(wildcard build/ui/*.d)

build/libs/libui.a: $(UI_OBJECTS) | build/libs
	ar rcs $@ $^

WIDGET_HEADERS := $(wildcard widgets/*.h)
WIDGET_SOURCES := $(wildcard widgets/*.cpp)
WIDGET_OBJECTS := $(patsubst widgets/%.cpp, build/widgets/%.o, $(WIDGET_SOURCES))

build/widgets/%.o: widgets/%.cpp | build/widgets
	g++ $(FLAGS) -MMD -MP -c $< -o $@ $(INCLUDE_DIRS)

-include $(wildcard build/widgets/*.d)

build/libs/libwidgets.a: $(WIDGET_OBJECTS) | build/libs
	ar rcs $@ $^

build/main.o: main.cpp | build
	g++ $(FLAGS) -c main.cpp -o build/main.o $(INCLUDE_DIRS)

build/interface.exe: build/main.o build/camera.o build/easing.o build/libs/libui.a build/libs/libwidgets.a | build
	g++ $(FLAGS) -o build/interface.exe build/main.o build/camera.o build/easing.o $(INCLUDE_DIRS) -Lvendor/lib -Lbuild/libs -lraylib -lm -lwinmm -lgdi32 -lwidgets -lui

build:
	if not exist build mkdir build

build/widgets: | build
	cd build && if not exist widgets mkdir widgets && cd ../

build/ui: | build
	cd build && if not exist ui mkdir ui && cd ../

build/libs: | build
	cd build && if not exist libs mkdir libs && cd ../