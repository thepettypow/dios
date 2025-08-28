# ESP32 LED Control Simulation Makefile

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -O2
LDFLAGS = 

# Project name
PROJECT = esp32_led_sim

# Source files
SRCS = main.c gpio_mock.c led_control.c button_control.c

# Object files
OBJS = $(SRCS:.c=.o)

# Header files
HEADERS = gpio_mock.h led_control.h button_control.h

# Default target
all: $(PROJECT)

# Build the main executable
$(PROJECT): $(OBJS)
	@echo "Linking $(PROJECT)..."
	$(CC) $(OBJS) -o $(PROJECT) $(LDFLAGS)
	@echo "Build complete: $(PROJECT)"

# Compile source files to object files
%.o: %.c $(HEADERS)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	rm -f $(OBJS) $(PROJECT)
	@echo "Clean complete."

# Run the program
run: $(PROJECT)
	@echo "Running $(PROJECT)..."
	./$(PROJECT)

# Debug build
debug: CFLAGS += -DDEBUG -g3
debug: clean $(PROJECT)

# Release build
release: CFLAGS += -DNDEBUG -O3
release: clean $(PROJECT)

# Install (copy to /usr/local/bin)
install: $(PROJECT)
	@echo "Installing $(PROJECT) to /usr/local/bin..."
	sudo cp $(PROJECT) /usr/local/bin/
	@echo "Installation complete."

# Uninstall
uninstall:
	@echo "Removing $(PROJECT) from /usr/local/bin..."
	sudo rm -f /usr/local/bin/$(PROJECT)
	@echo "Uninstall complete."

# Check for memory leaks (requires valgrind)
valgrind: $(PROJECT)
	@echo "Running valgrind memory check..."
	valgrind --leak-check=full --show-leak-kinds=all ./$(PROJECT)

# Format code (requires clang-format)
format:
	@echo "Formatting source code..."
	clang-format -i $(SRCS) $(HEADERS)
	@echo "Code formatting complete."

# Show help
help:
	@echo "Available targets:"
	@echo "  all      - Build the project (default)"
	@echo "  clean    - Remove build artifacts"
	@echo "  run      - Build and run the program"
	@echo "  debug    - Build with debug symbols"
	@echo "  release  - Build optimized release version"
	@echo "  install  - Install to /usr/local/bin"
	@echo "  uninstall- Remove from /usr/local/bin"
	@echo "  valgrind - Run with memory leak detection"
	@echo "  format   - Format source code with clang-format"
	@echo "  help     - Show this help message"

# Phony targets
.PHONY: all clean run debug release install uninstall valgrind format help

# Dependencies
main.o: main.c gpio_mock.h led_control.h button_control.h
gpio_mock.o: gpio_mock.c gpio_mock.h
led_control.o: led_control.c led_control.h gpio_mock.h
button_control.o: button_control.c button_control.h gpio_mock.h