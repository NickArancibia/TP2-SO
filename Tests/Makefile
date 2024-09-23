CC = gcc
CFLAGS = -Wall -g -std=c99
SRC_DIR = src
INCLUDE_DIR = include

# Source files
SOURCES = $(SRC_DIR)/memoryManager.c $(SRC_DIR)/testMM.c $(SRC_DIR)/testUtils.c $(SRC_DIR)/app.c

# Executable
EXECUTABLE = app

# Add include directory to CFLAGS
CFLAGS += -I$(INCLUDE_DIR)

all: $(EXECUTABLE)

# Rule for app
$(EXECUTABLE): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(EXECUTABLE)

.PHONY: all clean