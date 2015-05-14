all: bin compress decompress

bin:
		@[ ! -d bin ] && mkdir bin

compress:
		@g++ -std=c++1y src/letter.cpp src/functions.cpp src/encode.cpp -o bin/encode

decompress:
		@g++ -std=c++1y src/letter.cpp src/functions.cpp src/decode.cpp -o bin/decode

clean:
		@rm -R bin 2>/dev/null || true; rm *.compressed 2>/dev/null || true
