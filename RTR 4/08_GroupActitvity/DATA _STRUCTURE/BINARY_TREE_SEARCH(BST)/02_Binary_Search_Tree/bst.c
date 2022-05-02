#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bst.h"

/* BST Inteface Functions */
bst_t *create_bst(void)
{
	return ((bst_t *)xcalloc(1, sizeof(bst_t)));
}

ret_t insert_bst(bst_t *p_bst, data_t new_data)
{
	bst_node_t *p_new_node = NULL;
	bst_node_t *p_run = NULL;

	p_new_node = get_bst_node(new_data);
	p_run = p_bst->p_root_node;

	if (!p_run)
	{
		p_bst->p_root_node = p_new_node;
		p_bst->nr_elements++;
		return (SUCCESS);
	}

	while (TRUE)
	{
		if (new_data <= p_run->data)
		{
			if (p_run->left == NULL)
			{
				p_run->left = p_new_node;
				p_new_node->parent = p_run;
				p_bst->nr_elements++;
				return (SUCCESS);
			}
			else
			{
				p_run = p_run->left;
				continue;
			}
		}
		else
		{
			if (p_run->right == NULL)
			{
				p_run->right = p_new_node;
				p_new_node->parent = p_run;
				p_bst->nr_elements++;
				return (SUCCESS);
			}
			else
			{
				p_run = p_run->right;
				continue;
			}
		}
	}
}

bool search_bst(bst_t *p_bst, data_t search_data)
{
	return (search_bst_node(p_bst->p_root_node, search_data) != NULL);
}

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

ret_t maximum_bst(bst_t *p_bst, data_t *p_max_data)
{
	bst_node_t *p_run = NULL;

	p_run = p_bst->p_root_node;
	if (p_run == NULL)
		return (BST_EMPTY);

	while (p_run->right != NULL)
		p_run = p_run->right;

	*p_max_data = p_run->data;

	return (SUCCESS);
}

ret_t minimum_bst(bst_t *p_bst, data_t *p_min_data)
{
	bst_node_t *p_run = NULL;

	p_run = p_bst->p_root_node;
	if (p_run == NULL)
		return (BST_EMPTY);

	while (p_run->left != NULL)
		p_run = p_run->left;

	*p_min_data = p_run->data;

	return (SUCCESS);
}

ret_t inorder_successor(bst_t *p_bst, data_t e_data, data_t *p_succ_data)
{
	bst_node_t *pe_node = NULL;
	bst_node_t *p_succ_node = NULL;

	pe_node = search_bst_node(p_bst->p_root_node, e_data);
	if (pe_node == NULL)
		return (BST_DATA_NOT_FOUND);

	p_succ_node = inorder_successor_node(pe_node);
	if (p_succ_node != NULL)
	{
		*p_succ_data = p_succ_node->data;
		return (SUCCESS);
	}

	return (BST_NO_SUCCESSOR);
}

ret_t remove_bst(bst_t *p_bst, data_t d_data)
{
	bst_node_t *z = NULL; /* Address of node to be deleted */
	bst_node_t *y = NULL; /* Address of successor node */

	z = search_bst_node(p_bst->p_root_node, d_data);
	if (z == NULL)
		return (BST_DATA_NOT_FOUND);

	if (z->left == NULL)
		transplant_bst(p_bst, z, z->right);
	else if (z->right == NULL)
		transplant_bst(p_bst, z, z->left);
	else
	{
		y = inorder_successor_node(z);

		if (z->right != y)
		{
			transplant_bst(p_bst, y, y->right);
			y->right = z->right;
			y->right->parent = y;
		}

		transplant_bst(p_bst, z, y);
		y->left = z->left;
		y->left->parent = y;
	}

	free(z);
	z = NULL;

	p_bst->nr_elements--;

	return (SUCCESS);
}

ret_t destroy_bst(bst_t **pp_bst)
{
	bst_t *p_bst = NULL;

	p_bst = *pp_bst;
	__destroy_bst_r(p_bst->p_root_node);
	free(p_bst);
	*pp_bst = NULL;

	return (SUCCESS);
}

/* BST Helper Functions */
bst_node_t *search_bst_node(bst_node_t *p_root_node, data_t search_data)
{
	bst_node_t *p_run = NULL;

	p_run = p_root_node;

	while (p_run != NULL)
	{
		if (p_run->data == search_data)
			return (p_run);
		else if (p_run->data < search_data)
			p_run = p_run->right;
		else
			p_run = p_run->left;
	}

	return (NULL);
}

bst_node_t *inorder_successor_node(bst_node_t *p_bst_node)
{
	bst_node_t *x = NULL;
	bst_node_t *y = NULL;
	bst_node_t *p_run = NULL;

	/* case 1 */
	x = p_bst_node;
	if (x->right != NULL)
	{
		p_run = x->right;
		while (p_run->left != NULL)
			p_run = p_run->left;

		return (p_run);
	}

	/* case 2 */
	y = x->parent;
	while (y != NULL && x == y->right)
	{
		x = y;
		y = x->parent;
	}

	return (y);
}

bst_node_t *inorder_predecessor_node(bst_node_t *p_bst_node)
{
	bst_node_t *x = NULL;
	bst_node_t *y = NULL;
	bst_node_t *p_run = NULL;

	x = p_bst_node;

	/* case 1 */
	if (x->left != NULL)
	{
		p_run = x->left;
		while (p_run->right != NULL)
			p_run = p_run->right;

		return (p_run);
	}

	/* case 2 */
	y = x->parent;
	while (y != NULL && x == y->left)
	{
		x = y;
		y = x->parent;
	}

	return (y);
}

void __inorder_r(bst_node_t *p_root_node)
{
	if (p_root_node != NULL)
	{
		__inorder_r(p_root_node->left);
		printf("%d\n", p_root_node->data);
		__inorder_r(p_root_node->right);
	}
}

void __preorder_r(bst_node_t *p_root_node)
{
	if (p_root_node != NULL)
	{
		printf("%d\n", p_root_node->data);
		__preorder_r(p_root_node->left);
		__preorder_r(p_root_node->right);
	}
}

void __postorder_r(bst_node_t *p_root_node)
{
	if (p_root_node != NULL)
	{
		__postorder_r(p_root_node->left);
		__postorder_r(p_root_node->right);
		printf("%d\n", p_root_node->data);
	}
}

void __destroy_bst_r(bst_node_t *p_root_node)
{
	if (p_root_node != NULL)
	{
		__destroy_bst_r(p_root_node->left);
		__destroy_bst_r(p_root_node->right);
		free(p_root_node);
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

bst_node_t *get_bst_node(data_t new_data)
{
	bst_node_t *p_new_node = NULL;

	p_new_node = (bst_node_t *)xcalloc(1, sizeof(bst_node_t));
	p_new_node->data = new_data;

	return (p_new_node);
}

void *xcalloc(size_t n, size_t s)
{
	void *p = NULL;

	p = calloc(n, s);
	assert(p);

	return (p);
}
