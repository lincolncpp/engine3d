all:
	g++ src/main.cpp src/object.cpp src/utils.cpp -lglfw -lGL -lGLEW -lm && ./a.out