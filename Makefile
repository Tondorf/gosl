
CFLAGS=-g -Wall -Wextra -std=gnu99 


all: bin/netsl

#bin/net: net.c
#	mkdir -pv bin
#	gcc -o bin/net net.c

#bin/test: test.c
#	mkdir -pv bin
#	gcc -o bin/test test.c

bin/netsl: misc.h main.c net.c msg.c msg.h
	mkdir -pv bin
	gcc $(CFLAGS) -o bin/netsl main.c

clean:
	rm -rf bin

