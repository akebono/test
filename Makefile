all : directories stuff

directories:
	mkdir -p bin
stuff:
	gcc -Wall -lGLU GUI.c -o bin/gui -lGL -lm -lX11 -no-pie -lpng
