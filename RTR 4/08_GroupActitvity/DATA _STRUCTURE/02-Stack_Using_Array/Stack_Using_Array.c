// Array.c
#include <stdio.h>	// for printf(), scanf()
#include <conio.h>	// for getch()
#include <stdlib.h> //
#include <ctype.h>	// for toupper

#define MAX_STACK_SIZE 20

// function declartions

void ShowStackFullMessage(void);
void ShowStackEmptyMessage(void);

int main(int argc, char *argv[])
{
	// function prototypes
	int ShowMainMenu(void);
	void CreatAStack(int[], int *);
	void PushAnElementIntoTheStack(int[], int *);
	void PopElementFromTheStack(int[], int *);
	void DisplayTheStack(int[], int *);

	// varibale declarrtions
	int stackTopIndex = -1;
	int iArray[MAX_STACK_SIZE];
	int choice;

	// code
	do
	{
		choice = ShowMainMenu();
		switch (choice)
		{
		case 1:
			CreatAStack(iArray, &stackTopIndex);
			break;

		case 2:
			PushAnElementIntoTheStack(iArray, &stackTopIndex);
			break;

		case 3:
			PopElementFromTheStack(iArray, &stackTopIndex);
			break;

		case 4:
			DisplayTheStack(iArray, &stackTopIndex);
			break;

		case 5:
			exit(0);
			break;

		default:
			break;
		}
	} while (choice > 0 && choice < 6);

	return 0;
}

int ShowMainMenu(void)
{
	// varibale declartions
	int choice;

	// code
	printf("\n-----------------------------------------------\n");
	printf("ENTER THE NUMBER ALONGSIDE YOUR CHOICE : \n\n");
	printf("PRESS 1 - To CREATE A STACK\n");
	printf("PRESS 2 - TO PUSH AN ELEMENT INTO THE STACK\n");
	printf("PRESS 3 - To POP AN ELEMNET FROM THE STACK\n");
	printf("PRESS 4 - TO DISPLAY THE STACK\n");
	printf("PRESS 5 - (OR ANY KEY OTHER THAN 1-5) - To EXIT\n");
	printf("CHOICE  : ");
	scanf("%d", &choice);
	printf("\n\n");
	return (choice);
}

void CreatAStack(int iArray[], int *stackTop)
{
	// function declartions
	void ShowStackFullMessage(void);

	// variable declartions
	int counter;
	char option;

	// code
	counter = 0;
	*stackTop = 0;

	do
	{
		printf("\n ENTER ELEMNET NUMBER %d :", (counter + 1));
		scanf("%d", &iArray[*stackTop]);
		*stackTop = *stackTop + 1;

		if (*stackTop > (MAX_STACK_SIZE - 1))
		{
			ShowStackFullMessage();
			break;
		}

		printf("\n\n");
		printf("DO YOU WANT TO CONTINUE [Y -Yes / N - No] ?");
		option = toupper(getch());
		counter++;

	} while (option == 'Y');
}

void PushAnElementIntoTheStack(int iArray[], int *stackTop)
{
	// function declartions
	void ShowStackFullMessage(void);
	void ShowStackEmptyMessage(void);

	// variable declartions
	char option;

	// code
	if (*stackTop < 0)
		ShowStackEmptyMessage();
	else
	{
		do
		{
			if (*stackTop > (MAX_STACK_SIZE - 1))
			{
				ShowStackFullMessage();
				break;
			}
			printf("\nPUSH IN AN ELEMENT: ");
			scanf("%d", &iArray[*stackTop]);
			*stackTop = *stackTop + 1;
			printf("\n\n");
			printf("DO YOU WANT TO CONITINUE [Y - Yes / N -No] ?");
			option = toupper(getch());

		} while (option == 'Y');
	}
}

void PopElementFromTheStack(int iArray[], int *stackTop)
{
	// function declartions
	void ShowStackEmptyMessage(void);

	// variable declartions
	char option;

	// code
	if (*stackTop < 0)
		ShowStackEmptyMessage();
	else
	{
		do
		{
			if (*stackTop <= 0)
			{
				ShowStackEmptyMessage();
				break;
			}
			printf("\n\nELEMENT %d  HAS BEEN POPPED FROM STACk.\n\n", iArray[*stackTop - 1]);
			*stackTop = *stackTop - 1;
			printf("DO YOU WANT tO CONTINUE [Y -Yes / N -No] ?");
			option = toupper(getch());

		} while (option == 'Y');
	}
}

void DisplayTheStack(int iArray[], int *stackTop)
{

	void ShowStackEmptyMessage(void);

	if (*stackTop < 0)
		ShowStackEmptyMessage();
	else
	{
		printf("\n\n");
		printf("CONTENTS OF THE STACK ARE : \n\n");
		for (int i = 0; i <= (*stackTop - 1); i++)
			printf("INDEX = %d | ELEMeNT = %d\n", i, iArray[i]);
		printf("\n\n");
		printf("PRESS ANY KEY TO CONTINUE.\n");
		getch();
	}
}
void ShowStackFullMessage(void)
{
	printf("\n\n");
	printf("STACK IS FULL.\n");
	printf("PRESS ANY KEY TO PROCEED.\n\n");
	getch();
}

void ShowStackEmptyMessage(void)
{
	printf("\n\n");
	printf("STACK IS EMPTY.\n");
	printf("PRESS ANY KEY TO PROCEED.\n\n");
	getch();
}
