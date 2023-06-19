all: 
	echo "Command all"

compile: 
	gcc -g -Wall *.c -o sapcewar -lglut -lGL -lGLU -lSOIL -lm
 
execute: compile
	./sapcewar
 
clean:
	rm -rf sapcewar
