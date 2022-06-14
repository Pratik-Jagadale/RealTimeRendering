// Header Files
#include <stdio.h>
#include <stdlib.h>

// Symbolic Constants
#define SUCCESS 1
#define TRUE 1
#define FALSE 0

#define BST_DATA_NOT_FOUND 2
#define BST_NO_SUCCESSOR 3
#define BST_EMPTY 5

// Typedefs
typedef int ret_t;
typedef int bool;

// User Define Data Type (Structs)
struct bst_node
{
	int data;
	struct bst_node *left;
	struct bst_node *right;
	struct bst_node *parent;
};

struct bst
{
	struct bst_node *p_root_node;
	int nr_elements;
};

// Function Declarations
struct bst *create_bst(void);

ret_t insert_bst(struct bst *p_bst, int new_data);

bool search_bst(struct bst *p_bst, int search_data);
void inorder_r(struct bst *p_bst);
void preorder_r(struct bst *p_bst);
void postorder_r(struct bst *p_bst);
ret_t maximum_bst(struct bst *p_bst, int *p_max_data);
ret_t minimum_bst(struct bst *p_bst, int *p_min_data);
ret_t inorder_successor(struct bst *p_bst, int e_data, int *p_succ_data);
ret_t remove_bst(struct bst *p_bst, int d_data);
ret_t destroy_bst(struct bst **pp_bst);

struct bst_node *search_bst_node(struct bst_node *p_root_node, int search_data);
struct bst_node *get_bst_node(int new_data);

struct bst_node *inorder_successor_node(struct bst_node *p_bst_node);

void __inorder_r(struct bst_node *p_run);
void __preorder_r(struct bst_node *p_run);
void __postorder_r(struct bst_node *p_run);
void __destroy_bst_r(struct bst_node *p_run);
void transplant_bst(struct bst *p_bst, struct bst_node *u, struct bst_node *v);

void *xmalloc(size_t nr_bytes);

// Entry Point Function
int main(void)
{
	// function prototypes
	int ShowMainMenu(void);

	// variables
	int choice;
	int data;
	ret_t result;
	struct bst *p_bst = NULL;
	bool iFlag = TRUE;

	// code
	do
	{
		choice = ShowMainMenu();

		switch (choice)
		{
		case 1:
			printf("--------------------------------------------------------------\n\n");
			if (p_bst == NULL)
			{
				p_bst = create_bst();
				printf("Binary Search Created Successfully..\n\n");
			}
			else
				printf("Binary Search Tree is already Exits !!! \nIf You Want To Create New One! Then, Destroy Previous And then Create Another One.\n\n");
			printf("--------------------------------------------------------------\n");
			break;

		case 2:

			if (p_bst != NULL)
			{
				printf("Enter The Value To Insert Data Into BST : ");
				scanf("%d", &data);
				printf("--------------------------------------------------------------\n\n");
				result = insert_bst(p_bst, data);
				if (result == SUCCESS)
					printf("Data : %d was Added Successfully.\n\n", data);
				else
					printf("Data Not Added Successfully.\n\n");
			}
			else
				printf("Binary Search Tree Does Not Exist. Please Create The BST first.\n\n");
			printf("--------------------------------------------------------------\n");
			break;

		case 3:
			if (p_bst != NULL)
			{
				printf("Enter The Value To Search Data Into BST : ");
				scanf("%d", &data);

				result = search_bst(p_bst, data);
				printf("--------------------------------------------------------------\n\n");
				if (result == SUCCESS)
				{
					printf("Given Data : %d was Exist in the Binary Search Tree.\n\n", data);
				}
				else
					printf("Data : %d was Does Exist in the Binary Search Tree.\n\n", data);
			}
			else
				printf("Binary Search Tree Does Not Exist. Please Create The BST first.\n\n");
			printf("--------------------------------------------------------------\n");
			break;

		case 4:
			if (p_bst != NULL)
			{
				printf("Enter The Data To remove Data From BST : ");
				scanf("%d", &data);

				result = remove_bst(p_bst, data);
				if (result == SUCCESS)
				{
					printf("--------------------------------------------------------------\n\n");
					printf("Data : %d was Sucessfully Removed From the Binary Search Tree.\n\n", data);
				}
				else // means result == BST_DATA_NOT_FOUND
					printf("Data : %d was Does Exist in the Binary Search Tree.\n\n", data);
			}
			else
				printf("Binary Search Tree Does Not Exist. Please Create The BST first.\n\n");
			printf("--------------------------------------------------------------\n");
			break;

		case 5:
			if (p_bst != NULL)
			{
				inorder_r(p_bst);
			}
			else
				printf("Binary Search Tree Does Not Exist. Please Create The BST first.\n\n");

			break;

		case 6:
			if (p_bst != NULL)
			{
				preorder_r(p_bst);
			}
			else
				printf("Binary Search Tree Does Not Exist. Please Create The BST first.\n\n");

			break;

		case 7:
			if (p_bst != NULL)
			{
				postorder_r(p_bst);
			}
			else
				printf("Binary Search Tree Does Not Exist. Please Create The BST first.\n\n");

			break;

		case 8:
			if (p_bst != NULL)
			{
				result = maximum_bst(p_bst, &data);
				if (result == SUCCESS)
				{
					printf("--------------------------------------------------------------\n\n");
					printf("Maximum Value From The Binary Search Tree is : %d\n\n", data);
				}
				else // means BST Empty
					printf("BST Is Empty.\n\n");
			}
			else
				printf("Binary Search Tree Does Not Exist. Please Create The BST first.\n\n");
			printf("--------------------------------------------------------------\n");
			break;

		case 9:
			if (p_bst != NULL)
			{
				result = minimum_bst(p_bst, &data);
				if (result == SUCCESS)
				{
					printf("--------------------------------------------------------------\n\n");
					printf("Minimum Value From The Binary Search Tree is : %d\n\n", data);
				}
				else // means BST Empty
					printf("BST Is Empty.\n\n");
			}
			else
				printf("Binary Search Tree Does Not Exist. Please Create The BST first.\n\n");
			printf("--------------------------------------------------------------\n");
			break;

		case 10:
			if (p_bst != NULL)
			{
				printf("--------------------------------------------------------------\n\n");
				printf("Number Of Elements In The Binary Search Tree Is = %d \n\n", p_bst->nr_elements);
			}
			else
				printf("Binary Search Tree Does Not Exist.\n\n");
			printf("--------------------------------------------------------------\n");
			break;

		case 11:
			if (p_bst != NULL)
			{
				result = destroy_bst(&p_bst);
				if (result == SUCCESS)
				{
					printf("--------------------------------------------------------------\n\n");
					printf("BST Destroyed Sucessfully \n\n");
					printf("--------------------------------------------------------------\n");
				}
			}
			else
				printf("Binary Search Tree Does Not Exist.\n\n");

			break;

		default:
			if (p_bst != NULL)
			{
				result = destroy_bst(&p_bst);
				if (result == SUCCESS)
				{
					printf("--------------------------------------------------------------\n\n");
					printf("BST Destroyed Sucessfully \n\n");
					printf("--------------------------------------------------------------\n");
					iFlag = FALSE;
				}
			}
			return 0;
		}
	} while (iFlag);

	return 0;
}

// Function Definutions
int ShowMainMenu()
{
	// variables
	int choice;

	// code
	printf("**************************************\n");
	printf("ENTER THE NUMBER ALONGSIDE YOUR CHOICE : \n\n");
	printf("PRESS 1 - To Create Binary Search Tree .\n");
	printf("PRESS 2 - To Add An Element Into The Binary Search Tree.\n");
	printf("PRESS 3 - To Search The Value Into The Binary Search Tree .\n");
	printf("PRESS 4 - To Remove the Value From the Binary Search Tree\n");
	printf("PRESS 5 - To Display the Binary Search Tree in InOrder Sequence.\n");
	printf("PRESS 6 - To Display the Binary Search Tree in PreOrder Sequence.\n");
	printf("PRESS 7 - To Display the Binary Search Tree in Postorder Sequence.\n");
	printf("PRESS 8 - To Find The Maximum Value.\n");
	printf("PRESS 9 - To Find The Minimum Value.\n");
	printf("PRESS 10 - To Find The Number Of Elements in BST.\n");
	printf("PRESS 11 - To Destroy BST.\n");
	printf("PRESS    - Any Key other than 1 to 11 - To Exit.\n\n");
	printf("**************************************\n");

	printf("CHOICE = ");
	scanf("%d", &choice);
	printf("\n\n");
	return (choice);
}

struct bst *create_bst(void)
{
	// variable declartions
	struct bst *p_bst = NULL;

	// code
	p_bst = (struct bst *)xmalloc(sizeof(struct bst));

	p_bst->p_root_node = NULL;
	p_bst->nr_elements = 0;

	return (p_bst);
}

ret_t insert_bst(struct bst *p_bst, int new_data)
{
	// Variable declarations
	struct bst_node *p_new_node = NULL;
	struct bst_node *p_run = NULL;

	// code
	p_new_node = get_bst_node(new_data);
	p_run = p_bst->p_root_node;

	// case 1 : Tree is Empty
	if (!p_run)
	{
		p_bst->p_root_node = p_new_node;
		p_bst->nr_elements++;
		return (SUCCESS);
	}

	// case 2: Tree is Non Empty
	while (TRUE)
	{
		if (new_data <= p_run->data)
		{
			if (p_run->left == NULL)
			{
				// add new node
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
				// add new node
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

bool search_bst(struct bst *p_bst, int search_data)
{
	return (search_bst_node(p_bst->p_root_node, search_data) != NULL);
}

void inorder_r(struct bst *p_bst)
{
	printf("---------------------------------------------------\n");
	printf("Inorder Traversal of Binary Search Tree : \n");
	__inorder_r(p_bst->p_root_node);
	printf("---------------------------------------------------\n");
}

void preorder_r(struct bst *p_bst)
{
	printf("---------------------------------------------------\n");
	printf("Preorder Traversal of Binary Search Tree : \n");
	__preorder_r(p_bst->p_root_node);
	printf("---------------------------------------------------\n");
}

void postorder_r(struct bst *p_bst)
{
	printf("---------------------------------------------------\n");
	printf("PostOrder Traversal of Binary Search Tree : \n");
	__postorder_r(p_bst->p_root_node);
	printf("---------------------------------------------------\n");
}

ret_t maximum_bst(struct bst *p_bst, int *p_max_data)
{
	struct bst_node *p_run = NULL;

	p_run = p_bst->p_root_node;
	if (p_run == NULL)
		return (BST_EMPTY);

	while (p_run->right != NULL)
		p_run = p_run->right;

	*p_max_data = p_run->data;

	return (SUCCESS);
}

ret_t minimum_bst(struct bst *p_bst, int *p_min_data)
{
	struct bst_node *p_run = NULL;

	p_run = p_bst->p_root_node;
	if (p_run == NULL)
		return (BST_EMPTY);

	while (p_run->left != NULL)
		p_run = p_run->left;

	*p_min_data = p_run->data;

	return (SUCCESS);
}

ret_t inorder_successor(struct bst *p_bst, int e_data, int *p_succ_data)
{
	struct bst_node *pe_node = NULL;	 // temp
	struct bst_node *p_succ_node = NULL; // temp successor

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

ret_t remove_bst(struct bst *p_bst, int d_data)
{
	struct bst_node *z = NULL; /* Address of node to be deleted */ // remove
	struct bst_node *y = NULL; /* Address of successor node */	   // sucessor - remove -> right subtree -> lesser

	z = search_bst_node(p_bst->p_root_node, d_data);
	if (z == NULL)
		return (BST_DATA_NOT_FOUND);

	/*
	case 1:
	z->left == NULL
	and
	z->right may or may not be NULL
	*/

	if (z->left == NULL)
	{
		// parent
		if (z->parent == NULL)
			p_bst->p_root_node = z->right;
		else if (z == z->parent->left)
			z->parent->left = z->right;
		else
			z->parent->right = z->right;

		// child
		if (z->right)
			z->right->parent = z->parent;
	}
	/*
	case 2:
	z->left !=  NULL
	z->right == NULL
	*/
	else if (z->right == NULL)
	{
		if (z->parent == NULL)
			p_bst->p_root_node = z->left;
		else if (z == z->parent->left)
			z->parent->left = z->left;
		else
			z->parent->right = z->left;

		if (z->left)
			z->left->parent = z->parent;
	}
	else
	{
		/*case 3: z->left != NULL and z->right != NULL */
		y = z->right; // successor
		while (y->left != NULL)
			y = y->left;

		/*when y is immediate right of z */
		if (y != z->right)
		{
			y->parent->left = y->right; // NULL
			if (y->right)
				y->right->parent = y->parent;

			/* Win over z's right subtree */
			y->right = z->right;
			y->right->parent = y;
		}

		/* Win Over z's parent */
		if (z->parent == NULL)
			p_bst->p_root_node = y;
		else if (z == z->parent->left)
			z->parent->left = y;
		else
			z->parent->right = y;

		y->parent = z->parent;

		/* win over z's left subtree */
		y->left = z->left;
		y->left->parent = y;
	}

	free(z);
	z = NULL;

	p_bst->nr_elements--;

	return (SUCCESS);
}

ret_t destroy_bst(struct bst **pp_bst)
{
	struct bst *p_temp = NULL; // temp

	p_temp = *pp_bst;
	__destroy_bst_r(p_temp->p_root_node);
	free(p_temp);
	*pp_bst = NULL;

	return (SUCCESS);
}

// Helper Functions
struct bst_node *search_bst_node(struct bst_node *p_root_node, int search_data)
{
	struct bst_node *p_run = NULL;

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

struct bst_node *inorder_successor_node(struct bst_node *p_bst_node)
{
	struct bst_node *x = NULL;
	struct bst_node *y = NULL;

	struct bst_node *p_run = NULL;

	/* case 1 */
	if (p_bst_node->right != NULL)
	{
		p_run = p_bst_node->right;
		while (p_run->left != NULL)
			p_run = p_run->left;

		return (p_run);
	}

	/* case 2 */
	x = p_bst_node;
	y = x->parent;

	while (y != NULL && x == y->right)
	{
		x = y;
		y = x->parent;
	}

	return (y);
}

void __inorder_r(struct bst_node *p_run)
{
	if (p_run != NULL)
	{
		__inorder_r(p_run->left);
		printf("%d\n", p_run->data);
		__inorder_r(p_run->right);
	}
}

void __preorder_r(struct bst_node *p_run)
{
	if (p_run != NULL)
	{
		printf("%d\n", p_run->data);
		__preorder_r(p_run->left);
		__preorder_r(p_run->right);
	}
}

void __postorder_r(struct bst_node *p_run)
{
	if (p_run != NULL)
	{
		__postorder_r(p_run->left);
		__postorder_r(p_run->right);
		printf("%d\n", p_run->data);
	}
}

void __destroy_bst_r(struct bst_node *p_run)
{
	if (p_run != NULL)
	{
		__destroy_bst_r(p_run->left);
		__destroy_bst_r(p_run->right);

		free(p_run);
	}
}

void transplant_bst(struct bst *p_bst, struct bst_node *u, struct bst_node *v)
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

struct bst_node *get_bst_node(int new_data)
{
	struct bst_node *p_new_node = NULL;

	p_new_node = (struct bst_node *)xmalloc(sizeof(struct bst_node));

	p_new_node->data = new_data;
	p_new_node->left = NULL;
	p_new_node->right = NULL;
	p_new_node->parent = NULL;
	return (p_new_node);
}

void *xmalloc(size_t nr_bytes)
{
	void *p = NULL;

	p = malloc(nr_bytes);
	if (p == NULL)
	{
		printf("Memmory Allocation Failed!!! EXITTING NOW...\n");
		exit(0);
	}
	return (p);
}
