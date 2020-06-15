# huffman-compression
My implementation of Huffman compression algorithm in C.
### Description 
This program uses the Huffman algorithm to compress strings. An overview of the approach I have used is: <br />
1. Count the number of occurences of invidual ASCII characters <br />
2. Create Huffman binary tree according to the the number of occurences <br />
3. Create a codebook of the Huffman code using a 2D array data structure <br />
4. Compress the file using the codebook after adding the appropriate compression header information<br /><br />
Each step corresponds to a unique file which is then compared using `diff` with known outputs in the `./code`, `./count` & `./compressed` directories. This ensures easier troubleshooting and debugging during development.
<br />
This program takes 5 arguments. <br />
arg 1: File to be compressed <br />
arg 2: `.count` file. This file stores number of ASCII occurences <br />
arg 3: `.tree` file. This file stores generated Huffman binary tree in preorder traversal <br />
arg 4: `.code` file. This file stores generated Huffman code for each ASCII character <br />
arg 5: `.hbt` file. Desired name of compressed file
<br />
Example command line instructions. <br />
`make pa2` <br />
`./pa2 original/lorum lorum.count lorum.tree lorum.code lorum.hbt` <br />

### Makefile Instructions
` make pa2 ` - Compiles the program <br />
`make clean` - Removes all temporary files <br />
`make testN` - N corresponds to testcase 1-6 <br />
