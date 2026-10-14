CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude
LIBS = -lws2_32

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

TARGET = configcore.exe
TEST_TARGET = test_runner.exe

all: $(TARGET) $(TEST_TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

test: tests/test_main.c $(filter-out src/main.c src/server.c, $(OBJ))
	$(CC) $(CFLAGS) $^ -o $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	del /f src\*.o *.exe 2>nul
