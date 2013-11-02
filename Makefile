
CFLAGS=-g -Wall -Wextra -std=gnu99 
TARGET=bin/netsl
OBJECTS=main.o msg.o net.o

all: $(TARGET)

$(TARGET): $(OBJECTS)
	mkdir -pv bin
	gcc $^ -o $(TARGET)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf *.o bin


