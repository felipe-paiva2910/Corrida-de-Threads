CC = gcc
CFLAGS = -Wall -Wextra -pthread -Iinclude
TARGET = corrida

SRC = src/main.c src/corrida.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(CFLAGS)

src/%.o: src/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

run:
	./$(TARGET) 1

run_no_barrier:
	./$(TARGET) 0

clean:
	rm -f src/*.o $(TARGET)