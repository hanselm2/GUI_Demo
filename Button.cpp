#include "Button.h"
#include <string.h>
#include <iostream>
using namespace std;

Button::Button() : button_ID(button_number){
	_name = "";
	_color = Vector3f();
	_position = Vector3f();
	_width = 1;
	_height = 1;
	_function = NULL;
	_textColor = Vector3f(1.0,1.0,1.0);
	button_number ++;
	object_number ++;
}
	
Button::Button(const string& name, Vector3f position, Vector3f color, float width, float height, void (* function)() ) : button_ID(button_number){
	_name = name;
	_position = position;
	_color = color;
	_width = width;
	_height = height;
	_function = function;
	_textColor = Vector3f(1.0,1.0,1.0);
	button_number ++;
	object_number ++;
}

Button::~Button(){
	_clear();
}

Button::Button(const string& name, Vector3f position, Vector3f color, Vector3f textColor, float width, float height, void (* function)()): button_ID(button_number){
	_name = name;
	_position = position;
	_color = color;
	_width = width;
	_height = height;
	_function = function;
	
	_textColor = textColor;
	button_number ++;
}

void Button::move(float x, float y, float z){
	_position[0] += x;
	_position[1] += y;
	_position[2] += z;
}
//default draw function, font is helvitica 12 
void Button::draw(){
	glColor3f(_color[0], _color[1], _color[2]);
	glBegin(GL_QUADS);
		glVertex3f(_position[0], _position[1], _position[2]);
		glVertex3f(_position[0]+_width, _position[1], _position[2]);
		glVertex3f(_position[0]+_width, _position[1]-_height, _position[2]);
		glVertex3f(_position[0], _position[1]-_height, _position[2]);
	glEnd();
	
	//draw the text
	double sum = 0;
	for(unsigned int i = 0; i<_name.length(); i++)
		sum += glutBitmapWidth(GLUT_BITMAP_HELVETICA_12, (int)_name[i]);
	//convert sum of pixels in to opengl units : this method only works for windows near 1500x1200
	sum /= 233.0;	

	//draw the text
	glColor3f(1,1,1);
	drawString(GLUT_BITMAP_HELVETICA_12, _name,(_position[0]+(_width/2.0)-(sum/2.0)), _position[1]-(_height/2.0), _position[2]);
}

//to be called whenever working in a data structure that contains Object * instead of Button *
void Button::selectionObjDraw(float count){
	count += .00001;
	glPushName(getObjectID());
	glBegin(GL_QUADS);
		glVertex3f(_position[0], _position[1], _position[2]+count);
		glVertex3f(_position[0]+_width, _position[1], _position[2]+count);
		glVertex3f(_position[0]+_width, _position[1]-_height, _position[2]+count);
		glVertex3f(_position[0], _position[1]-_height, _position[2]+count);
	glEnd();
	glPopName();
}

void Button::selectionDraw(float count){
	count += .00001;
	glPushName(button_ID);
	glBegin(GL_QUADS);
		glVertex3f(_position[0], _position[1], _position[2]+count);
		glVertex3f(_position[0]+_width, _position[1], _position[2]+count);
		glVertex3f(_position[0]+_width, _position[1]-_height, _position[2]+count);
		glVertex3f(_position[0], _position[1]-_height, _position[2]+count);
	glEnd();
	glPopName();
}

//The start point is the top left hand corner, following convention of most pngs and bmps
void Button::draw(void * font){
	glColor3f(_color[0], _color[1], _color[2]);
	glBegin(GL_QUADS);
		glVertex3f(_position[0], _position[1], _position[2]);
		glVertex3f(_position[0]+_width, _position[1], _position[2]);
		glVertex3f(_position[0]+_width, _position[1]-_height, _position[2]);
		glVertex3f(_position[0], _position[1]-_height, _position[2]);
	glEnd();
	
	//This gives the sum of pixels, for the window we will use, 1500x1200 the ratio is 1 unit = 700 pixels
	double sum = 0;
	for(unsigned int i = 0; i<_name.length(); i++)
		sum += glutBitmapWidth(font, (int)_name[i]);
	//convert sum of pixels in to opengl units : this method only works for windows near 1500x1200
	sum /= 233.0;	

	//draw the text
	glColor3f(1,1,1);
	drawString(font, _name,(_position[0]+(_width/2.0)-(sum/2.0)), _position[1]-(_height/2.0), _position[2]);
}



//the fonts will be helvitica etc
void Button::drawString(void * font, string s, float x, float y, float z){
	unsigned int i;
	glRasterPos3f(x,y,z);
	
	for(i= 0; i < s.length(); i++)
          glutBitmapCharacter (font, s[i]);
}

void Button::Press(){
	(*_function)();
}

void Button::_clear(){
	_name = "";
	_color = Vector3f();
	_position = Vector3f();
	_width = 1;
	_height = 1;
	_function = NULL;
}

void Button::_copy(Button const & other){
	_name =  other._name;
	_color = other._color;
	_position = other._position;
	_width = other._width;
	_height = other._height;
	_function = other._function;
}

Button const & Button::operator = (Button const & other){
	if(&other != this)
	{	_clear();
		_copy(other);
	}
	return *this;
}

//they will be considered equal if they both perform the same function
bool Button::operator == (Button const & other)const {
	return (&_function == &(other._function));
}

bool Button::operator != (Button const & other)const {
	return (&_function != &(other._function));
}



