#pragma once

#define SUCCESS 1
#define TRUE 1
#define FALSE 0

#define BST_DATA_NOT_FOUND 2
#define BST_NO_SUCCESSOR 3
#define BST_EMPTY 5

typedef int ret_t;
typedef int bool;

struct bst_node
{
	int data;
	struct bst_node *left;
	struct bst_node *right;
	struct bst_node *parent;
};

struct bst
{
	struct bst_node *p_root_node;
	int nr_elements;
};

struct bst *create_bst(void);
ret_t insert_bst(struct bst *p_bst, int new_data);
bool search_bst(struct bst *p_bst, int search_data);
void inorder_r(struct bst *p_bst);
void preorder_r(struct bst *p_bst);
void postorder_r(struct bst *p_bst);
ret_t maximum_bst(struct bst *p_bst, int *p_max_data);
ret_t minimum_bst(struct bst *p_bst, int *p_min_data);
ret_t inorder_successor(struct bst *p_bst, int e_data, int *p_succ_data);
ret_t remove_bst(struct bst *p_bst, int d_data);
ret_t destroy_bst(struct bst **pp_bst);

struct bst_node *search_bst_node(struct bst_node *p_root_node, int search_data);
struct bst_node *get_bst_node(int new_data);
struct bst_node *inorder_successor_node(struct bst_node *p_bst_node);
struct bst_node *inorder_predecessor_node(struct bst_node *p_bst_node);
void __inorder_r(struct bst_node *p_root_node);
void __preorder_r(struct bst_node *p_root_node);
void __postorder_r(struct bst_node *p_root_node);
void __destroy_bst_r(struct bst_node *p_root_node);
void transplant_bst(struct bst *p_bst, struct bst_node *u, struct bst_node *v);

void *xmalloc(int nr_bytes);
