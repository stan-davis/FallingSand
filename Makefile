PROJECT_NAME := game
BUILD_DIR := bin

INCLUDE_DIRS = -IC:/Libraries/SDL2/include/SDL2 -Iext/imgui -Iext/cpp-marching-squares -Iext/polypartition -IC:/Libraries/box2d/include/box2d -Iext/fastnoiselite
LIB_DIRS = -LC:/Libraries/SDL2/lib -LC:/Libraries/box2d/lib

LIBS = -lmingw32 -lSDL2main -lSDL2 -lbox2d

SRC = $(wildcard src/*.cpp) $(wildcard src/engine/*.cpp) $(wildcard ext/imgui/*.cpp) $(wildcard ext/polypartition/*.cpp) $(wildcard ext/box2d/src/*/*.cpp)

default:
	g++ $(SRC) -o $(BUILD_DIR)/$(PROJECT_NAME) $(INCLUDE_DIRS) $(LIB_DIRS) $(LIBS)