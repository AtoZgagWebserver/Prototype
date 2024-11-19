all : server.o
	gcc -o server server.o
server.o : server.c
	gcc -g -c server.c

clear:
	rm server.o server
