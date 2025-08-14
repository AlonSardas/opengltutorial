CXX = g++
CXXFLAGS = -Wall -Isrc -Iinclude

LDFLAGS = -L/home/alon/Documents/Dev/OpenGLTutorial/glfw-3.4/build/src \
		  -lGL -lglfw

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

# $(OUT): $(SRC)
# 	$(CXX) $(SRC) -o $(OUT) $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f $(OUT)
