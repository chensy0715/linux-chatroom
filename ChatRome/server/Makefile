MYNAME = makefile
CC = gcc

objects = server.o register.o login.o list.o config.o chat.o

server: $(objects)
	cc -g -o server $(objects) -lsqlite3 -lpthread

server.o: server.c config.h
	cc -c server.c

register.o: register.c config.h
	cc -c register.c

login.o: login.c config.h
	cc -c login.c

list.o: list.c config.h
	cc -c list.c

config.o: config.c config.h
	cc -c config.c

chat.o: chat.c config.h
	cc -c chat.c

.PHONY: clean


clean:
	-rm server $(objects)
