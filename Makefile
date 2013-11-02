

all:  bin/netsl


#bin/net: net.c
#	mkdir -pv bin
#	gcc -o bin/net net.c

#bin/test: test.c
#	mkdir -pv bin
#	gcc -o bin/test test.c

bin/netsl: misc.h main.c net.c
	mkdir -pv bin
	gcc -o bin/netsl main.c

clean:
	rm -rf bin
