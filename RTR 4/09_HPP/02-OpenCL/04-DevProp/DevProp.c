#include <stdio.h>
#include <stdlib.h>

// OpenCL Headers
#include <CL/opencl.h>
#include <CL/cl.h>

#pragma comment(lib, "OpenCL.lib")

int main(void)
{
    // function declarations
    void printOpenCLdeviceProperties(void);

    // code
    printOpenCLdeviceProperties();
}

void printOpenCLdeviceProperties(void)
{
    // code
    printf("OpenCL INFORMATION: \n");
    printf("===============================================================\n");

    cl_int result;
    cl_platform_id ocl_platform_id;
    cl_uint dev_count;
    cl_device_id *ocl_device_ids;
    char oclPlatformInfo[512];

    // get first platform ID
    result = clGetPlatformIDs(1, &ocl_platform_id, NULL);
    if (result != CL_SUCCESS)
    {
        printf("clgetPlatformIDs() Failed\n");
        exit(EXIT_FAILURE);
    }

    // get GPU device count
    result = clGetDeviceIDs(ocl_platform_id, CL_DEVICE_TYPE_GPU, 0, NULL, &dev_count);
    if (result != CL_SUCCESS)
    {
        printf("clgetGetDeviceIDs() Failed\n");
        exit(EXIT_FAILURE);
    }
    else if (dev_count == 0)
    {
        printf("There is No OpenCL Supported Device on This System.\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        // get platform name
        clGetPlatformInfo(ocl_platform_id, CL_PLATFORM_NAME, 500, &oclPlatformInfo, NULL);
        printf("OpenCL Supporting GPU Platform Name : %s\n", oclPlatformInfo);

        // get platform version
        clGetPlatformInfo(ocl_platform_id, CL_PLATFORM_VERSION, 500, &oclPlatformInfo, NULL);
        printf("OpenCL SUPPORTING GPU Platform Version : %s\n", oclPlatformInfo);

        // print suppprting device number
        printf("Total number Of OpenCL Supporting GPU Device/Devices On This System : %d\n", dev_count);

        // allocate memory to hold those device ids
        ocl_device_ids = (cl_device_id *)malloc(sizeof(cl_device_id) * dev_count);

        // get ids into allocate buffer
        clGetDeviceIDs(ocl_platform_id, CL_DEVICE_TYPE_GPU, dev_count, ocl_device_ids, NULL);

        char ocl_dev_prop[1024];
        int i;

        for (i = 0; i < (int)dev_count; i++)
        {
            printf("\n");
            printf("***** GPU DEVICE GENERAL INFORMATION ******\n");
            printf("============================================\n");

            printf("GPU DEVICE NUMBER                           :%d\n", i);

            clGetDeviceInfo(ocl_device_ids[i], CL_DEVICE_NAME, sizeof(ocl_dev_prop), &ocl_dev_prop, NULL);
            printf("GPU DEVICE NAME                             :%s\n", ocl_dev_prop);

            clGetDeviceInfo(ocl_device_ids[i], CL_DEVICE_VENDOR, sizeof(ocl_dev_prop), &ocl_dev_prop, NULL);
            printf("GPU DEVICE VENDOR                           :%s\n", ocl_dev_prop);

            clGetDeviceInfo(ocl_device_ids[i], CL_DRIVER_VERSION, sizeof(ocl_dev_prop), &ocl_dev_prop, NULL);
            printf("GPU DRIVER VERSION                           :%s\n", ocl_dev_prop);

            clGetDeviceInfo(ocl_device_ids[i], CL_DEVICE_VERSION, sizeof(ocl_dev_prop), &ocl_dev_prop, NULL);
            printf("GPU DEVICE OpenCL VERSION                    :%s\n", ocl_dev_prop);

            cl_uint clock_frequency;
            clGetDeviceInfo(ocl_device_ids[i], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(clock_frequency), &clock_frequency, NULL);
            printf("GPU DEVICE Clock Rate                        :%u\n", clock_frequency);

            printf("\n");

            printf("******************* GPU DEVICE MEMORY INFORMATION ****************\n");
            printf("==================================================================\n");
            cl_ulong mem_size;
            clGetDeviceInfo(ocl_device_ids[i], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(mem_size), &mem_size, NULL);
            printf("GPU DEVICE GLOBAL MEMORY                    :%llu Bytes\n", (unsigned long long)mem_size);

            cl_device_local_mem_type local_mem_type;

            clGetDeviceInfo(ocl_device_ids[i], CL_DEVICE_LOCAL_MEM_SIZE, sizeof(mem_size), &mem_size, NULL);
            printf("GPU DEVICE LOCAL MEMORY                     :%llu Bytes\n", (unsigned long long)mem_size);

            clGetDeviceInfo(ocl_device_ids[i], CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, sizeof(mem_size), &mem_size, NULL);
            printf("GPU DEVICE CONSTANT BUFFER SIZE             :%llu Bytes\n", (unsigned long long)mem_size);

            printf("\n");
            printf("************ GPU DEVICE COMPUTE INFORMATION ************");
            printf("=================================================================\n");
            cl_uint compute_units;
            clGetDeviceInfo(ocl_device_ids[i], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(compute_units), &compute_units, NULL);
            printf("GPU DEVICE NUMBER OF PARALLEL PROCCESSORS CORES : %u \n", compute_units);

            size_t workgroup_size;
            clGetDeviceInfo(ocl_device_ids[i], CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(workgroup_size), &workgroup_size, NULL);
            printf("GPU DEVICE WORK GROUP SIZE                      : %u \n", (unsigned int)workgroup_size);

            size_t workitem_dims;
            clGetDeviceInfo(ocl_device_ids[i], CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(workitem_dims), &workitem_dims, NULL);
            printf("GPU DEVICE WORK ITEM DIMENSIONS                 : %d \n", (unsigned int)workitem_dims);

            size_t workitem_size[3];
            clGetDeviceInfo(ocl_device_ids[i], CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(workitem_size), &workitem_size, NULL);
            printf("GPU DEVICE WORK GROUP SIZE                      : %u /%u /%u \n", (unsigned int)workitem_size[0], (unsigned int)workitem_size[1], (unsigned int)workitem_size[2]);
        }
        free(ocl_device_ids);
    }
}
