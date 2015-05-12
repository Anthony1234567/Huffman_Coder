#!/bin/bash

echo Initializing decompression on $1...
#sleep 1

echo Compiling...
#sleep 1
make

echo Running...
#sleep 1
./bin/decode $1

echo Done!
