# CSCI5239_HW5
WebGL
Qingjun Wu
CSCI 5239 Fall 2016

#### Directions
Create a program that allows a scene to be viewed in 3D from any direction
under user control using WebGL.


At a minumum you need to add lighting and textures to Ex 8 and draw a different
3D solid object.


You may use Apple's CanvasMatrix library of functions provided in class or an
equivalent library to provide the transformation functions needed since I am
assuming that you know how to do transformations from the previous class.
However, you may NOT use libraries to create the objects.  The purpose of this
assignment is to give you some exposure to how objects are built in OpenGL ES.


You MAY NOT use the Three.js library or equivalent packages that does all the
OpenGL calls for you.  The reason is that it abstracts the objects and controls
to the extent that you never see WebGL.  I want you to see how WebGL actually
uses OpenGL ES 2.0.


There are a lot of new things to cope with here (HTML, Javascript and OpenGL
ES), so I don't expect you to draw a very complex scene, but I do want you to
make sure you can explain every line of code.


#### Program Use Instructions
1. The program is animated. The object in the canvas is rotating. Using the arrow keys to speed up or slow dowon the rotation;
2. The UI provides controls to switch between with/without lighting, and also change the light direction/colors.

**Time To Complete Assignment:** 12 hours

I read through a online WebGL Tutorial (http://learningwebgl.com/) and use the sample code as a template to draw my lamp. For lighting, two lights are taken account: directional light and ambient light. Light direction and color are passed from the UI to uniform variables.

The Normal matrix is calculated based on this formula:  normalMatrix = transpose(inverse(modelViewMatrix)). In this home work, a revised formula is used, based on Shy's comment in this post: http://learningwebgl.com/blog/?p=3322&cpage=1#comment-18985. 

