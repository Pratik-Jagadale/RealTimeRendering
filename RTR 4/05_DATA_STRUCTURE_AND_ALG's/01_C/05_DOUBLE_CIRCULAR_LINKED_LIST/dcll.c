#include <stdio.h>
#include <stdlib.h>
#include "dcll.h"

typedef int bool;

/* list - Interface Routines definitions*/
list_t *create_list(void)
{
    list_t *p_list = NULL;
    p_list = xmalloc(sizeof(list_t));
    p_list->data = 0;
    p_list->next = p_list;
    p_list->prev = p_list;

    return (p_list);
}

status_t insert_beg(list_t *p_list, data_t new_data)
{
    generic_insert(p_list, get_node(new_data), p_list->next);
    return (SUCCESS);
}

status_t insert_end(list_t *p_list, data_t new_data)
{
    generic_insert(p_list->prev, get_node(new_data), p_list);
    return (SUCCESS);
}

status_t insert_before(list_t *p_list, data_t e_data, data_t new_data)
{
    node_t *p_e_data = NULL;
    p_e_data = locate_node(p_list, new_data);
    if (p_e_data = NULL)
        return (LIST_DATA_NOT_FOUND);

    generic_insert(p_e_data, get_node(new_data), p_e_data->next);
    return (SUCCESS);
}

status_t insert_after(list_t *p_list, data_t e_data, data_t new_data)
{
    node_t *p_e_data = NULL;
    p_e_data = locate_node(p_list, new_data);
    if (p_e_data = NULL)
        return (LIST_DATA_NOT_FOUND);

    generic_insert(p_e_data->prev, get_node(new_data), p_e_data);
    return (SUCCESS);
}

status_t get_beg(list_t *p_list, data_t *p_data)
{
    if (is_list_empty(p_list) == TRUE)
        return (LIST_EMPTY);
    *p_data = (p_list->next->data);
    return (SUCCESS);
}

status_t get_end(list_t *p_list, data_t *p_data)
{
    if (is_list_empty(p_list) == TRUE)
        return (LIST_EMPTY);
    *p_data = p_list->prev->data;
    return (SUCCESS);
}

status_t pop_beg(list_t *p_list, data_t *p_data)
{
    if (is_list_empty(p_list) == TRUE)
        return (LIST_EMPTY);
    *p_data = p_list->next->data;
    generic_delete(p_list->next);
    return (SUCCESS);
}
status_t pop_end(list_t *p_list, data_t *p_data)
{
    if (is_list_empty(p_list) == TRUE)
        return (LIST_EMPTY);
    *p_data = p_list->prev->data;
    generic_delete(p_list->prev);
    return (SUCCESS);
}

status_t remove_beg(list_t *p_list)
{
    if (is_list_empty(p_list) == TRUE)
        return (LIST_EMPTY);
    generic_delete(p_list->next);
    return (SUCCESS);
}

status_t remove_end(list_t *p_list)
{
    if (is_list_empty(p_list) == TRUE)
        return (LIST_EMPTY);
    generic_delete(p_list->prev);
    return (SUCCESS);
}

status_t remove_data(list_t *p_list, data_t data)
{
    node_t *p_delete_node = NULL;
    p_delete_node = locate_node(p_list, data);
    printf("erjhwhrf\n");
    if (p_delete_node == NULL)
    {
        printf("wfefjwnf\n");
        return (LIST_DATA_NOT_FOUND);
    }
    printf("%d\n", p_delete_node->data);
    generic_delete(p_delete_node);
    return (SUCCESS);
}

len_t get_length(list_t *p_list)
{
    node_t *p_run = NULL;
    p_run = p_list->next;
    len_t len = 0;
    for (p_run; p_run != p_list; p_run = p_run->next)
        len++;
    return (len);
}

bool is_list_empty(list_t *p_list)
{
    /*
    node_t *p_run = NULL;
    p_run = p_list->next;
    len_t len = 0;
    for (p_run; p_run != p_list; p_run = p_run->next)
        len++;
    if (len == 0)
        return (FALSE);
    return (TRUE);
    */
    return (p_list->next == p_list && p_list->prev == p_list);
}

bool search_data(list_t *p_list, data_t search_data)
{
    return (locate_node(p_list, search_data) != NULL);
}

int get_repeate_count(list_t *p_list, data_t search_data)
{
    int count = 0;
    list_t *p_run = NULL;
    for (p_run = p_list->next; p_run != p_list; p_run = p_run->next)
    {
        if (p_run->data == search_data)
        {
            count++;
        }
    }
    return (count);
}

void show_list(list_t *p_list, const char *msg)
{
    if (msg)
    {
        puts(msg);
    }
    node_t *p_run;
    printf("[BEG]<->");

    for (p_run = p_list->next; p_run != p_list; p_run = p_run->next)
    {
        printf("[%d]<->", p_run->data);
    }

    printf("[END]\n\n");
}

status_t destroy_list(list_t **pp_list)
{
    list_t *p_list = *pp_list;
    node_t *p_run, *p_run_next;
    for (p_run = p_list->next; p_run != p_list; p_run = p_run_next)
    {
        p_run_next = p_run->next;
        free(p_run);
    }
    free(p_list);
    *pp_list = NULL;
    printf("List Destroyed Successfully!!!\n\n");
    return (SUCCESS);
}

/* list - auxillary Routines */
void generic_insert(node_t *p_beg, node_t *p_mid, node_t *p_end)
{
    p_mid->next = p_end;
    p_mid->prev = p_beg;
    p_beg->next = p_mid;
    p_end->prev = p_mid;
}

void generic_delete(node_t *p_delete_node)
{
    p_delete_node->prev->next = p_delete_node->next;
    p_delete_node->next->prev = p_delete_node->prev;
    free(p_delete_node);
}

node_t *locate_node(list_t *p_list, data_t search_data)
{
    node_t *p_run = NULL;
    for (p_run = p_list->next; p_run != p_list; p_run = p_run->next)
        if (p_run->data == search_data)
            return (p_run);
    return (NULL);
}

node_t *get_node(data_t new_data)
{
    node_t *p = NULL;
    p = xmalloc(sizeof(node_t));
    p->next = NULL;
    p->prev = NULL;
    p->data = new_data;
    return (p);
}

/* Generic Helper Function */
void *xmalloc(int nr_bytes)
{
    void *p = NULL;
    p = malloc(nr_bytes);
    if (p == NULL)
    {
        printf("Memmory Allocation Failed ...!!!\n");
        return NULL;
    }
    memset(p, 0, nr_bytes);
    return (p);
}
