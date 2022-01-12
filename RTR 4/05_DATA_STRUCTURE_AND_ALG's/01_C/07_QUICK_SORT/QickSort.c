#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ELEMENTS (100000)

int partition(int *iArray, int iStart, int iEnd);
void QuickSort(int *iArray, int iStart, int iEnd);
void swap(int *a, int *b);

int main(int argc, char *argv[])
{
    // function prototype
    void input(int *, int);
    void output(int *, int, const char *);
    void sort(int *, int);

    // variable declartions
    int *pa = NULL;
    int n = 0;

    // Code
    if (argc != 2)
    {
        printf("Usage Errror %s nr_Elements\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    n = atoi(argv[1]);

    if (n <= 0)
    {
        printf("Bad Size of Array ...\n");
        exit(EXIT_FAILURE);
    }

    pa = (int *)malloc(n * sizeof(int));
    if (pa == NULL)
    {
        printf("Out Of Memmory\n");
        exit(EXIT_FAILURE);
    }

    // Input
    input(pa, n);

    if (n <= 100)
        output(pa, n, "Before Sort");

    time_t start_time = time(0);
    sort(pa, n);
    time_t end_time = time(0);
    output(pa, n, "After Sort:");
    printf("end_time - start_time =%llu\n", end_time - start_time);
    free(pa);
    pa = NULL;

    return (EXIT_SUCCESS);
}

void input(int *pa, int n)
{
    // variable declartions
    int i;

    // Code
    srand(time(0));
    for (i = 0; i < n; i++)
        pa[i] = rand() % MAX_ELEMENTS;
}

void output(int *pa, int n, const char *msg)
{
    // variable declartions
    int i;

    //code
    if (msg)
        puts(msg);
    for (i = 0; i < n; i++)
        printf("a[%d]:[%d]\n", i, pa[i]);
}

void sort(int *pa, int n)
{
    //function prototype
    void QuickSort(int *, int, int);

    //code
    QuickSort(pa, 0, n - 1);
}

void swap(int *a, int *b)
{
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

void QuickSort(int *iArray, int iStart, int iEnd)
{
    int mid;
    if (iStart < iEnd)
    {
        mid = partition(iArray, iStart, iEnd);
        QuickSort(iArray, iStart, mid - 1);
        QuickSort(iArray, mid + 1, iEnd);
    }
}

int partition(int *iArray, int iStart, int iEnd)
{
    int pivot, i, j, temp;
    pivot = iArray[iEnd]; // last element as a pivot element

    i = iStart;

    for (j = iStart; j < iEnd; j++)
    {
        if (i == j)
            continue;
        if (iArray[j] <= pivot)
        {
            swap(&iArray[i], &iArray[j]);
            i++;
        }
    }
    swap(&iArray[i], &iArray[iEnd]);
    /*temp = iArray[i];
    iArray[i] = pivot;
    iArray[iEnd] = temp;
    */
    return (i);
}
