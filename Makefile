source = $(wildcard *.cpp)

.PHONY: all clean
all: main 
main: $(source)
	g++ $^ -I . -o $@

clean: 
	rm main