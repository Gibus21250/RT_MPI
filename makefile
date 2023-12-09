CORE_SRC = core/src
CORE_INCLUDE = core/include
CORE_LIB_NAME = libcore.a

BUILD_DIR = build
BIN_DIR = bin

MPIC = mpicc
CC = gcc
CFLAGS = -Wall -Wextra -g -I$(CORE_INCLUDE)

#Prendre tous les fichier sources de core
SOURCES := $(wildcard $(CORE_SRC)/**/*.c $(CORE_SRC)/*.c)

#Convertir en binaire dans bin
OBJECTS := $(patsubst $(CORE_SRC)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))

all: dirs $(BUILD_DIR)/$(CORE_LIB_NAME) start

dirs: $(BUILD_DIR) $(BIN_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

#Regroupe les .o en une lib .a
$(BUILD_DIR)/$(CORE_LIB_NAME): $(OBJECTS)
	ar rcs $@ $(OBJECTS)

$(BUILD_DIR)/%.o: $(CORE_SRC)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

start: main.c
	$(MPIC) $(CFLAGS) main.c -L$(BUILD_DIR) -lcore -lm -o $(BIN_DIR)/start

clean:
	rm -rf $(BUILD_DIR)/*.o $(BUILD_DIR)/$(CORE_LIB_NAME) $(BIN_DIR)/start