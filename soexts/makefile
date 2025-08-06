# Set compiler and archiver options
CC=gcc
AR = ar
ARFLAGS=rcs

SRC_DIR=src
OBJ_DIR=obj
LIB_DIR=lib
INC_DIR = include


SRCS=$(shell find . -iname "*.c" | tr '\n' ' ')
OBJS = $(pathsubt $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

#H_SRCS=$(shell find . -iname "*.h" | tr '\n' ' ')


LIB_NAME=soexts
STATIC_LIB=bin/lib$(LIB_NAME).a

.PHONY: all clean

all: $(STATIC_LIB)

$(STATIC_LIB): $(OBJS)
	@mkdir -p bin/lib$(LIB_NAME).a
	$(AR) $(ARFLAGS) $@ $(OBJS)
	@echo "Static library created: $@"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) -c $< -o $@ -I$(INC_DIR)

clean:
	-rm -rfv obj bin