# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Output binary name
TARGET = quiz

# Source files
SOURCES = quiz.c

# Default target
all: $(TARGET)

# Build the quiz program
$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET) $(LDFLAGS)

# Clean up binary
clean:
	rm -f $(TARGET)
