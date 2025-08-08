SRC_DIR := src
OBJ_DIR := out/obj
BIN_DIR := out

EXE := $(BIN_DIR)/matrix_mult
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CPPFLAGS := -Iinclude
# CFLAGS := -Wall -g -pg -std=c11 -ftree-vectorize -fvect-cost-model=very-cheap -msse2 -mfpmath=sse -mtune=generic
# CFLAGS := -Wall -g -pg -std=c11 -O0
CFLAGS := -Wall -g -pg -std=c11 -O2
LDFLAGS := -Llib -pg

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)
