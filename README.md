# Huffman_Coder
Undergraduate research project implementing the Huffman Algorithm for lossless data compression

### Running 
Run the scrips provided to run the program.
scripts take in one argument each, the relative path to the file user wishes to compress or decompress.
To compress run the compress script on the file
```
./scripts/compress.sh <relative path to file>
```
To decompress run decompress script on a ".compressed" file created from running compression program
```
./scripts/decompress <relative path to .compressed file>
```

### Known Bugs
* "Compressed" file is larger than original file 
* Decompression function is too slow in large files
* Huffman function relies on another function. wokrs for now
* Seg fault in a rare case where single line long and sometimes progress bar reads 105% on small files suggests Decode function is flawed somehow.
