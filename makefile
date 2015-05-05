all: bin compress decompress

bin:
		@[ ! -d bin ] && mkdir bin

compress:
		@g++ -std=c++1y src/main.cpp src/letter.cpp -o bin/compress

decompress:
		@g++ -std=c++1y src/unmain.cpp src/letter.cpp -o bin/decompress

clean:
		@rm -R bin ; rm *.extension ; rm *.info
