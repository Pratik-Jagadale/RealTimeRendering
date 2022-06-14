#pragma once

#define SUCCESS 1
#define TRUE 1
#define FALSE 0

#define BST_DATA_NOT_FOUND 2
#define BST_NO_SUCCESSOR 3
#define BST_EMPTY 5

struct bst_node;
struct bst;

typedef struct bst_node bst_node_t;
typedef struct bst bst_t;

typedef int data_t;
typedef int ret_t;
typedef int bool;

struct bst_node
{
	data_t data;
	bst_node_t *left;
	bst_node_t *right;
	bst_node_t *parent;
};

struct bst
{
	bst_node_t *p_root_node;
	int nr_elements;
};

/* Interface */

bst_t *create_bst(void);
ret_t insert_bst(bst_t *p_bst, data_t new_data);
bool search_bst(bst_t *p_bst, data_t search_data);
void inorder_r(bst_t *p_bst);
void preorder_r(bst_t *p_bst);
void postorder_r(bst_t *p_bst);
ret_t maximum_bst(bst_t *p_bst, data_t *p_max_data);
ret_t minimum_bst(bst_t *p_bst, data_t *p_min_data);
ret_t inorder_successor(bst_t *p_bst, data_t e_data, data_t *p_succ_data);
ret_t remove_bst(bst_t *p_bst, data_t d_data);
ret_t destroy_bst(bst_t **pp_bst);

/* BST Auxillary */
bst_node_t *search_bst_node(bst_node_t *p_root_node, data_t search_data);
bst_node_t *get_bst_node(data_t new_data);
bst_node_t *inorder_successor_node(bst_node_t *p_bst_node);
bst_node_t *inorder_predecessor_node(bst_node_t *p_bst_node);
void __inorder_r(bst_node_t *p_root_node);
void __preorder_r(bst_node_t *p_root_node);
void __postorder_r(bst_node_t *p_root_node);
void __destroy_bst_r(bst_node_t *p_root_node);
void transplant_bst(bst_t *p_bst, bst_node_t *u, bst_node_t *v);

/* General auxillary routines */
void *xcalloc(size_t n, size_t s);