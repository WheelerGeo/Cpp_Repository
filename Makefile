source = $(wildcard ./src/*.cpp)


.PHONY: all clean
all: main 
main: $(source:.cpp=.o)
	g++ $^ -lpthread -o $@

%.o: %.cpp
	g++ $< -I ./include/  -c -o  $@

clean: 
	rm main ./src/*.o