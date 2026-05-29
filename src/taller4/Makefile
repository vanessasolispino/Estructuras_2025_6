cflags = -Wall -Werror -std=c99 -O2
dflags = -Wall -Werror -std=c99 -g
includes = -I ../iostring -I ../structures/nodos -I ../structures/arbol -I ../structures/pila -I ../structures/heap
libs = ../../lib -liostring -lArbol -lPila -lHeap
source = $(wildcard *.c)
output = ../../bin/taller4

build :
	gcc $(cflags) $(source) -o $(output) $(includes) -L $(libs) -lm
	@echo "Compilo Exitosamente!!!"
rebuild:
	make -C ..\structures\arbol
	make -C ..\structures\heap
	gcc $(cflags) $(source) -o $(output) $(includes) -L $(libs) -lm
	@echo "Compilo Exitosamente!!!"