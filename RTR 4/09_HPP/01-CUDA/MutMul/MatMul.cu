// header files
// standard headers
#include <stdio.h>

// cuda headers
#include <cuda.h>
#include "helper_timer.h"
// macros
#define BLOCK_WIDTH 32

// global variables
int *hostA = NULL;
int *hostB = NULL;
int *hostC = NULL;
int *gold = NULL;

int *deviceA = NULL;
int *deviceB = NULL;
int *deviceC = NULL;

float timeOnGPU = 0.0f;
float timeOnCPU = 0.0f;

// cude kernel
__global__ void matMulGPU(int *A, int *B, int *C, int numARows, int numAColumns, int numBColumns, int numCColumns)
{
    // code
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int column = blockIdx.x * blockDim.x + threadIdx.x;

    // code
    if ((row < numARows) && (column < numBColumns))
    {
        int value = 0.0f;
        for (int k = 0.0f; k < numAColumns; k++)
        {
            int a = A[row * numAColumns + k];
            int b = B[k * numBColumns + column];
            value += a * b;
        }
        C[row * numCColumns + column] = value;
    }
}

// entry point function
int main(void)
{
    // function prototype
    void InitA(int *, int, int);
    void InitB(int *, int, int);
    void matMulCPU(int *, int *, int *, int, int, int, int);
    void cleanup(void);

    // variable declartions
    int numARows = BLOCK_WIDTH;
    int numAColumns = BLOCK_WIDTH;
    int numBRows = BLOCK_WIDTH;
    int numBColumns = BLOCK_WIDTH;

    int numCRows = numARows;
    int numCColumns = numBColumns;

    int numGoldRows = numARows;
    int numGoldColumns = numBColumns;

    int sizeA = numARows * numAColumns * sizeof(int);
    int sizeB = numBRows * numBColumns * sizeof(int);
    int sizeC = numCRows * numCColumns * sizeof(int);

    int sizeGold = numGoldRows * numBColumns * sizeof(int);

    cudaError_t result = cudaSuccess;

    // code
    //  host memory allocation
    hostA = (int *)malloc(sizeA);
    if (hostA == NULL)
    {
        printf("Host Memory Allocation is Failed for hostA array. \n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    hostB = (int *)malloc(sizeB);
    if (hostB == NULL)
    {
        printf("Host Memory Allocation is Failed for hostB array. \n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    hostC = (int *)malloc(sizeC);
    if (hostC == NULL)
    {
        printf("Host Memory Allocation is Failed for hostC array. \n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    gold = (int *)malloc(sizeGold);
    if (gold == NULL)
    {
        printf("Host Memory Allocation is Failed for gold array. \n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    // printing matrix dimension and sizes
    printf("The Dimension Of Matrix 'hostA' Are : %d x %d\n", numARows, numAColumns);
    printf("The Dimension Of Matrix 'hostB' Are : %d x %d\n", numBRows, numBColumns);
    printf("The Dimension Of Matrix 'hostC' Are : %d x %d\n", numCRows, numCColumns);

    printf("The Dimension Of Matrix 'gold' Are : %d x %d\n", numGoldRows, numGoldColumns);

    printf("Size Of Matrix hostA = %d\n", sizeA);
    printf("Size Of Matrix hostB = %d\n", sizeB);
    printf("Size Of Matrix hostC = %d\n", sizeC);

    printf("Size Of Matrix Gold = %d\n", sizeGold);

    // fill source matrices
    InitA(hostA, numARows, numAColumns);
    InitB(hostB, numBRows, numBColumns);

    // device memory allocation

    // device memory allocation
    result = cudaMalloc((void **)&deviceA, sizeA);
    if (result != cudaSuccess)
    {
        printf("Device memory allocation is failed for deviceA matrix.\n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    result = cudaMalloc((void **)&deviceB, sizeB);
    if (result != cudaSuccess)
    {
        printf("Device memory allocation is failed for deviceB matrix.\n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    result = cudaMalloc((void **)&deviceC, sizeC);
    if (result != cudaSuccess)
    {
        printf("Device memory allocation is failed for deviceC array.\n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    // copy data from host arrays into device arrays
    result = cudaMemcpy(deviceA, hostA, sizeA, cudaMemcpyHostToDevice);
    if (result != cudaSuccess)
    {
        printf("Host to Device Data is failed for deviceA matrix.\n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    result = cudaMemcpy(deviceB, hostB, sizeB, cudaMemcpyHostToDevice);
    if (result != cudaSuccess)
    {
        printf("Host to Device Data is failed for deviceA array.\n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    // cuda kernel  configuration
    dim3 dimGrid = dim3(ceil((int)numBColumns / (int)BLOCK_WIDTH), ceil((int)numARows / (int)BLOCK_WIDTH), 1);
    dim3 dimBlock = dim3(BLOCK_WIDTH, BLOCK_WIDTH, 1);

    // CUDA Kernel for vector Multiplication
    StopWatchInterface *timer = NULL;
    sdkCreateTimer(&timer);
    sdkStartTimer(&timer);

    matMulGPU<<<dimGrid, dimBlock>>>(deviceA, deviceB, deviceC, numARows, numAColumns, numBColumns, numCColumns);

    sdkStopTimer(&timer);
    timeOnGPU = sdkGetTimerValue(&timer);
    sdkDeleteTimer(&timer);
    timer = NULL;

    // copy data from device array into host array
    result = cudaMemcpy(hostC, deviceC, sizeC, cudaMemcpyDeviceToHost);
    if (result != cudaSuccess)
    {
        printf("Device to host Dta copy is failed for hostC array.\n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    // vector addition on host
    matMulCPU(hostA, hostB, gold, numARows, numAColumns, numBColumns, numCColumns);

    // comparison
    const float epsilon = 0.0000001f;

    int breakValue = -1;
    bool bAccuracy = true;
    for (int i = 0; i < numCRows * numCColumns; i++)
    {
        float val1 = gold[i];
        float val2 = hostC[i];

        if (fabs(val1 - val2) > epsilon)
        {
            bAccuracy = false;
            breakValue = i;
            break;
        }
    }

    char str[128];
    if (bAccuracy == false)
        sprintf(str, "Comparison of CPU and GPU Matrix Multiplication is not within accuracy of 0.000001 at array index %d", breakValue);
    else
        sprintf(str, "Comparison of CPU and GPU Matrix Multiplication is within accuracy of 0.000001");

    // Output
    printf("Time taken for Matrix  Multiplication On CPU = %.6f\n", timeOnCPU);
    printf("Time taken for Matrix  Multiplication On GPU = %.6f\n", timeOnGPU);
    printf("%s\n", str);

    // cleanupp
    cleanup();

    return (0);
}

void fillFloatArrayRandomNumber(float *arr, int len)
{
    // code
    const float fscale = 1.0f / (float)RAND_MAX;
    for (int i = 0; i < len; i++)
    {
        arr[i] = fscale * rand();
    }
}

void InitA(int *data, int row, int col)
{
    int num = 1;
    // code
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            *(data + i * col + j) = num;
            num++;
        }
    }
}

void InitB(int *data, int row, int col)
{
    int num = BLOCK_WIDTH;
    // code
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            *(data + i * col + j) = num;
            num--;
        }
    }
}

void matMulCPU(int *A, int *B, int *C, int numARows, int numAColumns, int numBColumns, int numCColumns)
{
    // code
    StopWatchInterface *timer = NULL;
    sdkCreateTimer(&timer);
    sdkStartTimer(&timer);

    for (int i = 0; i < numARows; i++)
    {
        for (int j = 0; j < numBColumns; j++)
        {
            int value = 0.0f;
            for (int k = 0; k < numAColumns; k++)
            {
                int a = A[i * numAColumns + k];
                int b = B[k * numBColumns + j];
                value += a * b;
            }
            C[i * numCColumns + j] = value;
        }
    }

    sdkStopTimer(&timer);
    timeOnCPU = sdkGetTimerValue(&timer);
    sdkDeleteTimer(&timer);
    timer = NULL;
}

void cleanup(void)
{
    // code
    if (deviceC)
    {
        cudaFree(deviceC);
        deviceC = NULL;
    }

    if (deviceB)
    {
        cudaFree(deviceB);
        deviceB = NULL;
    }

    if (deviceA)
    {
        cudaFree(deviceA);
        deviceA = NULL;
    }

    if (gold)
    {
        cudaFree(gold);
        gold = NULL;
    }

    if (hostC)
    {
        free(hostC);
        hostC = NULL;
    }

    if (hostB)
    {
        free(hostB);
        hostB = NULL;
    }

    if (hostA)
    {
        free(hostA);
        hostA = NULL;
    }
}
