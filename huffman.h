#ifndef __SHELL_LIST_H__
#define __SHELL_LIST_H__
#define ASCII_MAX 255

typedef struct tnode
{
  unsigned int character;
  unsigned long frequency;
  struct tnode *next;
  struct tnode *left;
  struct tnode *right;
} TreeNode;

//int a = 0;
//linked list functions
TreeNode *create_new_node(unsigned int ascii, unsigned int freq, TreeNode *, TreeNode *);
TreeNode *init_linked_list(unsigned long *);
void sort_list(TreeNode **, TreeNode *);

//tree fucntions
TreeNode *create_tree(TreeNode *head);
TreeNode *create_tree_helper(TreeNode *head, TreeNode **new_head);
bool write_tree(char *filename, TreeNode *root);
void write_tree_helper(FILE *fp, TreeNode *root, unsigned char *x);
bool write_code(char *filename, TreeNode *root, int **codebook);
void deleteTreeNode(TreeNode *tr);

//utility functions
void printList(TreeNode *head);
bool char_occurences(char *filename, char *output_count, unsigned long *char_occurrences);
int maxDepth(TreeNode *root);
void num_leafnodes(TreeNode *root, int *unique_char);

//compression functions
bool compress(char *filename, TreeNode *root, unsigned long *occurences, int **codebook, int codebook_idx, char *file_read);
void write_compression_from_file(FILE *file_to_write, FILE *file_to_read, int **codebook, int codebook_idx);
#endif
