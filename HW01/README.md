# CSCI5239_HW1
NDC to RGB Shader
Diana Southard
CSCI 5239 Spring 2016

#### Directions
Create a program that displays a scene in 3D that can be viewed from any
direction under user control.

The color of every vertex in every object should be set using a shader such
that the coordinates of the vertex in Normalized Device Coordinates (-1 to +1
in x,y,z) determines the color of the vertex using a linear mapping in RGB space.

Your program must work in both orthogonal and perspective, so NDC space starts as
4D homogeneous coordinates and w will not be 1.

#### Program Use Instructions
Use the dropdown menus to select between the following options:
- Shader: None or Basic
- Projection: Orthogonal or Perspective
- Object: Cube, Teapot, or T-Rex

Use the sliders to adjust the display object's current position.
*Note:* If the Basic shader option is selected, the object's current position will be used to set the object's vertex coloring.

Using the mouse, the object can be rotated by dragging with the right mouse button. Subsequent angles of rotation will be displayed below the positioning sliders. The scene can also be zoomed in/out by using the mouse wheel or scrolling with a touchpad.

Use the Quit button or the window's exit button to end the program.

**Time To Complete Assignment:** ~ 7 hours

Based on provided class Example 1.
