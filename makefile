all: matrix_mult.exe

matrix_mult.exe: matrix_mult.o
	gcc -o matrix_mult.exe matrix_mult.o

matrix_mult.o: mm.c
	gcc -c mm.c

clean:
	rm matrix_mult.o matrix_mult.exe