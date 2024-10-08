exec = llsc
sources = $(wildcard src/*.c)
objects = $(sources:.c=.o)
flags = -g -Wall -lm -ldl -fPIC -rdynamic

$(exec): $(objects)
	gcc $(objects) $(flags) -o $(exec)

%.o: %.c include/%.h
	gcc -c $(flags) $< -o $@

clean:
	- rm vgcore.*
	- rm *.out
	- rm *.o
	- rm *.a
	- rm src/*.o
	- rm $(exec)
	- rm a
	- rm *.s
