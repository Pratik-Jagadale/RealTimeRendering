#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

/* Iterface */
bst_t *create_bst(void)
{
    bst_t *p_bst = NULL;
    p_bst = (bst_t *)xmalloc(sizeof(bst_t));
    p_bst->p_root_node = NULL;
    p_bst->nr_elements = 0;
    return (p_bst);
}

status_t insert_bst(bst_t *p_bst, data_t new_data)
{

    bst_node_t *p_new_node = NULL;
    p_new_node = get_bst_node(new_data);

    // if bst empty
    if (p_bst->p_root_node == NULL)
    {
        p_bst->p_root_node = p_new_node;
        return (SUCCESS);
    }

    bst_node_t *p_run = NULL;
    p_run = p_bst->p_root_node;

    // is bst non empty
    while (1)
    {
        if (p_new_node->data <= p_run->data)
        {
            if (p_run->left == NULL)
            {
                p_run->left = p_new_node;
                p_new_node->parent = p_run;
                break;
            }
            else
            {
                p_run = p_run->left;
            }
        }
        else
        {

            if (p_run->right == NULL)
            {
                p_run->right = p_new_node;
                p_new_node->parent = p_run;
                break;
            }
            else
            {
                p_run = p_run->right;
            }
        }
    }
    p_bst->nr_elements += 1;
    return (SUCCESS);
}

bool search_bst(bst_t *p_bst, data_t search_data);

void inorder_r(bst_t *p_bst)
{
    __inorder_r(p_bst->p_root_node);
}

void preorder_r(bst_t *p_bst)
{
    __preorder_r(p_bst->p_root_node);
}

void postorder_r(bst_t *p_bst)
{
    __postorder_r(p_bst->p_root_node);
}

void inorder_nrc(bst_t *p_bst)
{
    __inorder_nrc(p_bst->p_root_node);
}
void preorder_nrc(bst_t *p_bst)
{
    __preorder_nrc(p_bst->p_root_node);
}

void postorder_nrc(bst_t *p_bst)
{
    __postorder_nrc(p_bst->p_root_node);
}

ret_t maximum_bst(bst_t *p_bst, data_t *p_data)
{
}

ret_t minimum_bst(bst_t *p_bst, data_t *p_data)
{
}

ret_t inorder_successor(bst_t *p_bst, data_t e_data, data_t *p_succ_data)
{
}

ret_t inorder_predecessor(bst_t *p_bst, data_t e_data, data_t *p_pred_data)
{
}

ret_t remove_bst(bst_t *p_bst)
{
}

ret_t destroy_bst(bst_t **pp_bst)
{
    bst_t *p_bst = NULL;
    p_bst = *pp_bst;

    __destroy_bst_r(p_bst->p_root_node);

    free(p_bst);
    pp_bst = NULL;
    return (SUCCESS);
}

/* BST Helper Routines */
bst_node_t *search_bst_node(bst_node_t *p_root_node, data_t search_data)
{
    bst_node_t *p_run = NULL;

    p_run = p_root_node;

    while (p_run != NULL)
    {
        if (p_run->data == search_data)
            return (p_run);
        else if (p_run->data >= search_data)
            p_run = p_run->left;
        else
            p_run = p_run->right;
    }
    return (NULL);
}

bst_node_t *get_bst_node(data_t new_data)
{
    bst_node_t *p_node = NULL;
    p_node = (bst_node_t *)xmalloc(sizeof(bst_node_t));
    p_node->data = new_data;

    return (p_node);
}

bst_node_t *inorder_successor_node(bst_node_t *p_bst_node)
{
}

bst_node_t *inorder_predecessor_node(bst_node_t *p_bst_node)
{
}

void __inorder_r(bst_node_t *p_bst_node)
{
    if (p_bst_node == NULL)
        return;

    __inorder_r(p_bst_node->left);
    printf("%d - ", p_bst_node->data);
    __inorder_r(p_bst_node->right);
}

void __preorder_r(bst_node_t *p_bst_node)
{
    if (p_bst_node == NULL)
        return;

    printf("%d - ", p_bst_node->data);
    __inorder_r(p_bst_node->left);
    __inorder_r(p_bst_node->right);
}

void __postorder_r(bst_node_t *p_bst_node)
{
    if (p_bst_node == NULL)
        return;

    __inorder_r(p_bst_node->left);
    __inorder_r(p_bst_node->right);
    printf("%d - ", p_bst_node->data);
}

void __inorder_nrc(bst_node_t *p_bst_node)
{
    bst_node_t *p_run = NULL;
    stack_t *p_stack;

    p_stack = create_bstptr_stack();
    p_run = p_bst_node;

    printf("[START]\n");
    while (TRUE)
    {
        while (p_run != NULL)
        {
            push_bstptr(p_stack, p_run);
            p_run = p_run->left;
        }

        if (pop_bstptr(p_stack, &p_run) == STACK_EMPTY)
            break;

        printf("[%d]<->", p_run->data);
        p_run = p_run->right;
    }

    printf("[END]\n");
    destroy_bstptr_stack(&p_stack);
}

void __preorder_nrc(bst_node_t *p_bst_node)
{
    bst_node_t *p_run = NULL;
    stack_t *p_stack;

    p_stack = create_bstptr_stack();
    p_run = p_bst_node;

    printf("[START]\n");
    while (TRUE)
    {
        while (p_run != NULL)
        {
            printf("[%d]<->", p_run->data);
            push_bstptr(p_stack, p_run);
            p_run = p_run->left;
        }

        if (pop_bstptr(p_stack, &p_run) == STACK_EMPTY)
            break;

        p_run = p_run->right;
    }

    printf("[END]\n");
    destroy_bstptr_stack(&p_stack);
}

void __postorder_nrc(bst_node_t *p_bst_node)
{
    bst_node_t *p_run = NULL;
    stack_t *p_stack;
    color_t color;

    p_stack = create_bstptr_stack();
    p_run = p_bst_node;

    printf("[START]\n");
    while (TRUE)
    {
        while (p_run != NULL)
        {
            push_bstptr(p_stack, p_run);
            set_bstptr_top_color(p_stack, GREY);
            p_run = p_run->left;
        }

        if (get_bstptr_top_color(p_stack, &color) == STACK_EMPTY)
            break;

        if (color == GREY)
        {
            set_bstptr_top_color(p_stack, BLACK);
            top_bstptr(p_stack, &p_run);
            p_run = p_run->right;
            continue;
        }
        else if (color == BLACK)
        {
            pop_bstptr(p_stack, &p_run);
            printf("[%d]<->", p_run->data);
            p_run = NULL;
        }
    }

    printf("[END]\n");
    destroy_bstptr_stack(&p_stack);
}

void __destroy_bst_r(bst_node_t *p_bst_node)
{
    if (p_bst_node != NULL)
    {
        __destroy_bst_r(p_bst_node->left);
        __destroy_bst_r(p_bst_node->right);
        free(p_bst_node);
    }
}

void transplant_bst(bst_t *p_bst, bst_node_t *u, bst_node_t *v)
{
    if (u->parent == NULL)
        p_bst->p_root_node = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;

    if (v)
        v->parent = u->parent;
}

/* Stack Interface Routines */
stack_t *create_bstptr_stack()
{
    return create_bstptr_list();
}
ret_t push_bstptr(stack_t *p_stack, bst_node_t *p_bst_node)
{
    return insert_bstptr_end(p_stack, p_bst_node);
}

ret_t top_bstptr(stack_t *p_stack, bst_node_t **pp_bst_node)
{
    return examine_bstptr_end(p_stack, pp_bst_node);
}

ret_t pop_bstptr(stack_t *p_stack, bst_node_t **pp_bst_node)
{
    return examine_and_delete_bstptr_end(p_stack, pp_bst_node);
}

ret_t is_bstptr_stack_empty(stack_t *p_stack)
{
    return is_bstptr_list_empty(p_stack);
}

ret_t set_bstptr_top_color(stack_t *p_stack, color_t new_color)
{
    if (is_bstptr_stack_empty(p_stack))
        return (STACK_EMPTY);

    p_stack->prev->color = new_color;
    return (SUCCESS);
}

ret_t get_bstptr_top_color(stack_t *p_stack, color_t *p_top_color)
{
    if (is_bstptr_list_empty(p_stack))
        ;
    return (STACK_EMPTY);
    *p_top_color = p_stack->prev->color;
    return (SUCCESS);
}

bool is_bstptr_stack_empty(stack_t *p_stack)
{
    return is_bstptr_list_empty(p_stack);
}

ret_t destroy_bstptr_stack(stack_t **pp_stack)
{
    return destroy_bstptr_list(pp_stack);
}

/* List Interface Routines */
list_t *create_bstptr_list(void)
{
    list_t *p_list = NULL;
    p_list = (list_t *)xmalloc(sizeof(list_t));
    p_list->p_bst_node = NULL;
    p_list->next = p_list;
    p_list->prev = p_list;

    return (p_list);
}

ret_t insert_bstptr_end(list_t *p_list, bst_node_t *p_bst_node)
{
    generic_bstptr_insert(p_list->prev, get_bstptr_node(p_bst_node), p_list);
    return (SUCCESS);
}

ret_t examine_bstptr_end(list_t *p_list, bst_node_t **pp_bst_node)
{
    if (is_bstptr_list_empty(p_list))
        return (LIST_EMTPY);

    *pp_bst_node = p_list->prev->p_bst_node;
    return (SUCCESS);
}

ret_t examine_and_delete_bstptr_end(list_t *p_list, bst_node_t **pp_bst_node)
{
    if (is_bstptr_list_empty(p_list))
        return (LIST_EMTPY);

    *pp_bst_node = p_list->prev->p_bst_node;
    generic_bstptr_delete(p_list->prev);

    return (SUCCESS);
}

bool is_bstptr_list_empty(list_t *p_list)
{
    if (p_list->next != NULL)
        return (FAILED);
    return TRUE;
}

ret_t destroy_bstptr_list(list_t **pp_list)
{
    bstptr_node_t *p_bstptr_run = NULL;
    bstptr_node_t *p_bstptr_run_next = NULL;

    list_t *p_list = NULL;
    p_list = *pp_list;
    p_bstptr_run = p_list->next;
    for (p_bstptr_run; p_bstptr_run != p_list; p_bstptr_run = p_bstptr_run_next)
    {
        p_bstptr_run_next = p_bstptr_run->next;
        free(p_bstptr_run);
    }

    free(p_list);
    *pp_list = NULL;

    return (SUCCESS);
}

/* List Helper Rouitines*/
void generic_bstptr_insert(bstptr_node_t *p_beg, bstptr_node_t *p_mid, bstptr_node_t *p_end)
{
    p_mid->next = p_end;
    p_mid->prev = p_beg;
    p_beg->next = p_mid;
    p_end->prev = p_mid;
}

void generic_bstptr_delete(bstptr_node_t *p_bstptr_node)
{
    p_bstptr_node->next->prev = p_bstptr_node->prev;
    p_bstptr_node->prev->next = p_bstptr_node->next;
    free(p_bstptr_node);
    p_bstptr_node = NULL;
}

bstptr_node_t *search_bstptr_node(list_t *p_list, bst_node_t *p_bst_node)
{
    bstptr_node_t *p_bstptr_run = NULL;
    p_bstptr_run = p_list->next;

    for (p_bstptr_run; p_bstptr_run != p_list; p_bstptr_run = p_bstptr_run->next)
        if (p_bst_node == p_bstptr_run->p_bst_node)
            return (p_bstptr_run);
    return (NULL);
}

bstptr_node_t *get_bstptr_node(bst_node_t *p_bst_node)
{
    bstptr_node_t *p_bstptr_node = NULL;
    p_bstptr_node = (bstptr_node_t *)xmalloc(sizeof(bstptr_node_t));

    p_bstptr_node->p_bst_node = p_bst_node;
    p_bstptr_node->next = NULL;
    p_bstptr_node->prev = NULL;
    p_bstptr_node->color = WHITE;

    return (p_bstptr_node);
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
