#ifndef _TESTENVIRO_H
#define _TESTENVIRO_H
#include "Object.h"
#include "Button.h"
#include "Menu.h"
#include "Vector3f.h"
#include "Enviroment.h"
#include <iostream>
#include <string>
#include <map>
/* Author Caleb Hanselman, simple test function to initialize a test enviroment. It only uses the 3 clases ive made so far, but all other classes will be simple to add and should scale well into the system i have made. All functions that declare an object pointer must 
load it into one of the 3 maps. This function not intended to last long, its just a test
*/

using namespace std;
using std::string;
using std::map;
using std::multimap;

float _width = 3.0;
float _height = 2.0;
Vector3f _color = Vector3f(-1.5,1,0);
Vector3f _position = Vector3f(-1.5, 1.0, 0);

multimap<string, map<int, Object *> > objectMap;
map<int,Object *> buttonMap;
map<int,Object *> menuMap;
map<int,Object * > enviroMap;
Enviroment* test;

void blank(){
};

void drawredsquare(){
	Object * redsquare = new Button ("", Vector3f(0,.5,0), Vector3f(1.0,0,0), .3, .3, blank);
	test->addObject(redsquare, redsquare->getObjectID());
};

void drawbluerect(){
	Object * bluerect = new Button ("", Vector3f(.5,1,0), Vector3f(0,0,1.0), .5, .2, blank);
	test->addObject(bluerect, bluerect->getObjectID());	
};

Object * makeMenu1(){
	float m1_w = .62;
	float m1_h = 1;
	//the width for this menus will be .12 greater than the button, because all buttons have .06 buffer on each side of the menu
	Object * menu1 = new Menu(_position+Vector3f(.05,-.05,0), Vector3f(.65,.65,.65), m1_w, m1_h, "Menu 1");
	Button * m1b1 = new Button("draw red square", Vector3f(0,0,0), Vector3f(.8,0,.8), .5, .5, drawredsquare);
	Button * m1b2 = new Button("draw blue rectangle", Vector3f(0,0,0), Vector3f(.8,0,.8), .5, .5, drawbluerect);
	dynamic_cast<Menu*>(menu1)->addButton(m1b1, m1b1->getObjectID());
	dynamic_cast<Menu*>(menu1)->addButton(m1b2, m1b2->getObjectID());
	return menu1;
};

Object * makeMenu2(){
	float m2_w = .62;
	float m2_h = 1;
	Object * menu2 = new Menu(_position+Vector3f(_width-.05-m2_w,-.05, 0), Vector3f(.65,.65,.65), m2_w, m2_h, "Menu 2");
	return menu2;	
};

void button1(){
	Object* m1 = makeMenu1();
	//store the Type(class specific), then take its map and store m1's id which should be 1 or more to be m1 itself
	test->addObject(m1, m1->getObjectID());
};

void button2(){
	Object* m2 = makeMenu2();
	test->addObject(m2, m2->getObjectID());;
};

void makeButtons(){
	float b1_w = .3;
	float b2_w = .3;
	float b1_h = .12;
	float b2_h = .12;
	
	Object* b1 = new Button("Button1", _position+Vector3f(.05,-.05,0.0), Vector3f(.1,.6,.1), b1_w, b1_h, button1);
	Object* b2 = new Button("Button2", _position+Vector3f(_width-.05-b1_w,-.05,0.0), Vector3f(.1,.1,.6), b2_w, b2_h, button2);
	test->addObject(b1, b1->getObjectID());
	test->addObject(b2, b2->getObjectID());
};

void controls(unsigned char key, int x, int y)
{
	if (key == 97) //a has been pressed
	    	//this->move(.05,0,0);
          
	if (key == 100) //d has been pressed 
		//this->move(-.05,0,0);
    
    	if (key == 115) //s has been pressed 
		//objectMap.find("Button")->second[foo::lastclicked->getID()]->move(0,.05,0);

    	if (key == 119) //w has been pressed
		//objectMap.find("Button")->second[foo::lastclicked->getID()]->move(0,.05,0);
	
	if (key == 27) 
        {
        	// ESC hit, so quit
        	printf("demonstration finished.\n");
        	exit(0);
        }                                  
};

void mouse(int x, int y){
};

void makeEnviro()
{	
	//initialize objectMap maybe
	objectMap.insert(std::make_pair<string, map<int, Object *> > ("Button", buttonMap));
	objectMap.insert(std::make_pair<string, map<int, Object *> > ("Menu", menuMap));
	objectMap.insert(std::make_pair<string, map<int, Object *> > ("Enviroment", enviroMap));
	

	test = new Enviroment(objectMap, controls, _width, _height, _position, Vector3f(0.0,1.0,1.0), mouse);
	makeButtons();
};
	
void loadEnviro()
{

};	

	

#endif
