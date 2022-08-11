#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "list.h"

list_t *create_list(void)
{
    node_t *p_new_node = NULL;
    p_new_node = get_list_node(0, 0, 0);
    p_new_node->next = p_new_node;
    p_new_node->prev = p_new_node;
    return (p_new_node);
}

status_t insert_beg(list_t *p_list, float x, float y, float z)
{
    generic_insert(p_list, get_list_node(x, y, z), p_list->next);
    return (SUCCESS);
}

status_t insert_end(list_t *p_list, float x, float y, float z)
{
    generic_insert(p_list->prev, get_list_node(x, y, z), p_list);
    return (SUCCESS);
}

status_t get_beg(list_t *p_list, float *x, float *y, float *z)
{
    if (is_list_empty(p_list) == TRUE)
    {
        return (LIST_EMPTY);
    }
    *x = p_list->next->x;
    *y = p_list->next->y;
    *z = p_list->next->z;
    return (SUCCESS);
}

status_t get_end(list_t *p_list, float *x, float *y, float *z)
{
    if (is_list_empty(p_list) == TRUE)
    {
        return (LIST_EMPTY);
    }
    *x = p_list->prev->x;
    *y = p_list->prev->y;
    *z = p_list->prev->z;
    return (SUCCESS);
}

status_t remove_beg(list_t *p_list)
{
    if (is_list_empty(p_list))
    {
        return (LIST_EMPTY);
    }
    generic_delete(p_list->next);
    return (SUCCESS);
}

status_t remove_end(list_t *p_list)
{
    if (is_list_empty(p_list))
    {
        return (LIST_EMPTY);
    }
    generic_delete(p_list->prev);
    return (SUCCESS);
}

len_t get_length(list_t *p_list)
{
    node_t *p_run;
    len_t len;
    for (p_run = p_list->next, len = 0; p_run != p_list; p_run = p_run->next, ++len)
    {
    }
    return (len);
}

bool is_list_empty(list_t *p_list)
{
    return (p_list->next == p_list && p_list->prev == p_list);
}

void show_list(list_t *p_list, const char *msg)
{
    node_t *p_run;
    if (msg)
    {
        puts(msg);
    }
    printf("[BEG]<->");
    for (p_run = p_list->next; p_run != p_list; p_run = p_run->next)
    {
        printf("[%f]", p_run->x);
        printf("[%f]", p_run->x);
        printf("[%f]<->", p_run->x);
    }
    puts("[END]");
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
    return (SUCCESS);
}

static void generic_insert(node_t *p_beg, node_t *p_mid, node_t *p_end)
{
    p_mid->next = p_end;
    p_mid->prev = p_beg;
    p_beg->next = p_mid;
    p_end->prev = p_mid;
}

static void generic_delete(node_t *p_delete_node)
{
    p_delete_node->next->prev = p_delete_node->prev;
    p_delete_node->prev->next = p_delete_node->next;
    free(p_delete_node);
}

static node_t *get_list_node(float x, float y, float z)
{
    node_t *p_new_node = NULL;
    p_new_node = (node_t *)xmalloc(sizeof(node_t));
    p_new_node->x = x;
    p_new_node->y = y;
    p_new_node->z = z;
    p_new_node->prev = NULL;
    p_new_node->next = NULL;
    return (p_new_node);
}

static void *xmalloc(size_t size_in_bytes)
{
    void *p = NULL;
    p = malloc(size_in_bytes);
    assert(p != NULL);
    return (p);
}

//////////////////Client Side////////////////////////////////
/*
list_t *p_list = NULL;
void main(void)
{

    float temp_x, temp_y, temp_z;

    p_list = create_list();

    printf("Right Side :\n");
    for (node_t *p_run = p_list->next; p_run->x > 0.0f; p_run = p_run->next)
    {
        temp_x = p_run->x;
        temp_y = p_run->y;
        temp_z = p_run->z;
        printf("%f , %f , %f \n", temp_x, temp_y, temp_z);
    }

    printf("Left Side :\n");
    for (node_t *p_run = p_list->prev; p_run->x < 0; p_run = p_run->prev)
    {
        temp_x = p_run->x;
        temp_y = p_run->y;
        temp_z = p_run->z;
        printf("%f , %f , %f \n", temp_x, temp_y, temp_z);
    }

    // show_list(p_list, "Test List");

    destroy_list(&p_list);
}

void InitCoordForChairPosition(void)
{

    insert_end(p_list, 8.0f, 2.0f, -2.0f);
    insert_end(p_list, 8.0f, 2.0f, 4.0f);
    insert_end(p_list, 8.0f, 2.0f, 10.0f);
    insert_end(p_list, 8.0f, 2.0f, 16.0f);
    insert_end(p_list, -8.0f, 2.0f, -2.0f);
    insert_end(p_list, -8.0f, 2.0f, 4.0f);
    insert_end(p_list, -8.0f, 2.0f, 10.0f);
    insert_end(p_list, -8.0f, 2.0f, 16.0f);
}
*/