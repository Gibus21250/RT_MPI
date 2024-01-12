CORE_SRC = core/src
CORE_INCLUDE = core/include
CORE_LIB_NAME = libcore.a

MPI_UTILS_INCLUDE = mpi/include
MPI_UTILS_SRC = mpi/src
MPI_UTILS_LIB_NAME = libmpi_utils.a

MPI_INCLUDE = /usr/lib/x86_64-linux-gnu/openmpi/include

CORE_BUILD_DIR = build/core
MPI_UTILS_BUILD_DIR = build/mpi
BIN_DIR = bin

MPIC = mpicc
CC = gcc
#-Wpadded -mavx2
CFLAGS = -Wall -O3 -Wextra -g -I$(CORE_INCLUDE) -I$(MPI_UTILS_INCLUDE) -I$(MPI_INCLUDE)

#Prendre tous les fichier sources de core
CORE_SOURCES := $(wildcard $(CORE_SRC)/**/*.c $(CORE_SRC)/*.c)

# Prendre tous les fichier sources de mpiutils
MPI_UTILS_SOURCES := $(wildcard $(MPI_UTILS_SRC)/**/*.c $(MPI_UTILS_SRC)/*.c)

#Convertir en binaire dans build pour core
CORE_OBJECTS := $(patsubst $(CORE_SRC)/%.c, $(CORE_BUILD_DIR)/%.o, $(CORE_SOURCES))

#Convertir en binaire dans build pour mpiutils
MPI_UTILS_OBJECTS := $(patsubst $(MPI_UTILS_SRC)/%.c, $(MPI_UTILS_BUILD_DIR)/%.o, $(MPI_UTILS_SOURCES))

#build pour les dirs de bases, core et mpiutils puis l'executable
all: dirs $(CORE_BUILD_DIR)/$(CORE_LIB_NAME) $(MPI_UTILS_BUILD_DIR)/$(MPI_UTILS_LIB_NAME) startmpi start

dirs: $(CORE_BUILD_DIR) $(MPI_UTILS_BUILD_DIR) $(BIN_DIR)

$(CORE_BUILD_DIR):
	mkdir -p $(CORE_BUILD_DIR)

$(MPI_UTILS_BUILD_DIR):
	mkdir -p $(MPI_UTILS_BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

#Regroupe les .o en une lib .a pour core
$(CORE_BUILD_DIR)/$(CORE_LIB_NAME): $(CORE_OBJECTS)
	ar rcs $@ $(CORE_OBJECTS)

# Regroupe les .o en une lib .a pour le module mpi_utils
$(MPI_UTILS_BUILD_DIR)/$(MPI_UTILS_LIB_NAME): $(MPI_UTILS_OBJECTS)
	ar rcs $@ $(MPI_UTILS_OBJECTS)

#build les .o en respectant l'arborescence de src de core
$(CORE_BUILD_DIR)/%.o: $(CORE_SRC)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# build les .o en respectant l'arborescence de src pour le module mpi_utils
$(MPI_UTILS_BUILD_DIR)/%.o: $(MPI_UTILS_SRC)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

#Build un start avec MPi
startmpi: mainmpi.c
	$(MPIC) $(CFLAGS) mainmpi.c -L$(CORE_BUILD_DIR) -L$(MPI_UTILS_BUILD_DIR) -lcore -lmpi_utils -lSDL2 -lm -o $(BIN_DIR)/startmpi

#Build un start sans MPI
start: main.c
	$(CC) $(CFLAGS) main.c -L$(CORE_BUILD_DIR) -L$(MPI_UTILS_BUILD_DIR) -lcore -lSDL2 -lm -o $(BIN_DIR)/start

clean:
	rm -rf $(CORE_BUILD_DIR)/**/*.o $(MPI_UTILS_BUILD_DIR)/**/*.o $(CORE_BUILD_DIR)/$(CORE_LIB_NAME) $(MPI_UTILS_BUILD_DIR)/$(MPI_UTILS_LIB_NAME) $(BIN_DIR)/start