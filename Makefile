BUILD_DIR = bin
SOURCE_DIR = src
WIN_BIN = $(BUILD_DIR)/acid.exe
LINUX_BIN = $(BUILD_DIR)/acid
SOURCES = $(SOURCE_DIR)/main.c $(SOURCE_DIR)/lexer.c $(SOURCE_DIR)/debug.c $(SOURCE_DIR)/ast.c \
	$(SOURCE_DIR)/parser.c $(SOURCE_DIR)/compiler.c $(SOURCE_DIR)/memory.c

CC = gcc

CFLAGS = -c -o -O2 -pipe -Wall -Wextra -Werror

win: $(WIN_BIN)
linux: $(LINUX_BIN)

$(WIN_BIN): $(SOURCES)
	$(CC) $(SOURCES) -o $(WIN_BIN)

$(LINUX_BIN): $(SOURCES)
	$(CC) $(SOURCES) -o $(LINUX_BIN)


%.o: %.c
	$(CC) $(CFLAGS) $*.c $*c

linux_clean:
	rm -f $(BUILD_DIR)/acid $(BUILD_DIR)/main.o

win_clean:
	del $(BUILD_DIR)/main.o, "$(WIN_BIN)"