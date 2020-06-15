#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "huffman.h"
// void convertFile ();
// void convertFile ()
// {
//   FILE * fp = fopen("gbin.bin", "wb");
//   char str[] = "go go gophers";
//   fwrite(str, sizeof(str), 1, fp);
//   fclose(fp);

// }

// void printArray()

void print_codebook(int **codebook, int unique_chars, int bit_len)
{
	int temp = 0;
	for (int j = 0; j < unique_chars; j++)
	{
		printf("%c : ", codebook[j][0]);
		for (int k = 1; k < bit_len; k++)
		{
			temp = codebook[j][k];
			printf("%d", temp);
		}
		printf("\n");
	}
}
int main(int argc, char **argv)
{
	//arv[1] = input file to be compressed
	//arv[2] = output file containing the frequencies of occurrences of characters based on the input file
	//arv[3] = output file to store the topology information of Huffman tree using preorder
	//arv[4] = output file to store huffman code of every character
	//arv[5] = output file to store compressed file

	if (argc != 6)
	{
		printf("invalid arguments!\n");
		return EXIT_FAILURE;
	}

	//variable declarations
	unsigned long ascii_char[256] = {0}; //array of ascii frequencies
	bool check;							 // function bool check
	TreeNode *head = NULL;				 // head of linked list of trees
	TreeNode *root = NULL;				 //root of huffman tree

	check = char_occurences(argv[1], argv[2], (unsigned long *)&ascii_char);
	if (check == false) //writes character occurences from argv[1] to argv[2]
	{
		printf("unable to find character occurences!\n");
		return EXIT_FAILURE;
	}

	head = init_linked_list(ascii_char); //returns sorted linked list of tree nodes

	root = create_tree(head); ///returns address of root of huffman binary tree

	check = write_tree(argv[3], root);
	if (check == false)
	{
		printf("error in writing tree in preorder to argv[3]\n");
		return EXIT_FAILURE;
	}
	//+++++++++++++++++ INIT CODEBOOK
	int **codebook;
	int unique_chars = 0;
	int bit_len = maxDepth(root) + 1; //number of unqiue char
	num_leafnodes(root, &unique_chars);
	printf("bit len = %d\nunique chars %d\n", bit_len, unique_chars);
	codebook = (int **)malloc(sizeof(int *) * unique_chars);
	for (int i = 0; i < unique_chars; i++)
	{
		codebook[i] = (int *)malloc(sizeof(int) * bit_len);
	}

	for (int j = 0; j < unique_chars; j++) //set all values to -1
	{
		for (int k = 0; k < bit_len; k++)
		{
			codebook[j][k] = -1;
		}
	}
	//	print_codebook(codebook, unique_chars, bit_len);
	//+++++++++++++++++++++++++++++++++

	check = write_code(argv[4], root, codebook);
	print_codebook(codebook, unique_chars, bit_len);
	if (check == false)
	{
		printf("error in writing code in preorder to argv[4]\n");
		return EXIT_FAILURE;
	}

	check = compress(argv[5], root, ascii_char, codebook, unique_chars, argv[1]); //(uncomressed, tree topology)
	deleteTreeNode(root);
	for (int i = 0; i < unique_chars; i++) //free codebook
	{
		free(codebook[i]);
	}
	free(codebook);

	return EXIT_SUCCESS;
}
