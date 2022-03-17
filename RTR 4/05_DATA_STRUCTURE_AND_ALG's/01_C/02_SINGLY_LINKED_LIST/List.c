#include <stdio.h>
#include <stdlib.h>
#include "List.h"

typedef int bool;

list_t *create_list(void)
{
    list_t* p_list = NULL;

    p_list = (list_t*)xmalloc(sizeof(list_t));
    p_list->p_head_node = (node_t*)xmalloc(sizeof(node_t));

    p_list->p_head_node->data = -1;
    p_list->p_head_node->next = NULL;

    return(p_list);
}

status_t insert_start(list_t *p_list, data_t new_data)
{
    generic_insert(p_list->p_head_node, get_node(new_data), p_list->p_head_node->next);;
}

status_t insert_end(list_t *p_list, data_t new_data)
{

    node_t* p_last_node = NULL;
    get_last_node(p_list ,&p_last_node);

    generic_insert(p_last_node, get_node(new_data), NULL);
    return (SUCCESS);
}

status_t insert_after(list_t* p_list, data_t e_data, data_t new_data)
{
    node_t* p_e_node = NULL;
    p_e_node = search_node(p_list, e_data);
    if(p_e_node == NULL)
        return (LIST_DATA_NOT_FOUND);

    generic_insert(p_e_node, get_node(new_data), p_e_node->next);

    return (SUCCESS);
}

status_t insert_before(list_t* p_list, data_t e_data, data_t new_data)
{
    node_t* p_e_node = NULL;
    node_t* p_prev_node = NULL;

    get_node_and_prev(p_list, e_data, &p_e_node, &p_prev_node);

    if(p_e_node == NULL)
        return (LIST_DATA_NOT_FOUND);

    generic_insert(p_prev_node, get_node(new_data), p_e_node);

    return (SUCCESS);
}

status_t get_start(list_t* p_list, data_t* p_start_data)
{
    p_start_data = p_list->p_head_node->next->data;
    if(p_start_data != NULL)
        return(SUCCESS);
    return(LIST_EMPTY);
}

status_t get_end(list_t* p_list, data_t* p_end_data)
{
    node_t* p_end_node = NULL;

    get_last_node(p_list, &p_end_node);

    p_end_data = p_end_node->data;

    if(p_end_data != NULL)
        return(SUCCESS);
    return(LIST_EMPTY);
}

status_t pop_start(list_t* p_list, data_t* p_start_data)
{
    p_start_data = p_list->p_head_node->next->data;

    generic_delete(p_list->p_head_node,p_list->p_head_node->next);

    return (SUCCESS);
}

status_t pop_end(list_t* p_list, data_t* p_end_data)
{
    
    node_t* p_remove_node = NULL;

    get_last_node(p_list, &p_remove_node);
    p_end_data = p_remove_node->data; 
    generic_delete(p_remove_node, NULL);

    return (SUCCESS);
}


status_t remove_start(list_t* p_list)
{
    node_t* p_remove_node = NULL;
    p_remove_node = p_list->p_head_node->next;
    generic_delete(p_list->p_head_node,p_remove_node);

    return (SUCCESS);

}

status_t remove_end(list_t* p_list)
{
    node_t* p_remove_node = NULL;

    get_last_node(p_list, &p_remove_node);
    generic_delete(p_remove_node, NULL);

    return (SUCCESS);

}

status_t remove_data(list_t* p_list, data_t r_data)
{
    node_t* p_remove_node = NULL;
    node_t* p_prev_r_node = NULL;
    get_last_node_and_prev(p_list->p_head_node, r_data, &p_remove_node, &p_prev_r_node);   
    if(p_remove_node != NULL)
        return (LIST_DATA_NOT_FOUND);
    generic_delete(p_prev_r_node, p_remove_node);
    return (SUCCESS);
}

len_t get_length(list_t* p_list)
{
    node_t* p_run = NULL;

    p_run = p_list->p_head_node->next;
    len_t len = 0;

    while(p_run != NULL)
    {
        len++;
    }
    return (len);
}

bool is_empty(list_t* p_list)
{
    len_t len = 0;
    len = get_length(p_list);
    if(len == 0)
        return (false);
    return (true);
}

bool is_member(list_t* p_list, data_t s_data)
{
    node_t* p_node = NULL;
    p_node = search_node(p_list, s_data);
    if(p_node != NULL)
        return true;
    return false;   
}

void show(list_t* p_list)
{
    node_t* p_run = NULL;
    p_run = p_list->p_head_node->next;
    
    printf("[START]<->");
    while(p_run != NULL)
    {
        printf("[ %d ]<->",p_run->data);
    }
    printf("[END]<->");
}

list_t* concate(list_t* p_list_1, list_t* p_list_2);
list_t* merge(list_t* p_list_1, list_t* p_list_2);
list_t* get_reversed_list(list_t* p_list);

status_t append(list_t* p_list_1, list_t* p_list_2);
status_t to_list(data_t* p_array, size_t size);

status_t destroy(list_t** pp_list)
{
    node_t* p_run = NULL;
    node_t* p_run_next = NULL;
    p_run = (*pp_list)->p_head_node;
    while(p_run != NULL)
    {
        p_run_next = p_run->next;
        free(p_run);
    }
    free(pp_list);
    pp_list = NULL;
    return (SUCCESS);
}


/* auxillary routines */
static void generic_insert(node_t *p_beg, node_t *p_mid, node_t *p_end)
{    
    p_mid->next = p_end;
    p_beg->next = p_mid;
}

static void generic_delete(node_t *p_prev_node, node_t *p_delete_node)
{
    p_prev_node->next = p_delete_node->next;
    free(p_delete_node);
    p_delete_node = NULL;
} 

static node_t *search_node(list_t *p_list, data_t s_data)
{
    node_t *p_run = NULL;
    p_run = p_list->p_head_node;
    while(p_run->next != NULL)
    {
        if(p_run->data == s_data)
            return(p_run);
    }
    return (NULL);
}

static void get_node_and_prev(list_t *p_list, data_t s_data, node_t **pp_node, node_t **pp_prev_node)
{
    node_t* p_run = NULL;
    node_t* p_prev_run = NULL;

    p_run = p_list->p_head_node->next;

    while(p_run != NULL)
    {
        if(p_run->data == s_data);
        {
            pp_node = p_run;
            pp_prev_node = p_prev_run;
        }
        p_prev_run = p_run;
        p_run = p_run->next;
    }
}

static void get_last_node(list_t *p_list, node_t **pp_last_node)
{
    node_t* p_run = NULL;
    p_run = p_list->p_head_node;

    while(p_run->next =! NULL)
    {
        p_run = p_run->next;
    }
    pp_last_node = p_run;
}

static void get_last_node_and_prev(list_t *p_list, node_t **pp_node, node_t **pp_prev_node)
{
    
    node_t* p_run = NULL;
    node_t* p_prev_run = NULL;

    p_run = p_list->p_head_node;

    while(p_run->next =! NULL)
    {
        p_prev_run = p_run;
        p_run = p_run->next;
    }
    pp_node = p_run;
    pp_prev_node = p_prev_run;
}

static node_t *get_node(data_t new_data)
{
    node_t* p_node = NULL;
    p_node = (node_t*)xmalloc(sizeof(node_t));
    p_node->data = 0;
    p_node->next = NULL;
    return (p_node);
}

static void *xmalloc(size_t nr_bytes)
{
    void* p = NULL;

    p = malloc(nr_bytes);
    if(p == NULL)
    {
        printf("Memmory Allocation failed...");
        return (NULL);
    }
    return (p);
}
