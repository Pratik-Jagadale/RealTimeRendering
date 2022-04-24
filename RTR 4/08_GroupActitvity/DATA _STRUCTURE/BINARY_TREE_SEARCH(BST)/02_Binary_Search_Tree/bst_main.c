#include <stdio.h>
#include <stdlib.h> 
#include <assert.h> 
#include "bst.h"

int main(void)
{
	bst_t *p_bst = NULL; 
	static data_t d_array[] = {12, 9, 5, 4, 7, 10, 15, 13, 19, 16}; 
	ret_t ret; 
	int i; 

	p_bst = create_bst(); 
	for(i = 0; i < sizeof(d_array) / sizeof(d_array[0]); i++)
		assert(insert_bst(p_bst, d_array[i]) == SUCCESS); 

	puts("PRE-ORDER RECURSIVE"); 
	preorder_r(p_bst); 

	puts("IN-ORDER RECURSIVE"); 
	inorder_r(p_bst); 

	puts("POST-ORDER RECURSIVE"); 
	postorder_r(p_bst); 

	puts("PRE-ORDER NON-RECURSIVE"); 
	preorder_nrc(p_bst); 

	puts("IN-ORDER NON-RECURSIVE");
	inorder_nrc(p_bst); 

	puts("POST-ORDER NON-RECURSIVE");
	postorder_nrc(p_bst); 

	assert(destroy_bst(&p_bst) && !p_bst); 

	return (EXIT_SUCCESS); 
}