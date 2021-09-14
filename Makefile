./bin/main: ./src/main.cpp
	mkdir -p bin
	g++ -std=c++17 -Wall -Wno-unused-function -o ./bin/main ./src/main.cpp

.PHONY: clean run
clean:
	rm -f bin/*

run: ./bin/main
	./bin/main
