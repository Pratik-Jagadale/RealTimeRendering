#ifdef _HASH_H
#define _HASH_H

// variable constants
#define TRUE 1
#define FALSE 0
#define SUCCESS 1
#define LITS_DATA_NOT_FOUND 2
#define BAD_KEY LITS_DATA_NOT_FOUND

// typedefs
typedef int key_t;
typedef int len_t;
typedef int status_t;
typedef	struct node node_t;
typedef node_t list_t;
typedef struct hashq hashq_t;


//stuct definitions
struct node
{
	key_t key;
	node_t* prev;
	node_t* next;
};

struct hashq
{
	list_t** p_list_arr;
	int bucket_size;
};

/* hashq Interface routines */
hashq_t* create_hashq(int bucket_size);
status_t add_key(hashq_t* p_hashq, key_t key);
status_t remove_key(hashq_t* p_hashq, key_t key);
status_t search_key(hashq_t* p_hashq, key_t key);
status_t destroy_hashq(hashq_t** p_hashq);
void hash_stat(hashq_t* p_hashq);

/* List Interface Routines */
list_t* create_list(void);
status_t insert_end(list_t* p_list, key_t key );
status_t remove_data(list_t* p_list, key_t key);
status_t search_data(list_t* p_list, key_t key);
node_t* search_node(list_t* p_list, key_t key);
len_t get_list_length(list_t* p_list);
status_t destroy_list(list_t* p_list);

/* List Helper Routines */
void generic_insert(node_t* p_beg,node_t* p_mid,node_t* p_end );
void generic_delete(node_t* p_delete);
node_t* get_node(key_t key);

void* xmalloc(int n);

inline int hash(key_t key, int bucket_size)
{
	return (key % bucket_size);
}

#endif /* _HAS_H */
