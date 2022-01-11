#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ELEMENTS (100000)

int main(int argc, char* argv[])
{
	// function prototype
	void input(int*, int);
	void output(int* , int , const char* );
	void  sort(int*, int);

	// variable declartions
	int*pa = NULL;
	int n = 0;

	// Code
	if(argc != 2){
		printf("Usage Errror %s nr_Elements\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	n = atoi(argv[1]);

	if(n <= 0)
	{
		printf("Bad Size of Array ...\n");
		exit(EXIT_FAILURE);
	}

	pa= (int*)malloc(n * sizeof(int));
	if(pa == NULL)
	{
		printf("Out Of Memmory\n");
		exit(EXIT_FAILURE);
	}

	// Input
	input(pa,n);

	if(n <= 100)
		output(pa,n, "Before Sort");

	time_t start_time = time(0);
	sort(pa, n);
	time_t end_time = time (0);
	output(pa, n, "After Sort:");
	printf("end_time - start_time =%llu\n",end_time - start_time);
	free(pa);
	pa =NULL;

	return (EXIT_SUCCESS);
}

void input(int* pa, int n)
{
	// variable declartions
	int i;

	// Code
	srand(time(0));
	for(i = 0; i < n ; i++)
			pa[i] = rand() % MAX_ELEMENTS;
}

void output(int* pa, int n, const char* msg)
{
	// variable declartions
	int i;

	//code
	if(msg)
		puts(msg);
	for(i = 0 ; i < n ; i++)
		printf("a[%d]:[%d]\n",i,pa[i]);
}

void sort(int* pa, int n)
{
	//function prototype
	void insertion_sort(int* , int);

	//code
	insertion_sort(pa,n);
}


void insertion_sort(int* pa, int n)
{
	int i, j , key;

	for(j = 0 ; j < n ; j++)
	{
		key = pa[j];
		i = j - 1 ;
		while(i > -1 && pa[i] > key)
		{
			pa[i+1] = pa[i];
			i = i - 1;
		}
		pa[i + 1] = key;
	}
}
