BUILD = build
CC = gcc
CFLAGS = -I. -I../include
SRC = vector.c main.c
OUTPUT = build/main.exe
OBJ = $(SRC:%.c=$(BUILD)/%.o)

TEST_BUILD = test/build
TEST_UNITY = test/unity.c
TEST_SRC := $(filter-out $(TEST_UNITY), $(wildcard test/*.c))
TEST_DEPS = $(patsubst %.c, test/build/%.o, $(filter-out main.c, $(SRC)))
TEST_OUTPUT = $(patsubst test/%.c, test/build/%.exe, $(TEST_SRC))

all: $(BUILD) $(OUTPUT) 

$(BUILD):
	mkdir build -p

$(OUTPUT): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD)/%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf build test/build

test: $(TEST_BUILD) $(TEST_DEPS) $(TEST_OUTPUT)
	@for exe in $(TEST_OUTPUT); do ./$$exe; done

$(TEST_BUILD): 
	mkdir test/build -p

$(TEST_BUILD)/%.exe: test/%.c $(TEST_DEPS) $(TEST_UNITY)
	@$(CC) $(CFLAGS) $< $(TEST_DEPS) $(TEST_UNITY) -o $@

$(TEST_BUILD)/%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@
