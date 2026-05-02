CC = gcc
CFLAGS = -Wall -Wextra -Iinc -O2
SRC_DIR = src
BUILD_DIR = build

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))
TARGET = $(BUILD_DIR)/clip

PREFIX = /usr/local
INSTALL_DIR = $(PREFIX)/bin

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

install: $(TARGET)
	@mkdir -p $(INSTALL_DIR)
	@cp $(TARGET) $(INSTALL_DIR)/clip
	@chmod +x $(INSTALL_DIR)/clip
	@echo "clip installed to $(INSTALL_DIR)/clip"

uninstall:
	@rm -f $(INSTALL_DIR)/clip
	@echo "clip removed from $(INSTALL_DIR)/clip"

purge: uninstall
	@rm -rf $$HOME/.clip
	@echo "All clip data removed from $$HOME/.clip"

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean install uninstall purge
