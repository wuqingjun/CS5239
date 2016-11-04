# CSCI5239_HW6
iOS
Qingjun Wu
CSCI 5239 Fall 2016

#### Directions
Homework6: Android and iOS
Create a program that allows a scene to be viewed in 3D from any direction
under user control that can be run on the iPhone/iPod/iPad or Android.


You must build some solid 3D objects yourself using OpenGL ES.  In particular,
there are a number of frameworks such as GLKit on the iPhone that permits you
to write applications without using OpenGL ES.  In this assignment I would like
you to get some exposure to the nuts and bolts of OpenGL ES, so using such
frameworks is prohibited.


You may want to start with my simple application and explore features such as
lighting and textures, gestures, and so on.  However, you do NOT have to follow
my example (borrowed from Rideout's iPone 3D programming) and develop the core
code in C++ and only providing a native wrapper.  You may want to  stick with 
Objective C on the iPhone or Java on the Android throughout your program. The
Android SDK contains examples that are pure Java.


You may also want to explore the Qt for Android or Qt for iPhone approach.  If you do,
I need you to improve the examples provided in their tutorials.


The requirements are deliberately vague.  What I want you to do is improve significantly
upon what you start with.  So your readme show itemize the improvements you made
from whatever you tarted with.


## This is potentially a difficult assignment since you need to install the Xcode
environment for the iPhone or the Android SDK/NDK.  Therefore you may work in
pairs for this assignment with somebody a bit further along on the curve.  Your
README should clearly tell me what the contribution of each team member was.


When you submit the assignment, the person whose environment you used should
upload the project.  Look at my examples on how to clean out stuff I don't
need, and upload only that.


For the presentations, the presentation would be by one individual.


If the iPhone/Android is your thing, please volunteer.  However, please bear in
mind that many people in class are just starting, so be careful not to talk over
people's heads.



#### Program Use Instructions
Gestures are supported in the solustion, so you can use your finger(s) to move around the object, scale or rotate it. 

I am doing this homework based on an online instruction and its sample code:https://www.raywenderlich.com/48299/export-blender-models-opengl-es-part-3. My contribution to this: 
1. added an object - a lamp cover to it; I wrote some code in C++ to generate the vertices coordinate, texture coordinates and also compute the normals;
2. did some research and implemented gestures - using code of another online sample;
3. the original sample code is using Phone lighting mode, but when I am testing with the lamp cover, the light doesn't like working, so I fixed(improved) it. 

**Time To Complete Assignment:** 12 hours


