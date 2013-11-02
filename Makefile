
CFLAGS=-g -Wall -Wextra -std=gnu99 
BIN_DIR=bin
SRC_DIR=src
TARGET=$(BIN_DIR)/netsl
OBJECTS=$(BIN_DIR)/main.o $(BIN_DIR)/msg.o $(BIN_DIR)/net.o $(BIN_DIR)/display.o

all: $(BIN_DIR) $(TARGET) 

$(BIN_DIR):
	mkdir -pv $(BIN_DIR)

$(TARGET): $(OBJECTS)
	gcc $^ -o $(TARGET)

#	mkdir -pv $(BIN_DIR)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c $(SRC_DIR)/%.h
	$(CC) $(CFLAGS) -o $@ -c $<


clean:
	rm -rf $(BIN_DIR)


