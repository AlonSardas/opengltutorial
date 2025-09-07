CXX = g++
CXXFLAGS = -Wall -Isrc -Iinclude

DEBUGFLAGS = -g -O0 -fsanitize=address,undefined
RELEASEFLAGS = -O2

PROJECT_DIR := $(HOME)/Documents/Dev/OpenGLTutorial

LDFLAGS = -L/$(PROJECT_DIR)/glfw-3.4/build/src \
		  -L/$(PROJECT_DIR)/assimp/bin \
		  -lGL -lglfw -lassimp

DEBUG_LDFLAGS = -fsanitize=address,undefined
RELEASE_LDFLAGS =

SRC = $(wildcard src/*.cpp src/*.c src/**/*.cpp src/**/*.c)
# OBJ = $(SRC:.cpp=.o)
# OBJ := $(OBJ:.c=.o)
OBJ := $(patsubst %.cpp,build/%.o,$(SRC))
OBJ := $(patsubst %.c,build/%.o,$(OBJ))

OUT = main.app

$(OUT): $(OBJ)
	$(CXX) $(OBJ) -o $(OUT) $(LDFLAGS)

build/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

debug: CXXFLAGS += $(DEBUGFLAGS)
debug: LDFLAGS += $(DEBUG_LDFLAGS)
debug: $(OUT)

release: CXXFLAGS += $(RELEASEFLAGS)
release: LDFLAGS += $(RELEASE_LDFLAGS)
release: $(OUT)

clean:
	rm -f $(OUT)
	rm -rf build/
