# CSCI5239_HW01
NDC to RGB Shader
Qingjun Wu (Wade)
CSCI 5239 Fall 2016 library course

#### Directions
Create a program that displays a scene in 3D that can be viewed from any
direction under user control.

The color of every vertex in every object should be set using a shader such
that the coordinates of the vertex in Normalized Device Coordinates (-1 to +1
in x,y,z) determines the color of the vertex using a linear mapping in RGB space.

Your program must work in both orthogonal and perspective, so NDC space starts as
4D homogeneous coordinates and w will not be 1.

#### Program Use Instructions

This homework is done on top of example code demonstrated in class. I added a few things: 
1. A new object Lamp is added; the source code is from my homework solution of course Computer Graphics, but I made quite some change to make it work here in QT Widget;
2. A Scale slider is also added, by dragging the slide, you will be able to scale the object.

Funcationality supported in the QT UI:
- Shader: None or Basic
- Projection: Orthogonal or Perspective
- Object: Cube, Teapot, T-Rex and Lamp
- Position: move the object in x, y, and z directions
- Scale: using the scala slide to scale the size of the object 
- Rotate: rotation is done by using the mouse to drag the object in the main window

Use the Quit button or the window's exit button to end the program.

**Time To Complete Assignment:** ~ 7 hours

Based on provided class Example 02.
