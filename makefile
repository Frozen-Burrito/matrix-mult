all: matrix_mult

matrix_mult: mm.o
	gcc -o matrix_mult mm.o

mm.o: mm.c
	gcc -c mm.c

clean:
	rm mm.o matrix_mult
