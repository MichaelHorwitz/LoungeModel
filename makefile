main: main.cpp glad.c
	g++ -g shader.cpp sceneObject.cpp main.cpp glad.c -lglfw -pthread -lGLEW -ldl -lGL -o main

clean:
	rm -f *.o *.txt main

run:
	./main

all:
	make clean
	make
	make run
all2: main.cpp glad.c
	make clean
	g++ -g shader.cpp sceneObject.cpp main2.cpp glad.c -lglfw -pthread -lGLEW -ldl -lGL -o main
	make run