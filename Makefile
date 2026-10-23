.PHONY: all clean run run_clean

CC = g++
CFLAGS = -Wall -Wextra -std=c++20

SRC_DIR = ./src
OBJ_DIR = ./obj

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
HEADERS = $(wildcard $(SRC_DIR)/*.hpp)

OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))

OUT = muas

all: clean $(OUT)

run: $(OUT)
	@echo
	@./$(OUT)

run_clean: all run

$(OUT): $(OBJS)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	@mkdir -p $(OBJ_DIR)
	$(CC) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(OUT)