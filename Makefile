CPP := $(wildcard *.cpp)
OUT := $(basename $(CPP))
LDFLAGS:= `/opt/homebrew/bin/pkg-config --libs --cflags sfml-system sfml-window sfml-graphics freetype2 glut`


all: $(OUT)

window.o : window/window.cpp
	clang++ --std=c++17 $(LDFLAGS) -c window/window.cpp -Wno-everything

%: %.cpp window.o
	clang++ --std=c++17 -o $@   $< $(LDFLAGS) -I glad glad/glad.c -Wno-everything -framework OpenGL  -lglut window.o

clean:
	rm $(OUT) window.o


