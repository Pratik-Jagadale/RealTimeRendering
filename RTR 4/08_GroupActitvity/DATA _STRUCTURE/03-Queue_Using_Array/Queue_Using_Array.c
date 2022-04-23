// Array.c
#include <stdio.h> // for printf(), scanf()
#include <conio.h> // for getch()
#include <stdlib.h>
#include <ctype.h> // for toupper

#define MAX_QUEUE_SIZE 20

// function declartions
void ShowQueueFullMessage(void);
void ShowQueueEmptyMessage(void);

int main(int argc, char *argv[])
{
	// function prototypes
	int ShowMainMenu(void);
	void CreatAQueue(int[], int *, int *);
	void PushAnElementIntoTheQueue(int[], int *, int *);
	void PopElementFromTheQueue(int[], int *, int *);
	void DisplayTheQueue(int[], int *, int *);

	// varibale declarrtions
	int queueBeginnigIndex = -1;
	int queueEndIndex = -1;
	int iArray[MAX_QUEUE_SIZE];
	int choice;

	// code
	do
	{
		choice = ShowMainMenu();
		switch (choice)
		{
		case 1:
			CreatAQueue(iArray, &queueBeginnigIndex, &queueEndIndex);
			break;

		case 2:
			PushAnElementIntoTheQueue(iArray, &queueBeginnigIndex, &queueEndIndex);
			break;

		case 3:
			PopElementFromTheQueue(iArray, &queueBeginnigIndex, &queueEndIndex);
			break;

		case 4:
			DisplayTheQueue(iArray, &queueBeginnigIndex, &queueEndIndex);
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
	printf("PRESS 1 - To CREATE A QUEUE\n");
	printf("PRESS 2 - TO PUSH AN ELEMENT INTO THE QUEUE\n");
	printf("PRESS 3 - To POP AN ELEMNET FROM THE QUEUE\n");
	printf("PRESS 4 - TO DISPLAY THE QUEUE\n");
	printf("PRESS 5 - (OR ANY KEY OTHER THAN 1-5) - To EXIT\n");
	printf("CHOICE  : ");
	scanf("%d", &choice);
	printf("\n\n");
	return (choice);
}

void CreatAQueue(int iArray[], int *queueBegining, int *queueEnd)
{
	// variable declartions
	int counter;
	char option;

	// code
	counter = 0;
	*queueEnd = 0;
	*queueBegining = 0;

	do
	{
		printf("\n ENTER ELEMENET NUMBER %d :", (counter + 1));
		scanf("%d", &iArray[*queueEnd]);
		*queueEnd = *queueEnd + 1;

		if (*queueEnd > (MAX_QUEUE_SIZE - 1))
		{
			ShowQueueFullMessage();
			break;
		}

		printf("\n\n");
		printf("DO YOU WANT TO CONTINUE [Y -Yes / N - No] ?");
		option = toupper(getch());
		counter++;

	} while (option == 'Y');
}

void PushAnElementIntoTheQueue(int iArray[], int *queueBegining, int *queueEnd)
{
	// variable declartions
	char option;

	// code
	if (*queueBegining == -1 || *queueEnd == -1)
		ShowQueueEmptyMessage();
	else
	{
		do
		{
			if (*queueEnd > (MAX_QUEUE_SIZE - 1))
			{
				ShowQueueFullMessage();
				break;
			}
			printf("\nPUSH IN AN ELEMENT: ");
			scanf("%d", &iArray[*queueEnd]);
			*queueEnd = *queueEnd + 1;
			printf("\n\n");
			printf("DO YOU WANT TO CONITINUE [Y - Yes / N -No] ?");
			option = toupper(getch());

		} while (option == 'Y');
	}
}

void PopElementFromTheQueue(int iArray[], int *queueBegining, int *queueEnd)
{
	// variable declartions
	char option;

	int index = 0, num;

	// code
	if (*queueBegining == 1 || *queueEnd == -1)
		ShowQueueEmptyMessage();
	else
	{
		index = *queueBegining;
		do
		{

			if (index == *queueEnd)
			{
				ShowQueueEmptyMessage();
				break;
			}

			num = iArray[index];
			index = index + 1;

			printf("\n\nELEMENT %d  HAS BEEN POPPED FROM QUEUE.\n\n", num);
			*queueBegining = index;
			printf("DO YOU WANT tO CONTINUE [Y -Yes / N -No] ?");
			option = toupper(getch());

		} while (option == 'Y');
	}
}

void DisplayTheQueue(int iArray[], int *queueBegining, int *queueEnd)
{

	void ShowQueueEmptyMessage(void);

	if (*queueEnd < 0)
		ShowQueueEmptyMessage();
	else
	{
		printf("\n\n");
		printf("CONTENTS OF THE QUEUE ARE : \n\n");
		for (int i = *queueBegining; i <= (*queueEnd - 1); i++)
			printf("INDEX = %d | ELEMeNT = %d\n", i, iArray[i]);
		printf("\n\n");
		printf("PRESS ANY KEY TO CONTINUE.\n");
		getch();
	}
}
void ShowQueueFullMessage(void)
{
	printf("\n\n");
	printf("QUEUE IS FULL.\n");
	printf("PRESS ANY KEY TO PROCEED.\n\n");
	getch();
}

void ShowQueueEmptyMessage(void)
{
	printf("\n\n");
	printf("QUEUE IS EMPTY.\n");
	printf("PRESS ANY KEY TO PROCEED.\n\n");
	getch();
}
