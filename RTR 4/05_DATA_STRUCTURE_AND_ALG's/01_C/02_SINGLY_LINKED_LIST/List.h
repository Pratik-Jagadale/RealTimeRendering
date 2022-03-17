#pragma once

#define SUCCESS 1
#define TRUE 1
#define FALSE 0
#define LIST_DATA_NOT_FOUND 2
#define LIST_EMPTY 3

typedef int data_t;
typedef int status_t;
typedef int lent_t;
typedef int Bool;
typedef int len_t;
typedef struct node node_t;
typedef struct list list_t;

enum{
    true,
    false
};

struct node
{
    data_t data;
    node_t *next;
};

struct list
{
    node_t *p_head_node;
};

/* Helper Routines */
list_t *create_list(void);

status_t insert_start(list_t *p_list, data_t new_data);
status_t insert_end(list_t *p_list, data_t new_data);
status_t insert_after(list_t* p_list, data_t e_data, data_t new_data);
status_t insert_before(list_t* p_list, data_t e_data, data_t new_data);

status_t get_start(list_t* p_list, data_t* p_start_data);
status_t get_end(list_t* p_list, data_t* p_end_data);
status_t pop_start(list_t* p_list, data_t* p_start_data);
status_t pop_end(list_t* p_list, data_t* p_end_data);

status_t remove_start(list_t* p_list);
status_t remove_end(list_t* p_list);
status_t remove_data(list_t* p_list, data_t r_data);

len_t get_length(list_t* p_list);
bool is_empty(list_t* p_list);
bool is_member(list_t* p_list, data_t s_data);
void show(list_t* p_list);

list_t* concate(list_t* p_list_1, list_t* p_list_2);
list_t* merge(list_t* p_list_1, list_t* p_list_2);
list_t* get_reversed_list(list_t* p_list);

status_t append(list_t* p_list_1, list_t* p_list_2);
status_t to_list(data_t* p_array, size_t size);

status_t destroy(list_t** pp_list);


/* auxillary routines */
static void generic_insert(node_t *p_beg, node_t *p_mid, node_t *p_end);
static void generic_delete(node_t *p_prev_node, node_t *p_delete_node);
static node_t *search_node(list_t *p_list, data_t s_data);
static void get_node_and_prev(list_t *p_list, data_t s_data, node_t **pp_node, node_t **pp_prev_node);
static void get_last_node(list_t *p_list, node_t **pp_last_node);
static void get_last_node_and_prev(list_t *p_list, node_t **pp_node, node_t **pp_prev_node);
static node_t *get_node(data_t new_data);

static void *xmalloc(size_t nr_bytes);
