#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#define MAX_ARRAY_SIZE 20

int numberOfElements = 0;

int main(int argc, char *argv[])
{
    // function prototype
    int ShowMainMenu(void);
    void CreateAnArray(int[], int *, int *);
    void AddElementAtTheBeginningOfArray(int[], int *);
    void DeleteElementFromBeginningOfArray(int[], int *);

    void AddElementAtTheEndOfArray(int[], int *);
    void DeleteElementAtTheEndOfArray(int[], int *);

    void DisplayAnArray(int[], int *, int *);

    // variables
    int choice;
    int firstIndex = -1;
    int lastIndex = -1;
    int iArray[MAX_ARRAY_SIZE];

    // code
    do
    {
        choice = ShowMainMenu();

        switch (choice)
        {
        case 1:
            CreateAnArray(iArray, &firstIndex, &lastIndex);
            break;
        case 2:
            AddElementAtTheBeginningOfArray(iArray, &firstIndex);
            break;
        case 3:
            AddElementAtTheEndOfArray(iArray, &firstIndex);
            break;
        case 4:
            DeleteElementFronBeginningOfTheArray(iArray, &lastIndex);
            break;
        case 6:
            DisplayAnArray(iArray, &firstIndex, &lastIndex);
            break;
        case 7:
            exit(0);
            break;
        default:

            break;
        }

    } while (choice > 0 && choice < 8);

    return 0;
}

//-----------DEFINITIONS--------------------

int ShowMainMenu()
{
    // variables
    int choice;

    // code
    printf("**************************************\n");
    printf("ENTER THE NUMBER ALONGSIDE YOUR CHOICE : \n\n");
    printf("PRESS 1 - To Create An Array .\n");
    printf("PRESS 2 - To Add a value at Beginning of the Array .\n");
    printf("PRESS 3 - To Add a value at End of the Array .\n");
    printf("PRESS 4 - To Delete a value from the Beginning of the Array\n");
    printf("PRESS 5 - To Delete a value from the End of the Array .\n");
    printf("PRESS 6 - To Display the Array .\n");
    printf("PRESS 7 - (Or Any Key other than 1 to 6) - To Exit.\n");
    printf("**************************************\n");

    printf("CHOICE = ");
    scanf("%d", &choice);
    printf("\n\n");
    return (choice);
}

//------------------------------------------------------------

void CreateAnArray(int iArray[], int *firstElementIndex, int *lastElementIndex)
{
    // variable declarations
    int position;
    int i;
    int counter;

    // code

    printf("Enter the position of 1st element of your Array :");
    scanf("%d", &position);

    printf("\nEnter the No of Elements you want in the Array : \n");
    printf("Number of Elements Must Not exceed %d .\n ", (MAX_ARRAY_SIZE - position));
    scanf("%d", &numberOfElements);

    printf("\n\n");

    counter = 1;
    for (i = position; i <= position + (numberOfElements - 1); i++)
    {
        printf("Enter Elements Number %d : ", counter);
        scanf("%d", &iArray[i]);
        counter++;
    }

    *firstElementIndex = position;
    *lastElementIndex = position + (numberOfElements);

    printf("\n\n");
    printf("Entry of Elements Inside Array is Complete.\n");
    printf("Enter Any key to Proceed.\n");

    getch();
}
//---------------------------------------------------------------
void AddElementAtTheBeginningOfArray(int iArray[], int *firstElementIndex)
{
    // variable declartions
    char choice;

    // code
    if (*firstElementIndex == -1)
    {
        printf("\n\n");
        printf("The Array is Empty.\n");
        printf("PLEASE FIRST CREATE AN ARRAY BY SELECTION OPTION 1 FROM THE GIVEN MENU.\n");
    }
    else if (*firstElementIndex == 0)
    {
        printf("\n\n");
        printf("0TH POSITION OF THE ARRAY IS ALREADY OCCUPIED.\n");
        printf("HENCE, CANNOT ADD ANY ELEMENTS AT THE BEGINNING OF THE ARRAY.\n ");
        printf("TRY OTHER OPTIONS, OR RECRREATE THE ARRAY BY SELECTION OPTION  1 FROM THE GIVEN MENU.\n ");
    }
    else
    {
        do
        {
            *firstElementIndex = *firstElementIndex - 1;
            if (*firstElementIndex == -1)
            {
                printf("\n\n");
                printf("0TH POSITON OF THE ARRAY  IS ALREADY OCCUPIED. \n");
                printf("HENCE, CANNOT ADD ANY ELEMENTS AT THE BEGGINING OF THE ARRAY.\n");
                printf("TRY OTHER OPTIONS, OR RECREATE THE ARRAY BY SLECTION OPTIoN 1 FROM THE GIVEN MENU.\n");
                *firstElementIndex = 0;
            }
            printf("\n\n");
            printf("EMTER AN ELEMENT = ");
            scanf("%d", &iArray[*firstElementIndex]);
            numberOfElements++;
            printf("ELEMENT %d HAS BEEN ADDED TO THE BEGGINING OF THE ARRAY.\n", iArray[*firstElementIndex]);
            printf("DO YOU WANT TO CONTINUE ADDING ELEMNETS TO THE BEGINNING OF THE ARRAY[Y - YES ,/ N - NO]\n");
            printf("CHOICE = ");
            choice = toupper(getch());
        } while (choice == 'Y');
    }
    printf("\n\n");
    printf("PRESS ANY KEY TO CONTINUE.\n");
    getch();
}

void AddElementAtTheEndOfArray(int iArray[], int *lastElementIndex)
{
    // variable declarations
    char choice;

    // code
    if (*lastElementIndex == -1)
    {
        printf("\n\n");
        printf("\nTHE ARRAY IS EMPTY!! \n");
        printf("Please First Create an Array by selecting option 1 From the Menu. \n");
    }
    else if (*lastElementIndex == (MAX_ARRAY_SIZE - 1))
    {
        printf("\n\n");
        printf("Last Avaiable Position of the array is Already Occupied . \n");
        printf("Hence, Cannot Add any elements At the End of the Array. \n");
        printf("Try other options, OR Recreate The Array By Selecting an option from MENU .\n");
    }
    else
    {
        do
        {
            *lastElementIndex = *lastElementIndex + 1;
            if (*lastElementIndex == MAX_ARRAY_SIZE)
            {
                printf("\n\n");
                printf("Last Available option of the Array is Already Occupied . \n");
                printf("Hence, cannot Add any elements At the End of The Array . \n");
                printf("Try other options, OR Recreate the Array by Selecting option #1  From Menu . \n");
                *lastElementIndex = (MAX_ARRAY_SIZE - 1);
                break;
            }
            printf("\n-------------\n\n");
            printf("Enter An Element : ");
            scanf("%d", &iArray[*lastElementIndex]);
            numberOfElements++;
            printf("Element %d Has been Added to the end of the Array .\n", iArray[*lastElementIndex]);
            printf("Do you want to continue Adding elements to the End of the Array [Y- YES / N- NO] . \n");
            printf(" CHOICE = ");
            choice = toupper(getch());
        } while (choice == 'Y');
    }
    printf("\n\n");
    printf(" PRESS ANY KEY TO CONTINUE . \n");
    getch();
}

void DeleteElementFromBeginningOfTheArray(int iArray[], int *firstElementIndex)
{
    // variable declarations
    char choice;

    // code
    if (*firstElementIndex == -1)
    {
        printf("\n\n");
        printf("The Array is Empty.\n");
        printf("Please First Create An Array By Selecting Option 1 From the Given Menu .\n");
    }
    else
    {

        do
        {
            if (numberOfElements == 0)
            {
                printf("All the elements of Array Have been deleted. \n");
                printf("The Array is Empty. \n");
                printf("Please First Create An Array By Selecting Option 1 from the Given Menu. \n");
                break;
            }
            printf("\n\n");
            printf("The Element Deleted From The Beginning of The Array is %d  . \n", iArray[*firstElementIndex]);
            *firstElementIndex = *firstElementIndex + 1;
            numberOfElements--;
            printf("Do You Want to continue deleting Elements From the beginning of the Array [Y - YES] / [N- NO]. \n");
            printf("CHOICE : ");
            choice = toupper(getch());
        } while (choice == 'Y');

        printf("\n\n");
        printf("PRESS ANY KEY TO CONTINUE. \n");
        getch();
    }
}

void DeleteElementAtTheEndOfArray(int iArray[], int *lastElementIndex)
{
    // variables
    char choice;

    // code
    if (*lastElementIndex == -1)
    {
        printf("\n\n");
        printf("The Array is EMPTY . \n");
        printf("Please First Create An Array by Selecting Option # 1 from given MENU . \n");
    }
    else
    {
        do
        {
            if (numberOfElements == 0)
            {
                printf("All elements of Array have been DELETED. \n");
                printf("The Array is Empty \n");
                printf("Please First Create An Array by Selecting Option #1 from given Menu . \n");
                break;
            }
            printf("\n\n");
            printf("The element deleted from the End of The Array is  %d  \n", iArray[*lastElementIndex]);
            *lastElementIndex = *lastElementIndex - 1;
            numberOfElements--;
            printf("Do you want to Continue Deleting Elements From The END OF ARRAY ? [ Y- YES / N - NO]  \n");
            printf(" CHOICE =  ");
            choice = toupper(getch());
        } while (choice == 'Y');
    }
    printf("\n\n");
    printf(" PRESS ANY KEY TO CONTINUE . \n");
    getch();
}

void DisplayAnArray(int iArray[], int *firstElementIndex, int *lastElementIndex)
{
    // variables

    // code
    if (*firstElementIndex == -1 || *lastElementIndex == -1 || numberOfElements == 0)
    {
        printf("\n\n");
        printf("The Array is Empty.\n");
        printf("To use this facility,Please first create an Array by Selecting option 1 from the given Menu");
    }
    else
    {
        printf("\n\n");
        printf("Contents of the Array are : \n\n");
        for (int i = *firstElementIndex; i <= *lastElementIndex - 1; i++)
        {
            printf("Index = %d | Element = %d \n ", i, iArray[i]);
        }
    }
    printf("\n\n");
    printf("Press Any Key To Continue.\n");
    getch();
}