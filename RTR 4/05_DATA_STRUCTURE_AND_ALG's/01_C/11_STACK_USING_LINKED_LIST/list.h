#include <stdio.h>
#include <stdlib.h>

/* Symbolic constants */
#define SUCCESS 1
#define TRUE 1
#define FALSE 0
#define FAILURE 0
#define LIST_DATA_NOT_FOUND 2
#define LIST_EMPTY 3

/* Typedefs */
typedef struct node node_t;
typedef int data_t;
typedef int status_t;
typedef int len_t;
typedef node_t list_t;
typedef int bool;

struct node
{
    data_t data;
    node_t *prev;
    node_t *next;
};

/* list - Interface Routines declartions*/
list_t *create_list(void);

status_t insert_beg(list_t *p_list, data_t new_data);
status_t insert_end(list_t *p_list, data_t new_data);
status_t insert_after(list_t *p_list, data_t e_data, data_t new_data);
status_t insert_before(list_t *p_list, data_t e_data, data_t new_data);

status_t get_beg(list_t *p_list, data_t *p_data);
status_t get_end(list_t *p_list, data_t *p_data);
status_t pop_beg(list_t *p_list, data_t *p_data);
status_t pop_end(list_t *p_list, data_t *p_data);

status_t remove_beg(list_t *p_list);
status_t remove_end(list_t *p_list);
status_t remove_data(list_t *p_list, data_t data);

len_t get_length(list_t *p_list);
bool is_list_empty(list_t *p_list);
bool search_data(list_t *p_list, data_t search_data);
int get_repeate_count(list_t *p_list, data_t search_data);
void show_list(list_t *p_list, const char *msg);

/* list - auxillary Routines */
void generic_insert(node_t *p_beg, node_t *p_mid, node_t *p_end);
status_t destroy_list(list_t **pp_list);
void generic_delete(node_t *p_delete_node);
node_t *locate_node(list_t *p_list, data_t search_data);
node_t *get_node(data_t new_data);

/* Generic Helper Function */
void *xmalloc(int nr_bytes);
