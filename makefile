# Makefile for pipe test

all: pipes.x

# $@ is make shorthand for the thing on the left side of the colon
#   (pipes.x in this case)
# $^ stands for everything on the right side of the colon (the .o files)
pipes.x: main.o parsetools.o
	gcc -g -o $@ $^

# $< is the first item after the colon (main.c here)
main.o: main.c parsetools.h constants.h
	gcc -g -c -o $@ $<

parsetools.o: parsetools.c constants.h
	gcc -g -c -o $@ $<

clean:
	rm -f *.x *.o *~
