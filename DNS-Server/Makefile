CC=gcc
CFLAGS=-g

dns_server: dns_port.o main.o hashmap.o file_reader.o log.o linked_hashmap.o utils.o
	gcc *.o -o dns_server -luv

clean:
	${RM} *.o dns_server *.log

run: dns_server
	./dns_server

log.o: log.c log.h
	${CC} ${CFLAGS} -c -o log.o log.c -DLOG_USE_COLOR
