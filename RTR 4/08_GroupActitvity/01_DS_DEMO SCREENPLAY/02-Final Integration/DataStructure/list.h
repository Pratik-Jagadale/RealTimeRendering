/* Doubly Circular Linked List */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* Symbolic constants */

#define TRUE 1
#define FALSE 0
#define SUCCESS 1
#define FAILURE 0
#define LIST_DATA_NOT_FOUND 2
#define LIST_EMPTY 3

/* typedefs */
typedef int data_t;
typedef int status_t;
typedef int len_t;
typedef struct node node_t;
typedef node_t list_t;

// typedef enum {FALSE=0, TRUE} bool;

/* node layout definition */
struct node
{
    float x;
    float y;
    float z;
    struct node *prev;
    struct node *next;
};

/* interface routines declarations */
list_t *create_list(void);

status_t insert_beg(list_t *p_list, float x, float y, float z);
status_t insert_end(list_t *p_list, float x, float y, float z);

status_t get_beg(list_t *p_list, float *x, float *y, float *z);
status_t get_end(list_t *p_list, float *x, float *y, float *z);

status_t remove_beg(list_t *p_list);
status_t remove_end(list_t *p_list);
status_t remove_data(list_t *p_list, data_t r_data);

len_t get_length(list_t *p_list);
bool is_list_empty(list_t *p_list);
int get_repeat_count(list_t *p_list, data_t search_data);
void show_list(list_t *p_list, const char *msg);

status_t destroy_list(list_t **pp_list);

/* list auxillary routines */
static void generic_insert(node_t *p_beg, node_t *p_mid, node_t *p_end);
static void generic_delete(node_t *p_delete_node);
static node_t *locate_node(list_t *p_list, data_t search_data);
static node_t *get_list_node(float, float, float);

/* memory allocation routines */

static void *xmalloc(size_t size_in_bytes);