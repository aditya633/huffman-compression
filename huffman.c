#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "huffman.h"

int a;
/*
Function prints linked list
*/
void printList(TreeNode *head)
{
	TreeNode *p = head;
	long freq = 0;
	printf("printList: \n");
	while (p != NULL)
	{
		freq = freq + p->frequency;
		printf("character = '%d'\tfrequency = %ld\n", p->character, p->frequency);
		p = p->next;
	}
	printf("total freq = %lu", freq);
	printf("\n");
}
/*
Function Description: Create new node as part of linked list of trees. Returns address
of created node
*/
TreeNode *create_new_node(unsigned int ascii, unsigned int freq, TreeNode *l,
						  TreeNode *r)
{
	TreeNode *newNode = malloc(sizeof(TreeNode));
	if (newNode == NULL)
		return NULL;
	newNode->next = NULL;
	newNode->left = l;
	newNode->right = r;
	newNode->character = ascii;
	newNode->frequency = freq;
	return newNode;
}
/*
Creates huffman  binary tree. returns address of root of tree
*/
TreeNode *create_tree_helper(TreeNode *head, TreeNode **new_head)
{
	TreeNode *newNode = NULL;
	TreeNode *temp = head;
	unsigned int value = 256 + a;
	if (head == NULL)
		return head;
	if (head->next == NULL)
	{
		*new_head = head;
		return head;
	}
	unsigned long freq = head->frequency + head->next->frequency;
	newNode = create_new_node(value, freq, head, head->next);

	//printf("left = %c\tright = %c\tcharacter = %d\n",
	//	newNode->left->character, newNode->right->character, newNode->character);
	temp = head->next;
	head->next = NULL;
	head = temp->next;
	temp->next = NULL;
	sort_list(&head, newNode);
	//printList(head);
	a++;
	create_tree_helper(head, new_head);
	//printList(head);
	return head;
}
TreeNode *create_tree(TreeNode *head)
{
	TreeNode *temp = NULL;
	TreeNode *root = NULL;
	temp = head;
	a = 0;
	create_tree_helper(temp, &root);
	//printList(root);

	return root;
}

/*
Function/s writes tree to argv[3] in preorder
*/

void write_tree_helper(FILE *fp, TreeNode *root, unsigned char *x)
{
	unsigned char temp = 0;
	if (root == NULL)
		return;
	//printf("%d ", root->character);
	//printf("\n");
	if (root->character >= 256)
	{
		// fwrite(ascii_char,sizeof(unsigned long),ASCII_MAX+1,fp);
		*x = '0';
		//printf("%c",*x);
		fwrite(x, sizeof(unsigned char), 1, fp);
	}
	else
	{
		temp = (unsigned char)root->character;
		*x = '1';
		//printf("%c",*x);
		//printf("%c",(unsigned char)root->character);
		fwrite(x, sizeof(unsigned char), 1, fp);
		fwrite(&temp, sizeof(unsigned char), 1, fp);
	}

	write_tree_helper(fp, root->left, x);
	write_tree_helper(fp, root->right, x);
}
bool write_tree(char *filename, TreeNode *root)
{
	FILE *fp = NULL;
	unsigned char x = '0';
	fp = fopen(filename, "w");
	if (fp == NULL)
	{
		printf("file open fail in write_tree!\n");
		return false;
	}

	fseek(fp, 0, SEEK_SET);
	write_tree_helper(fp, root, &x);
	printf("\n");
	fclose(fp);
	return true;
}

/*
DELETED TREE wand frees memory
*/

void deleteTreeNode(TreeNode *tr)
{
	if (tr == NULL)
	{
		return;
	}
	deleteTreeNode(tr->left);
	deleteTreeNode(tr->right);
	free(tr);
}

/*
writes huffman code to argv[4]
*/
//void write_code_helper (FILE *fp, TreeNode * root, unsigned char * x);

void write_code_helper(FILE *fp, TreeNode *root, int *arr, int x, int height, int **codebook, int *codebook_idx);
int maxDepth(TreeNode *root)
{
	if (root == NULL)
		return 0;
	else
	{
		/* compute the depth of each subtree */
		int lDepth = maxDepth(root->left);
		int rDepth = maxDepth(root->right);

		/* use the larger one */
		if (lDepth > rDepth)
			return (lDepth + 1);
		else
			return (rDepth + 1);
	}
}
void write_code_helper(FILE *fp, TreeNode *root, int *arr, int x, int height, int **codebook, int *codebook_idx)
{
	if (root == NULL)
		return;
	if (root->left != NULL)
	{
		arr[x] = '0';
		write_code_helper(fp, root->left, arr, x + 1, height, codebook, codebook_idx);
	}
	if (root->right != NULL)
	{
		arr[x] = '1';
		write_code_helper(fp, root->right, arr, x + 1, height, codebook, codebook_idx);
	}
	if ((root->right == NULL) && (root->left == NULL))
	{
		unsigned char temp = 0;
		//printf("%c:", root->character);
		temp = root->character;
		fwrite(&temp, sizeof(unsigned char), 1, fp);
		codebook[(*codebook_idx)][0] = temp;
		codebook[(*codebook_idx)][1] = x;
		temp = ':';
		fwrite(&temp, sizeof(unsigned char), 1, fp);

		for (int i = 0; i < x; i++)
		{
			temp = arr[i];
			//int code =

			codebook[(*codebook_idx)][i + 2] = temp - '0';
			//printf("%c",temp);
			fwrite(&temp, sizeof(unsigned char), 1, fp);
		}
		//printf("\n");
		temp = '\n';
		fwrite(&temp, sizeof(unsigned char), 1, fp);
		(*codebook_idx) = (*codebook_idx) + 1;
		// fwrite(&arr,sizeof(int)*x,1,fp);
	}
	return;
}
//void write_code_helper ()

bool write_code(char *filename, TreeNode *root, int **codebook)
{

	FILE *fp = NULL;
	int height = maxDepth(root); //height of bhinary tree
	int codebook_idx = 0;		 //index to move through codebook
	//printf("height insise writecode = %d\n", height);
	int x = 0; //array index counter
	// printf("height of binary tree = %d\n",height);
	height = height - 1;
	int *arr = malloc(sizeof(int) * height);
	fp = fopen(filename, "w");
	if (fp == NULL)
	{
		printf("file open fail in write_tree!\n");
		return false;
	}
	fseek(fp, 0, SEEK_SET);

	write_code_helper(fp, root, arr, x, height, codebook, &codebook_idx);
	// fseek(fp, 0, SEEK_SET);
	// write_tree_helper(fp, root, &x);
	// printf("\n");
	free(arr);
	fclose(fp);

	return true;
}
/*
Function Sorts Linked List
*/
void sort_list(TreeNode **head, TreeNode *newNode)
{
	TreeNode *current = NULL;
	if (*head == NULL || (*head)->frequency > newNode->frequency)
	{
		newNode->next = *head;
		*head = newNode;
	}
	else
	{
		current = *head;
		while (current->next != NULL && current->next->frequency <= newNode->frequency)
		{
			current = current->next;
		}

		newNode->next = current->next;
		current->next = newNode;
	}
}
/*
Function Description: Reads array and creates a sorted linked list of trees.
Returns head of linked list  
*/

TreeNode *init_linked_list(unsigned long *ascii_char)
{
	TreeNode *head = NULL;
	TreeNode *sorted = NULL;
	TreeNode *temp = NULL;
	TreeNode *next = NULL;
	for (int i = 0; i < 256; i++) //creates unorganized linked list
	{
		if (ascii_char[i] != 0)
		{
			if (head == NULL)
			{
				head = create_new_node((unsigned char)i, (unsigned long)ascii_char[i], NULL, NULL);
				temp = head;
			}
			else
			{
				temp->next = create_new_node((unsigned char)i, (unsigned long)ascii_char[i], NULL, NULL);
				temp = temp->next;
			}
		}
	}
	//printList(head);
	temp = head;
	while (temp != NULL)
	{
		next = temp->next;
		sort_list(&sorted, temp);
		temp = next;
	}

	head = sorted;
	//printList(head);
	return head;
}

/* 
Function Description: Reads argv[1], calculates number of occurences and writes it 
to argv[2]
*/

bool char_occurences(char *filename, char *output_count, unsigned long *ascii_char)
{
	FILE *fp = NULL;
	//unsigned char * temp = NULL;  //temporory chacter buffer
	fp = fopen(filename, "rb");
	if (fp == NULL)
	{
		printf("file open fail in ASCII_char!\n");
		return false;
	}

	fseek(fp, 0, SEEK_END);
	int size = ftell(fp); //size of file
	fseek(fp, 0, SEEK_SET);
	//printf("\nsize of input file= %d \n\n", size);

	unsigned char *temp = malloc(sizeof(unsigned char) * size);
	if (temp == NULL)
	{
		return false;
	}

	fread(temp, size, 1, fp);

	for (int i = 0; i < size; i++)
	{
		ascii_char[(int)temp[i]]++;
	}

	free(temp);
	fclose(fp);

	fp = NULL;
	fp = fopen(output_count, "wb");
	if (fp == NULL)
	{
		return false;
	}

	fwrite(ascii_char, sizeof(unsigned long), ASCII_MAX + 1, fp);

	fclose(fp);

	return true;
}

//void create_codebook(int ** codebook, unsigned long occurunces)
//code book fucntions
//initis a code book nxm array with -1 at all indexes

void num_leafnodes(TreeNode *root, int *unique_char)
{
	if (root == NULL)
	{
		return;
	}

	if ((root->left == NULL) && (root->right == NULL))
	{
		(*unique_char) = (*unique_char) + 1;
		//printf("unique chars in help = %d\n", (*unique_char));
	}

	//printf("char = %c\nfreq = %lu\n", root->character, root->frequency);
	num_leafnodes(root->left, unique_char);
	num_leafnodes(root->right, unique_char);
}

/*
 Creates huffman compression
 */
void char_in_topology(TreeNode *root, long *number);
void char_in_topology(TreeNode *root, long *number)
{
	if (root == NULL)
	{
		return;
	}

	if ((root->left == NULL) && (root->right == NULL))
	{
		(*number) = (*number) + 1 + 8;
	}
	else
	{
		(*number) = (*number) + 1;
	}

	//printf("char = %c\nfreq = %lu\n", root->character, root->frequency);
	char_in_topology(root->left, number);
	char_in_topology(root->right, number);
	return;
}

void char_in_compressed(long *number_compressed, unsigned long *occurences, int **codebook, int codebook_idx);
void char_in_compressed(long *number_compressed, unsigned long *occurences, int **codebook, int codebook_idx)
{
	unsigned char temp_char;
	int bit_counter = 0;
	int size = 0;
	for (int i = 0; i < 255; i++)
	{
		//printf("%d: %ld\n", i, occurences[i]);
		if (occurences[i] != 0)
		{
			temp_char = i; //index of occerences is ascii value
			for (int j = 0; j < codebook_idx; j++)
			{
				if (codebook[j][0] == temp_char) //if codebook value matches unique char
				{
					size = codebook[j][1];				   //size of bits
					bit_counter += (size * occurences[i]); //total bits represented by char in file
					break;
				}
			}

			//printf("%d\n", temp_char);
		}
	}
	(*number_compressed) = bit_counter;
}

unsigned char revers_bits(unsigned char number);
unsigned char revers_bits(unsigned char number)
{
	unsigned char number_of_bits = 8;
	unsigned char reverse_num = 0;
	unsigned char i;
	unsigned char temp;

	for (i = 0; i < number_of_bits; i++)
	{
		temp = (number & (1 << i));
		if (temp)
		{
			reverse_num |= (1 << ((number_of_bits - 1) - i));
		}
	}

	return reverse_num;
}
int index_counter = 0;
//unsigned char byte_buffer = 0x0;
void find_topology(unsigned char *array, TreeNode *root);
void find_topology(unsigned char *array, TreeNode *root)
{
	unsigned char char_buffer;
	if (root == NULL)
	{
		return;
	}

	if ((root->left == NULL) && (root->right == NULL))
	{
		char_buffer = '0' + 1;
		array[index_counter] = char_buffer;

		char_buffer = root->character;
		array[index_counter + 1] = char_buffer;
		index_counter = index_counter + 2;
	}
	else
	{
		char_buffer = '0';
		array[index_counter] = char_buffer;
		index_counter++;
	}

	find_topology(array, root->left);
	find_topology(array, root->right);
}
void write_topology(FILE *filename, TreeNode *root, int num_nodes);
void write_topology(FILE *filename, TreeNode *root, int num_nodes)
{
	unsigned char *preorder_array = malloc(sizeof(char) * num_nodes);
	unsigned char byte_to_write = 0x00;
	int index = 0;
	int padding_check = 0;
	//unsigned char reverse_of_char = 0x00;
	find_topology(preorder_array, root);
	for (int j = 0; j < num_nodes; j++)
	{
		printf("%c", preorder_array[j]);
	}
	printf("\n");
	for (int i = 0; i < num_nodes; i++)
	{
		if (index > 7)
		{
			fwrite(&byte_to_write, sizeof(unsigned char), 1, filename);
			padding_check++;
			byte_to_write = 0x00;
			index = 0;
		}
		if (preorder_array[i] == '0')
		{
			byte_to_write &= ~(1 << (index));
			index++;
		}
		else if (preorder_array[i] == '1')
		{
			byte_to_write |= 1 << (index);

			index++;
			//reverse_of_char = revers_bits(preorder_array[i + 1]);
			unsigned char char_buffer = preorder_array[i + 1];
			i++;
			if (index > 7)
			{
				fwrite(&byte_to_write, sizeof(unsigned char), 1, filename);
				padding_check++;
				byte_to_write = 0x00;
				byte_to_write |= char_buffer;
				fwrite(&byte_to_write, sizeof(unsigned char), 1, filename);
				padding_check++;
				byte_to_write = 0x00;
				index = 0;
			}
			else
			{
				int remaining_size = 8 - index;
				unsigned char append_mask = 0xFF >> index;
				unsigned char appended_byte = append_mask & char_buffer;
				byte_to_write |= appended_byte << index;
				fwrite(&byte_to_write, sizeof(unsigned char), 1, filename);
				padding_check++;
				byte_to_write = 0x00;
				//unsigned char nxt_append_mask = 0xFF >> remaining_size;
				unsigned char nxt_appended_byte = char_buffer >> remaining_size;
				byte_to_write |= nxt_appended_byte; // <<\ remaining_size;
				index = 8 - remaining_size;
			}
		}
		//printf("%c", preorder_array[i]);
	}
	// if ((padding_check % 2) != 0) //not an even number of bytes written
	// {
	// 	byte_to_write = 0x00;
	// 	fwrite(&byte_to_write, sizeof(unsigned char), 1, filename);
	// }

	free(preorder_array);
}
void number_of_nodes(TreeNode *root, long *nodes);
void number_of_nodes(TreeNode *root, long *nodes)
{
	if (root == NULL)
	{
		return;
	}
	(*nodes) = (*nodes) + 1;
	number_of_nodes(root->left, nodes);
	number_of_nodes(root->right, nodes);
}

void write_compression_from_file(FILE *file_to_write, FILE *file_to_read, int **codebook, int codebook_idx)
{
	fseek(file_to_read, 0, SEEK_END);
	int size = ftell(file_to_read); //size of file
	fseek(file_to_read, 0, SEEK_SET);
	//printf("\nsize of input file= %d \n\n", size);

	unsigned char *char_buffer = malloc(sizeof(unsigned char) * size);

	fread(char_buffer, size, 1, file_to_read);

	int padding_check = 0;
	unsigned char byte_to_write = 0x00;
	int index = 0;
	int size_of_code;

	for (int i = 0; i < size; i++)
	{

		if (index > 7)
		{
			fwrite(&byte_to_write, sizeof(unsigned char), 1, file_to_write);
			padding_check++;
			byte_to_write = 0x00;
			index = 0;
		}
		int j;
		for (j = 0; j < codebook_idx; j++) // finds char
		{
			if (codebook[j][0] == char_buffer[i]) //finds char in codebook
			{
				break;
			}
		}
		size_of_code = codebook[j][1];
		if (size_of_code + index > 7) //need to append bits
		{
			int remaining_size = 8 - index;
			for (int p = 0; p < remaining_size; p++)
			{
				byte_to_write |= (codebook[j][p + 2]) << index;
				index++;
			}
			fwrite(&byte_to_write, sizeof(unsigned char), 1, file_to_write);
			byte_to_write = 0x00;
			int remaining_bits = size_of_code - remaining_size;
			index = 0;
			for (int k = 0; k < remaining_bits; k++)
			{
				byte_to_write |= (codebook[j][k + 2]) << index;
				index++;
			}
		}
		else
		{

			for (int k = 0; k < size_of_code; k++)
			{
				byte_to_write |= (codebook[j][k + 2]) << index;
				index++;
			}
		}

		//printf("%c", char_buffer[i]);
	}
	free(char_buffer);

	// unsigned char char_buffer;
	// while (1)
	// {
	// 	if (feof(file_to_read))
	// 	{
	// 		break;
	// 	}
	// 	char_buffer = fgetc(file_to_read);
	// 	printf("%c", char_buffer);
	// }
}
bool compress(char *filename, TreeNode *root, unsigned long *occurences, int **codebook, int codebook_idx, char *file_read)
{

	//header long integers required
	long number_huffman_char = 0; //number of char in tree topology
	long number_uncompressed = 0; //number of charcters in uncompressed
	long number_compressed = 0;	  //charcters in compressed

	for (int i = 0; i < 256; i++)
	{
		if (occurences[i] != 0)
		{
			number_uncompressed += occurences[i]; //
		}
	}

	char_in_topology(root, &number_huffman_char); //calculates number of char in topology
	if ((number_huffman_char % 8) == 0)
	{
		number_huffman_char = number_huffman_char / 8;
	}
	else
	{
		number_huffman_char = number_huffman_char / 8;
		number_huffman_char++;
	}
	char_in_compressed(&number_compressed, occurences, codebook, codebook_idx);
	if ((number_compressed % 8) == 0)
	{
		number_compressed = number_compressed / 8;
	}
	else
	{
		number_compressed = number_compressed / 8;
		number_compressed++;
	}

	number_compressed += number_huffman_char + (3 * sizeof(long));
	//writing to file
	FILE *fp = fopen(filename, "wb");
	if (fp == NULL)
	{
		return false;
	}
	//writing file headers
	fwrite(&number_compressed, sizeof(unsigned long), 1, fp);
	fwrite(&number_huffman_char, sizeof(unsigned long), 1, fp);
	fwrite(&number_uncompressed, sizeof(unsigned long), 1, fp);
	//printf("chaarcters in compressed = %ld\nchars in huffman topology = %ld\nchars in orginal uncompressed = %ld\n",
	//	   number_compressed, number_huffman_char, number_uncompressed);

	//writing huffman tree topology
	long nodes_in_tree = 0;
	number_of_nodes(root, &nodes_in_tree);
	nodes_in_tree = nodes_in_tree + codebook_idx;
	write_topology(fp, root, nodes_in_tree); //writes topography of tree
	//write_file_compression
	FILE *file_to_read = fopen(file_read, "rb");
	write_compression_from_file(fp, file_to_read, codebook, codebook_idx);
	fclose(file_to_read);
	// unsigned char lol = 'g';
	// unsigned char rev = revers_bits(lol);
	//printf("normal %d\nREVERSE = %d\n", lol, rev);
	//unsigned char xx = 0b00111100;

	fclose(fp);
	return true;
}