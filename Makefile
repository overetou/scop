#Work title
TARGET=scop

#Directories
OBJ_DIR=obj
SRC_DIR=src
INC_DIRS=includes
LIB_DIRS=/media/jxxj/DATA/Documents/code/unix/config/libft

#Compilation related indications
CC=gcc
CFLAGS=-Wall -Wextra -Werror
LIBS=SDL2 GL GLEW m#m stands for mathlib

#From there we use the info to compile everything.
SRC_FILE_NAMES = main.c process_args.c gl_buffers.c
SOURCES=$(addprefix $(SRC_DIR)/, $(SRC_FILE_NAMES))
OBJS:=$(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
INC_FLAGS=$(addprefix -I,$(INC_DIRS))
LIBIDR_FLAGS=$(addprefix -L,$(LIB_DIRS))
CFLAGS+=$(INC_FLAGS)
LIBS_FLAGS=$(addprefix -l,$(LIBS))
VPATH=$(SRC_DIR)

.phony: $(TARGET) clean re

$(TARGET): $(OBJS) $(INC_DIRS)/scop.h
	@$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LIBIDR_FLAGS) $(LIBS_FLAGS)
	@echo "\033[0;32mCompilation succeeded.\033[0m"

$(OBJS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.c |$(OBJ_DIR)
	@$(COMPILE.c) $(OUTPUT_OPTIONS) -o $@ $<

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

clean:
	rm -rf $(OBJS) $(TARGET)

re: clean $(TARGET)

debian-deps:
	sudo apt install libsdl2-dev libglew-dev