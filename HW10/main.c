#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
#include <sys/time.h>

#define MAX_SOURCE_SIZE (0x100000)
const int LIST_SIZE = 1024 * 1024 * 256;
FILE *fp = NULL;
char *source_str = NULL;
size_t source_size = 0;

void add_opencl(void) {
    // Create the two input vectors
    int i;
    int *A = (int*)malloc(sizeof(float)*LIST_SIZE);
    int *B = (int*)malloc(sizeof(float)*LIST_SIZE);
    for(i = 0; i < LIST_SIZE; i++) {
        A[i] = i;
        B[i] = LIST_SIZE - i;
    }


	struct timeval stop, start;
	double elapsedTime;

	gettimeofday(&start, NULL);
    // Get platform and device information
    cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;   
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    ret = clGetDeviceIDs( platform_id, CL_DEVICE_TYPE_ALL, 1, 
            &device_id, &ret_num_devices);

    // Create an OpenCL context
    cl_context context = clCreateContext( NULL, 1, &device_id, NULL, NULL, &ret);

    // Create a command queue
    cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

	gettimeofday(&stop, NULL);
    elapsedTime = (stop.tv_sec - start.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (stop.tv_usec - start.tv_usec) / 1000.0;   // us to ms
	printf("device query and context initialization takes:  %f ms\n", elapsedTime);

    // Create memory buffers on the device for each vector 
	gettimeofday(&start, NULL);
    cl_mem a_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, 
            LIST_SIZE * sizeof(float), NULL, &ret);
    cl_mem b_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY,
            LIST_SIZE * sizeof(float), NULL, &ret);
    cl_mem c_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
            LIST_SIZE * sizeof(float), NULL, &ret);

    // Copy the lists A and B to their respective memory buffers
    ret = clEnqueueWriteBuffer(command_queue, a_mem_obj, CL_TRUE, 0,
            LIST_SIZE * sizeof(float), A, 0, NULL, NULL);
    ret = clEnqueueWriteBuffer(command_queue, b_mem_obj, CL_TRUE, 0, 
            LIST_SIZE * sizeof(float), B, 0, NULL, NULL);

	gettimeofday(&stop, NULL);
    elapsedTime = (stop.tv_sec - start.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (stop.tv_usec - start.tv_usec) / 1000.0;   // us to ms
	printf("create memory buffer on device and copy data from cpu to gpu takes:  %f ms\n", elapsedTime);
    // Create a program from the kernel source
	gettimeofday(&start, NULL);
    cl_program program = clCreateProgramWithSource(context, 1, 
            (const char **)&source_str, (const size_t *)&source_size, &ret);

    // Build the program
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

    // Create the OpenCL kernel
    cl_kernel kernel = clCreateKernel(program, "vector_dot", &ret);

	gettimeofday(&stop, NULL);
    elapsedTime = (stop.tv_sec - start.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (stop.tv_usec - start.tv_usec) / 1000.0;   // us to ms
	printf("creating program and kernel takes:  %f ms\n", elapsedTime);
    // Set the arguments of the kernel
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj);
    ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&c_mem_obj);
    
	gettimeofday(&start, NULL);
    // Execute the OpenCL kernel on the list
    size_t global_item_size = LIST_SIZE; // Process the entire lists
    size_t local_item_size = 1024 * 1024 * 16; // Process in groups of 64
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, 
            &global_item_size, &local_item_size, 0, NULL, NULL);

	gettimeofday(&stop, NULL);
    elapsedTime = (stop.tv_sec - start.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (stop.tv_usec - start.tv_usec) / 1000.0;   // us to ms
	printf("opencl kernel execution takes:  %f ms\n", elapsedTime);

    // Read the memory buffer C on the device to the local variable C
	gettimeofday(&start, NULL);
    // Execute the OpenCL kernel on the list
    int *C = (int*)malloc(sizeof(float)*LIST_SIZE);
    ret = clEnqueueReadBuffer(command_queue, c_mem_obj, CL_TRUE, 0, 
            LIST_SIZE * sizeof(float), C, 0, NULL, NULL);
	gettimeofday(&stop, NULL);
    elapsedTime = (stop.tv_sec - start.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (stop.tv_usec - start.tv_usec) / 1000.0;   // us to ms
	printf("copy data from GPU back to CPU takes:  %f ms\n", elapsedTime);

    // Display the result to the screen
	// for(i = 0; i < LIST_SIZE; i++)
	//     printf("%d + %d = %d\n", A[i], B[i], C[i]);

    // Clean up
    ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseMemObject(a_mem_obj);
    ret = clReleaseMemObject(b_mem_obj);
    ret = clReleaseMemObject(c_mem_obj);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);
    free(A);
    free(B);
    free(C);
}

void add_regular(void){

struct timeval stop, start;
gettimeofday(&start, NULL);
    // Create the two input vectors
    int i;
//    const int LIST_SIZE = 1024;
    int *A = (int*)malloc(sizeof(float)*LIST_SIZE);
    int *B = (int*)malloc(sizeof(float)*LIST_SIZE);
    for(i = 0; i < LIST_SIZE; i++) {
        A[i] = i;
        B[i] = LIST_SIZE - i;
    }

    // Read the memory buffer C on the device to the local variable C
    int *C = (int*)malloc(sizeof(float)*LIST_SIZE);

    for(int i = 0; i < LIST_SIZE; i++){
        C[i] = A[i] * B[i];
    }

gettimeofday(&stop, NULL);
double elapsedTime;
    elapsedTime = (stop.tv_sec - start.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (stop.tv_usec - start.tv_usec) / 1000.0;   // us to ms

	printf("%f ms\n", elapsedTime);
    // Display the result to the screen
    //for(i = 0; i < LIST_SIZE; i++)
    //    printf("%d + %d = %d\n", A[i], B[i], C[i]);

    free(A);
    free(B);
    free(C);
}

int main(int argc, char **argv){
	if(argc < 2) {printf("please provider parameter: 1 - running opencl; 0 - running non-opencl");}
    // Load the kernel source code into the array source_str

    fp = fopen("vector_dot_kernel.cl", "r");
    if (!fp) {
        fprintf(stderr, "Failed to load kernel.\n");
        exit(1);
    }
    source_str = (char*)malloc(MAX_SOURCE_SIZE);
    source_size = fread( source_str, 1, MAX_SOURCE_SIZE, fp);
    fclose( fp );
	if(atoi(argv[1]) == 1) 
	{
		add_opencl();
		printf("using opencl \n");
	}
	else 
	{
		add_regular();
		printf("not using opencl \n");
	}
	
	return 0;
}
