

all: bin/net bin/test


bin/net: net.c
	mkdir -pv bin
	gcc -o bin/net net.c

bin/test: test.c
	mkdir -pv bin
	gcc -o bin/test test.c

clean:
	rm -rf bin
