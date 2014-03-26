GUI/OpenGL Tools
Author: Caleb Hanselman

Overview: This project allows for an easier time creating programs/applicications in desktop openGL by providing methods to build objects, hold objects,
select objects, draw and format objects and so on. The vision of this program is to build a small library that will allow for an application like "Paint"
or chess, or some data editor that can show graphs to be written in a seperate file, and then displayed and processed in the main.cpp. To do this I 
decided to label everything that is drawn or proccesed in openGL as "objects" with some base properties, and have it be a abstract class. Any other
"thing" that will be drawn in openGL will be a child of the "Objects" class. This allows for easy picking, drawing, movement, and much more. This project
is much larger than any of my others, and as so is continually being added on, but for the time being the basic idea is complete. 

Compilation: To compile this just run the makefile after confirming that other openGL programs will compile and then exectue the gui_demo executable.

Demonstration: I made a very very simple enviroment that demonstrates my simple goal with this project. After compiling and running, teal blue 
"enviroment" will open that has two buttons. Each button, when clicked will spawn a menu. And one menu has buttons that have actions attached to them.
My goals with this demonstration were to: 1) Prove drawing worked by iterating through an enviroments objects and calling their subsequent draw functions
2) prove that my picking method worked and will work for any enviroment I make by just calling an enviroments selection function. 3)prove that controls 
can easily be placed onto objects selected (this means you can program click and dragability) and 4) prove that creating an enviroment in a seperate file
will be much simpler and once done, rendering and acting with it will be much cleaner. 

Problems: First and formost is the lack of complete features. I still need to implement a texture feature, as well as lighting and even a shader perhaps.
This is due to the fact that this is an ongoing project and my first goal is to use these tools to make a few simple programs like chess, paint etc.
Also the comenting on this project is pretty abysmal. This is one of my own flaws, as I tend to go over a program and comment it after a few days away
from it, because I begin to explain it to myself and just type out my explanations. And finally is the actual Object Oriented Design. I did not take 
enough time to sit down and plan out the relations of all my classes, and this resulted in having few classes with many responsibilities. There are lots
of changes i would like to make in retrospect to make it cleaner or more scalable, but as is, i still think the program is scalabe and versatile.


