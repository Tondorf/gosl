

all: bin/net bin/test bin/main


bin/net: net.c
	mkdir -pv bin
	gcc -o bin/net net.c

bin/test: test.c
	mkdir -pv bin
	gcc -o bin/test test.c

bin/main: main.c
	mkdir -pv bin
	gcc -o bin/main main.c

clean:
	rm -rf bin
