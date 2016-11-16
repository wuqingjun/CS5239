# CSCI5239_HW10
Qingjun Wu
CSCI 5239 Fall 2016

#### Directions

## Homework 10: GPU Computing 

Create a program that uses CUDA or OpenCL or a compute shader  to speed up calculations.  

Compare the speed of a CPU based implementation with a GPU implementation.


It is NOT acceptable to simply take my examples and adding meaningless

computations to the matrix operations.  You need to attempt a meaningful

computation using the GPU.  In order to see a benefit from the GPU computing,

you will likely have to operate on a large data set.


What to submit:


1) README with discussion of results, brief instructions on how to use the program and a short discussion of the parallel algorithm;


2) One ZIP or TAR archive containing all files you are submitting;


3) Source code, Makefile and any data files I need;


4) Time it took you to complete the assignment.



#### Program Use Instructions

I implemented a function to compute dot product of two vectors, and tested with large data set: 256M. 

Please type below commands to run the executable after you run make: 

1. time ./main 0 -- running this will use CPU to compute the dot product of two vectors;

2. time ./main 1 -- running this will use GPU to compute the dot product of two vectors;

When running with GPU, I was also timing the running kernel part, excluding all the other parts like extracting device info, creating buffer etc. and this part is really fast. 

It will takes only ~0.007ms to finish the computation, compared with the regular CPU approach with a big for-loop, which takes ~3s. But the total time of GPU approach is still higher than CPU, this is due to the facts that there's some overheads happening: query device info, create buffer, copy data from CPU to GPU and than copy it back. 


Here is one of the tests I ran: 

device query and context initialization takes:  12.532000 ms
create memory buffer on device and copy data from cpu to gpu takes:  1354.715000 ms
creating program and kernel takes:  1.488000 ms
opencl kernel execution takes:  0.004000 ms
copy data from GPU back to CPU takes:  2973.686000 ms

In the real world, the program and context will be initialized only once and will be used for ever. With respect to data copy between CPU and GPU, if the data needs much much more computation, for example running a 3D game, then this time is relatively just a small part and can be ignored.   

**Time To Complete Assignment:** 5 hours

