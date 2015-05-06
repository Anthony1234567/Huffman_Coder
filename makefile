all: bin compress decompress

bin:
		@[ ! -d bin ] && mkdir bin

compress:
		@g++ -std=c++1y src/main.cpp src/letter.cpp src/functions.cpp -o bin/compress

decompress:
		@g++ -std=c++1y src/unmain.cpp src/letter.cpp src/functions.cpp -o bin/decompress

clean:
		@rm -R bin ; rm tests/*.extension ; rm tests/*.info
