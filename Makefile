CPP := $(wildcard *.cpp)
OUT := $(basename $(CPP))
EXE := $(OUT:=.exe)
LDFLAGS:= -lsfml-graphics -lsfml-window -lsfml-system -lopengl32 -lGLU32 -lfreetype -lglfw3 -lfreeglut

ifeq ($(OS),Windows_NT)
all: $(EXE)
$(info windows)

window.o : window/window.cpp
	g++ -std=c++14 -c window/window.cpp
%.exe: %.cpp
	g++ -std=c++17 -o $@ $< $(LDFLAGS) -I glad glad/glad.c

clean:
	rm $(EXE)
else
all: $(OUT)
endif


