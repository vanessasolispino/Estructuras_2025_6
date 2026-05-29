cflags = -std=c99 -O2 
dflags = -std=c99 -g
includes = -I ../iostring -I ../structures/tablashash/cuadratica

libs = ../../lib -liostring 
hash_obj = ../structures/tablashash/cuadratica/HashQ.o

source = $(wildcard *.c) 
output = ../../bin/practica9

build :
	gcc $(cflags) $(source) $(hash_obj) -o $(output) $(includes) -L $(libs)
	@echo "Compilo Exitosamente!!!"
run :
	$(output).exe