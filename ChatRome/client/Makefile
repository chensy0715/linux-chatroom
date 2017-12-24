MYNAME = makefile
CC = gcc

objects = client.o config.o register.o login.o interface.o chat.o

server: $(objects)
	cc -g -o client $(objects) -lsqlite3 -lpthread

client.o: client.c config.h
	cc -c client.c 

register.o: register.c config.h
	cc -c register.c

login.o: login.c config.h
	cc -c login.c

interface.o: interface.c config.h
	cc -c interface.c

chat.o: chat.c config.h
	cc -c chat.c

config.o: config.c config.h
	cc -c config.c

.PHONY: clean


clean:
	-rm client $(objects) 

