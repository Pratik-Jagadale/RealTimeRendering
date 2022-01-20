#ifdef _BST_H
#define _BST_H

#define SUCCESS 0
#define FAILURE 1
#define DATA_NOT_FOUND 2
#define BST_IS_EMPTY 3

typedef int data_t;
typedef int status_t;
typedef int ret_t;
typedef struct bst bst_t;
typedef struct node node_t;
typedef node_t bst_node_t;

struct bst
{
    bst_node_t *p_root_node;
};

struct node
{
    data_t data;
    bst_node_t *left;
    bst_node_t *right;
    bst_node_t *parent;
};

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
void transplant_bst(bst_t *p_bst, bst_node_t *u, bst_node_t *v);

/* Helper Routines */
void *xmalloc(int nr_bytes);

#endif /* _BST_H */