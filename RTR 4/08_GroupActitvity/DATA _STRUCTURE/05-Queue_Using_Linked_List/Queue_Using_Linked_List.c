#include <stdio.h> // printf & scanf
#include <stdlib.h>
#include <conio.h> // getch
#include <ctype.h> // toupper

//  --------------------

struct slist
{
    int value;
    struct slist *Next;
};

// Global function declaration
void ShowLinkedListEmptyMessage(void);

int main(void)
{
    // Function prototypes
    struct slist *getnode(void);
    int ShowMainMenu(void);
    void create(struct slist *);

    void InsertAtTheBeginning(void);
    void InsertAtTheEnd(void);
    void InsertInTheMiddle(void);
    void DisplayLinkedList(void);
    void DeleteFromTheBeginning(void);
    void DeleteFromTheEnd(void);
    void DeleteFromTheMiddle(void);

    // variable declarations
    int choice;

    // code
    do
    {
        choice = ShowMainMenu();
        switch (choice)
        {
        case 1:
            InsertAtTheBeginning();
            break;
        case 2:
            InsertAtTheEnd();
            break;
        case 3:
            InsertInTheMiddle();
            break;
        case 4:
            DeleteFromTheBeginning();
            break;
        case 5:
            DeleteFromTheEnd();
            break;
        case 6:
            DeleteFromTheMiddle();
            break;
        case 7:
            DisplayLinkedList();
            break;
        case 8:
            exit(0);
            break;
        }
    } while (choice > 0 && choice < 8);
    return (0);
}

int ShowMainMenu(void)
{
    // variable declarations
    int choice;

    // code
    printf("********************************************\n");
    printf("ENTER THE NUMBER ALONGSIDE YOUR CHOICE : \n\n");
    printf("PRESS 1 - INSERT AN ELEMENT AT THE BEGINNING OF THE LINKED LIST\n");
    printf("PRESS 2 - INSERT AN ELEMENT AT THE END OF THE LINKED LIST\n");
    printf("PRESS 3 - INSERT AN ELEMENT IN THE MIDDLE OF THE LINKED LIST\n");
    printf("PRESS 4 - DELETE AN ELEMENT FROM THE BEGINNING OF THE LINKED LIST\n");
    printf("PRESS 5 - DELETE AN ELEMENT FROM THE END OF THE LINKED LIST\n");
    printf("PRESS 6 - DELETE AN ELEMENT FROM THE MIDDLE OF THE LINKED LIST\n");
    printf("PRESS 7 - DISPLAY THE LINKED LIST\n");
    printf("PRESS 8 (OR ANY KEY OTHER THAN 1 - 7) - TO EXIT\n");
    printf("********************************************\n");
    printf("CHOICE = ");
    scanf("%d", &choice);
    printf("\n\n");
    return (choice);
}

void DisplayLinkedList()
{
    if (Head == NULL)
        ShowLinkedListEmptyMessage();

    else
    {
        struct Node *Temp = Head;

        while (Temp != NULL)
        {
            printf("| %d |-> ", Temp->data);
            Temp = Temp->Next;
        }
        printf("\n\n");
        printf(" PRESS A KEY TO CONTINUE \n");
        getch();
    }
}

void InsertAtTheBeginning()
{
    // local variables
    int iData = 0;

    struct Node *newn;

    // code
    printf("Enter the value for iData :  ");
    scanf("%d", &iData);

    newn = (struct Node *)malloc(sizeof(struct Node));

    newn->data = iData;
    /*
        if (Head == NULL)
        {
            // LL is empty
            Head = newn;
            Head->Next = NULL;
            return;
        }
        */
    newn->Next = Head;
    Head = newn;

    return;
}

void InsertAtTheEnd()
{
    // variables
    struct Node *Temp = NULL;
    struct Node *newn = NULL;
    newn = (struct Node *)malloc(sizeof(struct Node));

    int iValue = 0;

    if (Head == NULL)
    {
        InsertAtTheBeginning();
        return;
    }

    // code
    Temp = Head;

    while (Temp->Next != NULL)
    {
        Temp = Temp->Next;
    }
    printf("Enter the value for iData :  ");
    scanf("%d", &iValue);

    newn->data = iValue;

    Temp->Next = newn;

    newn->Next = NULL;
}

void InsertInTheMiddle()
{
    // variable
    int iPos = 0;
    int i = 1;
    struct Node *middleNode = NULL;
    struct Node *prevMiddle = NULL;
    struct Node *newNode = NULL;
    int iData = 0;

    // code
    printf("Enter Position for New NODE : ");
    scanf("%d", &iPos);

    if (Head == NULL)
    {
        // Error + return
        ShowLinkedListEmptyMessage();
        return;
    }
    else if (iPos < 1 || iPos > (iSize + 1))
    {
        // Error + return
        printf("Error : Invalid Positionn \n");
        return;
    }
    else if (iPos == 1)
    {
        InsertAtTheBeginning();
        return;
    }
    else if (iPos == (iSize + 1))
    {
        InsertAtTheEnd();
        return;
    }
    else
    {
        // Position is between 1 and last node
        middleNode = Head;

        printf("Enter DATA for new NODE: ");
        scanf("%d", &iData);

        newNode = (*struct Node *)malloc(sizeof(struct Node));
        if (newNode == NULL)
        {
            printf("Error : malloc() failed . \n");
            return 0;
        }

        while (i < iPos && middleNode != NULL)
        {
            i++;
            prevMiddle = middleNode; // retai nning address of previous node

            middleNode = middleNode->Next;
        }

        // reached the position
        newNode->Next = middleNode;
        prevMiddle->Next = newNode;

        printf("Inserted Node with DATA : %d ", newNode->data);
    }
}

void DeleteFromTheBeginning()
{
    // variables
    struct Node *Temp;

    // code
    if (Head == NULL)
        ShowLinkedListEmptyMessage();
    else
    {
        Temp = Head;

        Head = Head->Next;
        printf(" Deleted Node Value : %d \n\n", Temp->data);

        free(Temp);
    }
}

void DeleteFromTheEnd()
{

    struct Node *Temp = NULL;

    if (Head == NULL)
    {
        // LL is Empty
        ShowLinkedListEmptyMessage();
        return;
    }
    else if (Head->Next == NULL)
    {
        // Only 1 Element in LL
        DeleteFromTheBeginning();
        return;
    }
    else
    {

        Temp = Head;

        while ((Temp->Next)->Next != NULL)
        {
            Temp = Temp->Next;
        }

        printf(" Deleteted Node value  : %d \n", (Temp->Next)->data);

        free(Temp->Next);

        Temp->Next = NULL;
    }
}

void DeleteFromTheMiddle()
{
    void DeleteFromTheEnd()
    {

        struct Node *Temp = NULL;

        if (Head == NULL)
        {
            // LL is Empty
            ShowLinkedListEmptyMessage();
            return;
        }
        else if (Head->Next == NULL)
        {
            // Only 1 Element in LL
            DeleteFromTheBeginning();
            return;
        }
        else
        {

            Temp = Head;

            while ((Temp->Next)->Next != NULL)
            {
                Temp = Temp->Next;
            }

            printf(" Deleteting Node value  : %d \n", (Temp->Next)->data);

            free(Temp->Next);
            Temp->Next = NULL;

            iSize--;
        }
    }
}

void ShowLinkedListEmptyMessage(void)
{
    printf("\n\n");
    printf("LINKED LIST IS EMPTY.\n");
    printf("PRESS ANY KEY TO PROCEED. \n\n");
    getch();
}

void pop(slist *h, slist *node)
{
    // Variable declartions
    slist *temp = NULL;
    int num;

    // code

    if (h == NULL || h->next == NULL)
        stack_empty();

    do
    {
        temp = h;
        if (temp->next == NULL)
            stack_empty();
        while (temp->next->next != NULL) //
            temp = temp->next;           // temp la pudhach node banava

        // At this point 4, 'temp' is one node before tail node
        node = temp->next;
        num = node->value;
        temp->next = NULL;

        // Now, 'temp' becomes 'tail' node
        free(node);

        printf("VALUE %d IS DELETED FROM THE STACK.\n", num);
        printf("DO YOU WANT TO CONTiNUE ? [ Y / N]\n");
        option = toupper(getch());
    } while (option == 'Y');
}
