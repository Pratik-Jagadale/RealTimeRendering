#include "cuda.h"

#define rnd(x) (x * rand() / RAND_MAX)
#define SPHERE 20
#define INF 2e30f

struct Sphere
{
    float r, g, b;
    float radius;
    float x, y, z;
    __device__ float hit(float ox, float oy, float *n)
    {
        float dx = ox - x;
        float dy = oy - y;
        if (dx * dx + dy * dy < radius * radius)
        {
            float dz = sqrt(radius * radius - dx * dx - dy * dy);
            *n = dz / sqrt(radius * radius);
            return dz + z;
        }
        return -INF;
    }
};

int main(void)
{
    // capture the start time
    cudaEvent_t start, stop;
    HANDLE_ERROR(cudaEventCreate(&start));
    HANDLE_ERROR(cudaEventCreate(&stop));
    HANDLE_ERROR(cudaEventRecord(start, 0));

    CPUBitmap bitmap(DIM, DIM);
}
