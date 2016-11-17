build: matrix quadtree.o src
	gcc -g matrix.o quadtree.o src.o -o quadtree -lm
	rm matrix.o quadtree.o src.o 
matrix:
	gcc -g -c matrix.c -o matrix.o
quadtree.o:
	gcc -g -c quadtree.c -o quadtree.o
src:
	gcc -g -c src.c -o src.o
clean:
	rm matrix.o quadtree.o src.o
