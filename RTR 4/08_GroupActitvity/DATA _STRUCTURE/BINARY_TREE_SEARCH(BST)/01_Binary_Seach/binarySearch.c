// header files
#include <stdio.h>

// entry point function
int main()
{
    // function declartions
    int binarySearch(int[], int);

    // variable declartions
    int arr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    int foundIndex = 0;

    foundIndex = binarySearch(arr, 8);
}

int binarySearch(int arr[], int search)
{
    int i = 0;
    int len = sizeof(arr) / sizeof(int);

    if (search < arr[i])
}
