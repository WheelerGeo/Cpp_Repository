source = $(wildcard ./src/*.cpp)

.PHONY: all clean
all: main 
main: $(source)
	g++ $^ -I ./include/ -o $@

clean: 
	rm main