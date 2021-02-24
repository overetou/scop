#Work title
TARGET=scop

#Directories
OBJ_DIR=obj
SRC_DIR=src
INC_DIRS=includes
LIB_DIRS=

#Compilation related indications
CC=g++
CPPFLAGS=
LIBS=SDL2 GL GLEW m#m stands for mathlib

#From there we use the info to compile everything.
SRC_FILE_NAMES = main.cpp process_args.cpp gl_buffers.cpp loader.cpp obj_loader.cpp	parsing.cpp	obj_builer.cpp trio_simplification.cpp vectors.cpp
SOURCES=$(addprefix $(SRC_DIR)/, $(SRC_FILE_NAMES))
OBJS:=$(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
INC_FLAGS=$(addprefix -I,$(INC_DIRS))
LIBIDR_FLAGS=$(addprefix -L,$(LIB_DIRS))
CPPFLAGS+=$(INC_FLAGS)
LIBS_FLAGS=$(addprefix -l,$(LIBS))
VPATH=$(SRC_DIR)

.phony: $(TARGET) clean re

$(TARGET): $(OBJS) $(INC_DIRS)/scop.h
	@$(CC) $(CPPFLAGS) $(OBJS) -o $(TARGET) $(LIBIDR_FLAGS) $(LIBS_FLAGS)
	@echo "\033[0;32mCompilation succeeded.\033[0m"

$(OBJS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp |$(OBJ_DIR)
	@$(COMPILE.cpp) $(OUTPUT_OPTIONS) -o $@ $<

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

clean:
	rm -rf $(OBJS) $(TARGET)

re: clean $(TARGET)

debian-deps:
	sudo apt install libsdl2-dev libglew-dev