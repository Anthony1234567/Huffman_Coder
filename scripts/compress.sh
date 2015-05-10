#!/bin/bash

echo Reading file $1...
#sleep 1

echo Compiling...
#sleep 1
make

echo Running...
#sleep 1
./bin/encode $1

mv $1.compressed .
#sleep 1

echo Done!
