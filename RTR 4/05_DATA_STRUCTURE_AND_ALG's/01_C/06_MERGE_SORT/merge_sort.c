#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_ELEMENTS (1000000)
#define TRUE 1

// function prototype
void input(int *, int);
void output(int *, int, const char *);
void sort(int *, int);
void merge_sort(int *, int, int);
void merge(int *, int, int, int);

int main(int argc, char *argv[])
{
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
	//code
	merge_sort(pa, 0, n - 1);
}

void merge_sort(int *iArray, int iStart, int iEnd)
{
	//variable declartions
	int iMid;

	// code
	if (iStart < iEnd)
	{
		iMid = (iStart + iEnd) / 2;
		merge_sort(iArray, iStart, iMid);
		merge_sort(iArray, iMid + 1, iEnd);
		merge(iArray, iStart, iMid, iEnd);
	}
}

void merge(int *iArray, int iStart, int iMid, int iEnd)
{
	int *ptr_iArray_one = NULL;
	int *ptr_iArray_Two = NULL;
	int iSizeArrayOne;
	int iSizeArrayTwo;
	int i, j, k, cnt;

	iSizeArrayOne = (iMid - iStart) + 1;
	iSizeArrayTwo = iEnd - iMid;

	ptr_iArray_one = (int *)malloc(iSizeArrayOne * sizeof(int));
	if (ptr_iArray_one == NULL)
	{
		printf("Out Of Memmory\n");
		exit(EXIT_FAILURE);
	}
	memset(ptr_iArray_one, 0, iSizeArrayOne * sizeof(int));

	ptr_iArray_Two = (int *)malloc(iSizeArrayTwo * sizeof(int));
	if (ptr_iArray_Two == NULL)
	{
		printf("Out Of Memmory\n");
		exit(EXIT_FAILURE);
	}
	memset(ptr_iArray_Two, 0, iSizeArrayTwo * sizeof(int));

	for (cnt = 0; cnt < iSizeArrayOne; cnt++)
	{
		ptr_iArray_one[cnt] = iArray[iStart + cnt];
	}

	for (cnt = 0; cnt < iSizeArrayOne; cnt++)
	{
		printf("iAraone[%d] = %d", cnt, ptr_iArray_one[cnt]);
	}

	for (cnt = 0; cnt < iSizeArrayTwo; cnt++)
	{
		ptr_iArray_Two[cnt] = iArray[iMid + 1 + cnt];
	}

	i = 0;
	j = 0;
	k = 0;

	while (TRUE)
	{

		if (ptr_iArray_one[i] <= ptr_iArray_Two[j])
		{
			iArray[iStart + k] = ptr_iArray_one[i];
			i++;
			k++;
			if (i == iSizeArrayOne)
			{
				while (j < iSizeArrayTwo)
				{
					iArray[iStart + k] = ptr_iArray_Two[j];
					j++;
					k++;
				}
				break;
			}
			else
			{
				iArray[iStart + k] = ptr_iArray_Two[j];
				j++;
				k++;
				if (j == iSizeArrayTwo)
				{
					while (i < iSizeArrayOne)
					{
						iArray[iStart + k] = ptr_iArray_one[i];
						i++;
						k++;
					}
					break;
				}
			}
		}

		free(ptr_iArray_one);
		ptr_iArray_one = NULL;

		free(ptr_iArray_Two);
		ptr_iArray_Two = NULL;
	}
}
