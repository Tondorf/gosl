
CFLAGS=-g -Wall -Wextra -std=gnu99 
BIN=bin
SRC=src
LIBS=-lncurses
TARGET=$(BIN)/netsl
OBJECTS=$(BIN)/main.o $(BIN)/msg.o $(BIN)/net.o $(BIN)/display.o

all: $(BIN) $(TARGET)

$(BIN):
	mkdir -pv $(BIN)

$(TARGET): $(OBJECTS)
	gcc $^ $(LIBS) -o $(TARGET)

#	mkdir -pv $(BIN)

$(BIN)/%.o: $(SRC)/%.c $(SRC)/%.h
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf $(BIN)

