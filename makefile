runshell: simpleshell.o
	g++ -o runshell simpleshell.o

simpleshell.o: simpleshell.c
	g++ -c -Wall simpleshell.c

clean:
	rm *.o runshell