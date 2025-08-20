
# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

# Source files
SRCS = main.c string_split.c

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
TARGET = string_splitter_example

# Default target
all: $(TARGET)

# Link the object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile source files into object files
%.o: %.c string_split.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
