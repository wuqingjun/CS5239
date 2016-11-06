# CSCI5239_HW6
Image Processing
Qingjun Wu
CSCI 5239 Fall 2016

#### Directions

## Homework 7: Image Processing
Create a program that performs image processing using GLSL.

This is a very domain specific assignment, but the applications are very wide

ranging.  Some examples are

1)  Spatial transformations such as sharpening, blurring, edge detection, etc;

2)  Color transformations;

3)  Resampling transformations such as averaging, anti-aliasing;

4)  Multi-image operations;

5)  Special effects.

Anybody who can write a shader that will sharpen an image so you can read the
license plate from the reflection on somebody's sunglasses gets an automatic A+
in this class.

Hint:  If you need to write to a buffer that is a different size than the
window, Examples 11 and 12 shows how to use a framebuffer objects.

What to submit:

1) README with discussion of results and brief instructions on how to use the program;


2) One ZIP or TAR archive containing all files you are submitting;


3) Source code (shader and prorgam), Makefile and any data files I need;


4) Time it took you to complete the assignment.


#### Program Use Instructions

The solusiton is based on ex13. I removed all the other items from the dropdown list and kept only original image and transtion.

I am following this online documentation to implement transform a colorful image to black-white one. A threshold is used in the algorithm, 

if the Euclidean distance is less than the threshold, the pixel will be converte to white color, or it will black.

By sliding the fraction bar under "transtion", you will be able to change the threshold.

**Time To Complete Assignment:** 3 hours


