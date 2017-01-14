all: server client

server: server.o networking.o
	gcc -o server server.o networking.o

client: client.o networking.o
	gcc -lncurses -o client client.o networking.o

server.o: server.c networking.h
	gcc -c server.c

client.o: client.c networking.h
	gcc -c client.c

networking.o: networking.c networking.h
	gcc -c networking.c

clean:
	rm *.o
	rm *~s
