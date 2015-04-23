all: bin compress

bin:
		[ ! -d bin ] && mkdir bin

compress:
		g++ -std=c++11 src/compress.cpp -o bin/compress

clean:
		rm -R bin
