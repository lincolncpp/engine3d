all:
	g++ src/main.cpp src/model.cpp src/utils.cpp -lglfw -lGL -lGLEW -lm && ./a.out