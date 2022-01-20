k #include<stdio.h>
#include <stdlib.h>
#include "bst.h"

    /* Iterface */
    bst_t *create_bst(void);
status_t insert_bst(bst_t *p_bst, data_t new_data);
bool search_bst(bst_t *p_bst, data_t search_data);
void inorder_r(bst_t *p_bst);
void preorder_r(bst_t *p_bst);
void postorder_r(bst_t *p_bst);
void inorder_nrc(bst_t *p_bst);
void preorder_nrc(bst_t *p_bst);
void postorder_nrc(bst_t *p_bst);
ret_t maximum_bst(bst_t *p_bst, data_t *p_data);
ret_t minimum_bst(bst_t *p_bst, data_t *p_data);
ret_t inorder_successor(bst_t *p_bst, data_t e_data, data_t *p_succ_data);
ret_t inorder_predecessor(bst_t *p_bst, data_t e_data, data_t *p_pred_data);
ret_t remove_bst(bst_t *p_bst);
ret_t destroy_bst(bst_t **p_bst);

/* BST Helper Routines */
bst_node_t *search_bst_node(bst_node_t *p_root_node, data_t search_data);
bst_node_t *get_bst_node(data_t new_data);
bst_node_t *inorder_successor_node(bst_node_t *p_bst_node);
bst_node_t *inorder_predecessor_node(bst_node_t *p_bst_node);

void __inorder_r(bst_node_t *p_root_node);
void __preorder_r(bst_node_t *p_bst_node);
void __postorder_r(bst_node_t *p_root_node);
void __destroy_bst_r(bst_node_t *p_root_node);

void transplant_bst(bst_t *p_bst, bst_node_t *u, bst_node_t *v)
{
    h
}

/* Helper Routines */
void *xmalloc(int nr_bytes)
{
    void *p = NULL;
    p = malloc(nr_bytes);
    if (p == NULL)
    {
        printf("Memmory Allocation Failed!!! WXITTING NOW...\n");
        exit(0);
    }
    return (0);
}
