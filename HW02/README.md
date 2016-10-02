#HW02 
Procedural Textures <br>
Qingjun Wu
CSCI 5239 Fall 2016<br>

#### Directions
Create a program that allows a scene to be viewed in 3D from any direction under user control.

The objects in the scene should be colored using a procedural texture shader. You may not use the brick or mandlebrot shaders shown in class.  If you use a shader from the text or the web, you should improve it.<br>

Your scene should be lit.  You may reuse the phong() function from Example 3 but improvements are always encouraged.

The resulting shader may be only a few lines long, so make sure you can justify and explain every line.

#### Program Use Instructions
Use the dropdown menus to select between the following options:
- Shader: None, "Chess Board," "Psychedelic"<br>
- Projection: Orthogonal or Perspective<br>
- Object: Cube, Teapot, or Lamp<br>

Use the sliders to adjust the display object's current position in the X, Y, and Z axis. Subsequent translations will be displayed labeled "Position" below positioning sliders.

Using the mouse, the object can be rotated by dragging with the right mouse button. Subsequent angles of rotation will be displayed below the positioning sliders. The scene can also be zoomed in/out by using the mouse wheel or scrolling with a touchpad, with the resulting zoom level also displayed.

The scene is lit by a rotating sphere which can be paused using the "Pause" button. Once paused, the button is renamed to "Animate" which will resume the sphere's rotation.

All positioning adjustments can be reset by clicking on the "Reset" button on the bottom left of the menu. This will restore the object to the center of the screen in its origiinal angle position.

Use the Quit button or the window's exit button to end the program.

**Time To Complete Assignment:** ~ 4 hours

Based on provided class Example 3.
