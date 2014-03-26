#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>//need to change this include to #include <GL/GLUT.h> for xcode. and relevant glut library for linux
#include "Vector3f.h"
#include <vector>
#include <iostream>
#include <cmath>
#include <string.h>
#include <list>

#include "Object.h"
#include "Button.h"
#include "Menu.h"
#include "Enviroment.h"
#include "testenviro.h"

/* an untransformed viewport matrix has units of 700 pixels for 1 unit in the window size of 1500x1200
There are probably better methods like glortho but that stops 3d from being enabled.
*/
using std::list;
using std::vector;
using namespace::std;   //this and iostream for troubleshooting

#define PI 3.141592658
#define BUFSIZE 512

GLuint selectBuf[BUFSIZE];
//initialize all to be 1 because 0 is reserved for lack of an object
int Object::object_number = 1;	//guarentees uniquness but a longer search time
int Menu::menu_number = 1;
int Button::button_number = 1;
int Enviroment::enviro_number = 1;

int nFPS = 60;
double width, height, ratio;
        
//probably bad to define this as global but i couldnt think of another idea
Object * lastclicked = NULL;
Enviroment * openEnviro = NULL;
list<Enviroment *> enviros;	//alt tabbing and such

void testButton(){
        cout<<"Test function is working"<<endl;
}

//Button* testbut = new Button("this is a string", Vector3f(-1.0, 1.2, 0), Vector3f(1,0,0), 1, 1, testButton);

void init(void)
{
    // init your data, setup OpenGL environment here
    glClearColor(0.3,0.3,0.3,1.0); // clear color is gray       
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // uncomment this function if you only want to draw wireframe model
    /*
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_NORMALIZE);
    */

	makeEnviro();
	loadEnviro();
	enviros.push_front(test);
	openEnviro = test;
}



//Menu testMenu(Vector3f(-1, .75, 0), Vector3f(0,0,1), .5, 1, "testMenu"); */
void startPicking(int mouseX, int mouseY){
	GLint viewport[4];
	
	glSelectBuffer(BUFSIZE, selectBuf);
	glRenderMode(GL_SELECT);
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();		//save w/e matrix we have before we do our mouse selection
	glLoadIdentity();
	
	glGetIntegerv(GL_VIEWPORT,viewport);
	gluPickMatrix(mouseX, viewport[3]-mouseY, 5, 5, viewport);
	gluPerspective(45,ratio, 0.1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glInitNames();
	
	for(list<Enviroment *>::iterator it = enviros.begin(); it != enviros.end(); it++){
		(*it)->selectionObjDraw(0.0);
	}
}

//The point of the process is to change the last hit object, all controls in the main.cpp should focus on the last clicked object.
void processHits(unsigned int hits, GLuint buffer[]){
	GLuint names, *ptr, minZ, *ptrNames, numberOfNames;

  	ptr = (GLuint *) buffer;
	minZ = 0xffffffff;
	for (unsigned int i = 0; i < hits; i++) {
  		names = *ptr;
		ptr++;
		if (*ptr < minZ) {
			numberOfNames = names;
			minZ = *ptr;
			ptrNames = ptr+2;
	  	}
	  
	  	ptr += names+2;
	}

	/*ptr = ptrNames;
	 for (unsigned int j = 0; j < numberOfNames; j++,ptr++) {
     		printf ("%d ", *ptr);
 	}
 	 printf ("\n");*/
	ptr = ptrNames;
	//clicked in a new window, swap the order in the list now.
	if(*ptr != (unsigned int)openEnviro->getObjectID())	
	{	bool foundOtherEnviro = false;
		for(list<Enviroment *>::iterator it = enviros.begin(); it != enviros.end(); it++){
			if( (unsigned int)(*it)->getID() == *ptr ){
				Enviroment * holder = *enviros.begin(); // hold so we can swap
				*enviros.begin() = *it;
				*it = holder;
				foundOtherEnviro = true;
			}
		}
		//then we clicked in the backround, so close the current window
		if(*ptr == 0){
			openEnviro->minimize();
			foundOtherEnviro = true;
		}
		//some error somewhere
		else if(!foundOtherEnviro) cout<<"Error: No enviroment in the list with the area you just clicked on"<<endl;
	}
	//else were in the open Enviroment so now use the find function to find the corresponding object
	else if(numberOfNames == 1) lastclicked = openEnviro;
	else{
		ptr ++;
		numberOfNames --;	
		lastclicked = openEnviro->findObject(ptr, numberOfNames);
		//cout<<"last clicked is "<<lastclicked->getObjectID()<<endl;
	}		
	if (lastclicked->getType() == "Button")	 dynamic_cast<Button*>(lastclicked)->Press();
		
		
}


void stopPicking(){
	unsigned int hits;
	
	//restore the original projection matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glFlush();

	//make it render now
	hits = glRenderMode(GL_RENDER);
// then we have an array of hits, following the description layed out here http://www.lighthouse3d.com/opengl/picking/index.php?openglway3
	if(hits!=0)
		processHits(hits, selectBuf);
}

void display(void)
{
	
        //put your OpenGL display commands here
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	//Probably bad style, but in order to have accurate mouse clicking detection im transforming the scene to what the picker
	//supposes the screen looks like. 
        glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,ratio, 0.1, 1000);
	
	// reset OpenGL transformation matrix
        glMatrixMode(GL_MODELVIEW);
	for(list<Enviroment *>::iterator it = enviros.begin(); it != enviros.end(); it++){
		(*it)->draw();
	}
	
        glLoadIdentity(); // reset transformation matrix to identity
        gluLookAt(0.f,0.f,3.f,0.f,0.f,0.f,0.f,1.f,0.f);
   
        glFlush();
        glutSwapBuffers();  // swap front/back framebuffer to avoid flickering 
}


void reshape (int w, int h)
{
	width += w;
	height += h;
        // reset viewport ( drawing screen ) size
        glViewport(0, 0, w, h);
        float fAspect = ((float)w)/h; 
        // reset OpenGL projection matrix
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(70.f,fAspect,0.001f,30.f); 
}

void keyboard(unsigned char key, int x, int y)
{
    // TODOOOO    
    // put your keyboard control here
    
    	if (key == 97) 
        	lastclicked->move(-.01,0,0);
    
   	if (key == 100) //d has been pressed 
		lastclicked->move(.01,0,0);
    
    	if (key == 115) //s has been pressed 
		lastclicked->move(0,-.01,0);

    	if (key == 119) //w has been pressed
		lastclicked->move(0,.01,0);
	
        if (key == 27) 
        {
        	// ESC hit, so quit
        	printf("demonstration finished.\n");
        	exit(0);
        }                                            
}

void mouseUp(int x, int y){
	//oldx = x;
	//oldy = y;
}

void mouseDown(int x, int y)
{	
	startPicking(x,y);
	stopPicking();
}

void mouse(int button, int state, int x, int y)
{
       if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        	mouseDown(x,y);
	
	//startPicking(x,y);
	//stopPicking();
}

void timer(int v)
{
        glutPostRedisplay(); // trigger display function by sending redraw into message queue
        glutTimerFunc(1000/nFPS,timer,v); // restart timer again
	ratio = width / height;

}

int main(int argc, char* argv[])
{
        glutInit(&argc, (char**)argv);
        //set up for double-buffering & RGB color buffer & depth test
        glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	width = 1500;
	height = 1000; 
	glutInitWindowSize (1500, 1000); 
        glutInitWindowPosition (100, 100);
        glutCreateWindow ((const char*)"Test GUI Demo");
        
        init(); // setting up user data & OpenGL environment

        // set up the call-back functions 
        glutDisplayFunc(display);  // called when drawing 
        glutReshapeFunc(reshape);  // called when change window size
        glutKeyboardFunc(keyboard); // called when received keyboard interaction
	glutPassiveMotionFunc(mouseUp);
        glutMouseFunc(mouse);       // called when received mouse interaction
        glutTimerFunc(100,timer,nFPS); // a periodic timer. Usually used for updating animation
                                                              
        glutMainLoop(); // start the main message-callback loop
        
        return 0;
}
    

