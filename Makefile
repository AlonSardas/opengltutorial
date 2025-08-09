CXX = g++
CXXFLAGS = -Iinclude

LDFLAGS = -L/home/alon/Documents/Dev/OpenGLTutorial/glfw-3.4/build/src \
		  -lGL -lglfw

SRC = $(wildcard src/*.cpp src/*.c)
OUT = main.app

$(OUT): $(SRC)
	$(CXX) $(SRC) -o $(OUT) $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f $(OUT)
