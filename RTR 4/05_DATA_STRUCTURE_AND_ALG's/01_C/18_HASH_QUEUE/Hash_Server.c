#include <stdio.h>
#include <stdlib.h>
#include "Hash.h"

/* hashq Interface routines */
hashq_t* create_hashq(int bucket_size)
{
	hashq_t* p_hashq = NULL;
	p_hashq = (hashq_t*)malloc(hashq_t);

	p_hashq->bucket_size = bucket_size;

	list_t* p_list_arr = (list_t*)malloc(bucket_size * sizeof(list_t*));
	p_hashq->p_list_arr = p_list_arr; 

	for(int i = 0 ; i < bucket_size ; i++)
		p_hashq->p_list_arr[i] = create_list();
	return (p_hashq);
}	

status_t add_key(hashq_t* p_hashq, key_t key)
{
	status_t status;
	int bucket_number;
	bucket_number = hash(key, p_hashq->bucket_size);

	status = insert_end(hashq_t->p_list_arr[bucket_number],key);

	return	(status);
}

status_t remove_key(hashq_t* p_hashq, key_t key)
{
	status_t status;
	int bucket_number;
	bucket_number = hash(key, p_hashq->bucket_size);

	status = remove_data(p_hashq->p_list_arr[bucket_number] , key);
	return (status);
}

status_t search_key(hashq_t* p_hashq, key_t key)
{
	status_t status;
	int bucket_number;
	bucket_number = hash(key, p_hashq->bucket_size);

	status = search_data(p_hashq->p_list_arr[bucket_number] , key);
	return (status);
}

status_t destroy_hashq(hashq_t** p_hashq)
{
	status_t status;

	for (int i = 0;i < p_hashq->bucket_size; i++)
	{
		/* code */
		status = destroy_list(p_hashq->p_list_arr[i]);
		if(status == FALSE)
		{
			fprintf(stderr, "destroy_list Failed\n", );
			exit(0);
		}
		p_hashq->p_list_arr[i] = NULL;
	}
	free(p_hashq);
	p_hashq = NULL;
	return (SUCCESS);
}

void hash_stat(hashq_t* p_hashq)
{
	int i ;
	for( i = 0 ; i < p_hashq->bucket_size ; i++)
	{
		printf("Bucket Number = %d , Number of Keys: %d\n",i, get_list_length(p_hashq->p_list_arr[i]));
	}
	printf("Total Number of Buckets is = %d\n",i);
}

/* List Interface Routines */
list_t* create_list(void)
{
	list_t* p_list = NULL;
	p_list = (list_t*)malloc(list_t);
	if(p_list == NULL)
	{
		fprintf(stderr, "Failed To Allocation\n", );
		exit(0);
	}
	p_list->key = 0;
	p_list->prev = p_list;
	p_list->next = p_list;
	return(p_list);
}

status_t insert_end(list_t* p_list, key_t key )
{
	generic_insert(p_list->p_end , get_node(key), p_list);
	return (SUCCESS);	
}

status_t remove_data(list_t* p_list, key_t key)
{
	node_t* p_run = NULL;
	p_run = p_list->next;
	for(p_run; p_run->next != p_list; p_run = p_run->next )
		if(key == p_run->key)
		{
			generic_delete(p_run);
			return(SUCCESS);
		}

	return (FALSE);
}

status_t search_data(list_t* p_list, key_t key)
{
	node_t* p_run = NULL;
	p_run = p_list->next;
	for(p_run; p_run->next != p_list; p_run = p_run->next )
		if(key == p_run->key)
			return (SUCCESS);
	return (FALSE);
}

node_t* search_node(list_t* p_list, key_t key)
{
	node_t* p_run = NULL;
	p_run = p_list->next;
	for(p_run; p_run->next != p_list; p_run = p_run->next )
		if(key == p_run->key)
			return (p_run);
	return (NULL);
}

len_t get_list_length(list_t* p_list)
{
	node_t* p_run = NULL;
	len_t len = 0;
	p_run = p_list->next;
	for(p_run; p_run->next != p_list; p_run = p_run->next )
		len++;
	return (len);
}

status_t destroy_list(list_t* p_list)
{
	node_t* p_run = NULL;
	node_t* p_run_next = NULL;

	for(p_run = list_t->next ; p_run != p_list ; p_run = p_run_next)
	{
		p_run_next = p_run->next;
		generic_delete(p_run);
	}
	free(p_list);
	p_list = NULL;
}

/* List Helper Routines */
void generic_insert(node_t* p_beg,node_t* p_mid,node_t* p_end )
{
	p_mid->next = p_end;
	p_mid->prev = p_beg;
	p_beg->next = p_mid;
	p_end->prev = p_mid;
}

void generic_delete(node_t* p_delete)
{
	p_delete->prev->next = p_delete->next;
	p_delete->next->prev = p_delete->prev;
}

node_t* get_node(key_t key)
{
	node_t* p_node = NULL;
	p_node = (node_t*)xmalloc(sizeof(node_t));

	p_node->key = key;
	p_node->prev = NULL;
	p_node->next = NULL;
	return (p_node);
}

void* xmalloc(int n)
{
	void* p = NULL;
	p = malloc(n);

	if(p == NULL)
	{
		fprintf(stderr,"Memmory Allocation Failed\n");
	}	
	return (p);
}
