#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#define HASH(n, k) ((n) % (k))

int main(int argc, char *argv[])
{
    int i;
    int *ar_bucket = NULL;
    int nr_elements, devisible;
    int remiander;

    assert(argc == 3);
    nr_elements = atoi(argv[1]);
    devisible = atoi(argv[2]);
    int bucket_size = devisible;

    ar_bucket = (int *)malloc(bucket_size * sizeof(int));
    memset(ar_bucket, 0, bucket_size * sizeof(int));

    srand(time(0));

    for (i = 0; i < nr_elements; i++)
    {
        remiander = HASH(rand(), devisible);
        ar_bucket[remiander] = ar_bucket[remiander] + 1;
    }

    for (i = 0; i < bucket_size; i++)
        printf("Number of Elements in the 'ar_bucket[%d]' = [%d]\n", i, ar_bucket[i]);

    free(ar_bucket);
    ar_bucket = NULL;
    return (0);
}