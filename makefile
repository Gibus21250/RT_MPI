main: main.c math/vector.o
	mpicc main.c math/vector.o -lm -o start
vector.o: math/vector.c math/vector.h
	gcc -c math/vector.c 