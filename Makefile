SRC_DIR = ./src/
OBJ_DIR = ./obj/
INC_DIR = ./include/
INCLUDE = -I $(INC_DIR)
LIB = -lpthread
SRCS = $(wildcard $(SRC_DIR)*.cpp)
OBJS = $(patsubst $(SRC_DIR)%.cpp,$(OBJ_DIR)%.o,$(SRCS))

.PHONY: all clean
all: main 
main: $(OBJS)
	g++ $^ $(LIB) -o $@

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	g++ $< $(INCLUDE) -g -c -o  $@

clean: 
	rm main $(OBJ_DIR)*.o