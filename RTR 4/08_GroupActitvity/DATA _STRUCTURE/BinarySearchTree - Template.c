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
void __inorder_r(struct bst_node *p_root_node);
void __preorder_r(struct bst_node *p_root_node);
void __postorder_r(struct bst_node *p_root_node);
void __destroy_bst_r(struct bst_node *p_root_node);
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
	printf("PRESS 9 - To Find The Minimum Value Value.\n");
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
	return (NULL);
}

ret_t insert_bst(struct bst *p_bst, int new_data)
{
	return (NULL);
}

bool search_bst(struct bst *p_bst, int search_data)
{
	return (FALSE);
}

void inorder_r(struct bst *p_bst)
{
}

void preorder_r(struct bst *p_bst)
{
}

void postorder_r(struct bst *p_bst)
{
}

ret_t maximum_bst(struct bst *p_bst, int *p_max_data)
{
	return (SUCCESS);
}

ret_t minimum_bst(struct bst *p_bst, int *p_min_data)
{

	return (SUCCESS);
}

ret_t inorder_successor(struct bst *p_bst, int e_data, int *p_succ_data)
{

	return (BST_NO_SUCCESSOR);
}

ret_t remove_bst(struct bst *p_bst, int d_data)
{

	return (SUCCESS);
}

ret_t destroy_bst(struct bst **pp_bst)
{

	return (SUCCESS);
}

// Helper Functions
struct bst_node *search_bst_node(struct bst_node *p_root_node, int search_data)
{

	return (NULL);
}

struct bst_node *inorder_successor_node(struct bst_node *p_bst_node)
{

	return (NULL);
}

void __inorder_r(struct bst_node *p_root_node)
{
}

void __preorder_r(struct bst_node *p_root_node)
{
}

void __postorder_r(struct bst_node *p_root_node)
{
}

void __destroy_bst_r(struct bst_node *p_root_node)
{
}

void transplant_bst(struct bst *p_bst, struct bst_node *u, struct bst_node *v)
{
}

struct bst_node *get_bst_node(int new_data)
{
	return (NULL);
}

void *xmalloc(size_t nr_bytes)
{
	return (NULL);
}
