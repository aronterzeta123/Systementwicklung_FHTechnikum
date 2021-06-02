all: server client
server: server.c 
	gcc -Wall --pedantic -Wextra server.c -o server
client: client.c
	gcc -Wall --pedantic -Wextra client.c -o client
clean:
	rm -rf *~ *.o server client
