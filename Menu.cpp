#include "Menu.h"
#include <string>
#include <iostream>
#include <map>
using std::map;
using namespace std;  

Menu::Menu() : menu_ID(menu_number){
	_width = 0;
	_height = 0;
	_title = " ";
	_color = Vector3f();
	_position = Vector3f();
	menu_number ++;
	object_number ++;
	_open = false;
}

Menu::Menu(Vector3f position, Vector3f color, float width, float height, string title) : menu_ID(menu_number){
	_width = width;
	_height = height;
	_title = title;
	_color = color;
	_position = position;
	menu_number ++;
	object_number ++;
	_open = true;
}

Menu::~Menu(){
	_clear();
}

void Menu::move(float x, float y, float z){
	_position[0] += x;
	_position[1] += y;
	_position[2] += z;
}

void Menu:: draw(){
	if( !_open ) return;
	glColor3f(_color[0], _color[1], _color[2]);
	glBegin(GL_QUADS);
		glVertex3f(_position[0], _position[1], _position[2]);
		glVertex3f(_position[0]+_width, _position[1], _position[2]);
		glVertex3f(_position[0]+_width, _position[1]-_height, _position[2]);
		glVertex3f(_position[0], _position[1]-_height, _position[2]);
	glEnd();

	//title will be over by 2 spaces and down a little
	glColor3f(1,1,1);
	drawString(GLUT_BITMAP_HELVETICA_18, _title, (_position[0]+.01), (_position[1]-.08), _position[3]);
	
	//draw the buttons within the menu
	float spacing = (_height-.16) / float(buttons.size());
	int count = 0;
	//now draw the buttons
	for(map<int,Button *>::iterator it = buttons.begin(); it!=buttons.end(); it++)
	{	
		it->second->_width = _width-.06;	// want to draw the buttons with .03 width on either side
		it->second->_height = spacing;
		Vector3f dif = _position - it->second->getPosition();
		it->second->move( (dif[0]+.03), (dif[1]-.1-(count*spacing)), dif[2]);
		it->second->draw();
		count++;
	}
}

//The addition of count is to allow for the z-buffer to change on successive calls of selection draw, this will let the most recently 
//drawn object be on top and be the picked object
void Menu:: selectionDraw(float count){
	count += .00001;
	if( !_open ) return;
	glPushName(menu_ID);
	glBegin(GL_QUADS);
		glVertex3f(_position[0], _position[1], _position[2]+count);
		glVertex3f(_position[0]+_width, _position[1], _position[2]+count);
		glVertex3f(_position[0]+_width, _position[1]-_height, _position[2]+count);
		glVertex3f(_position[0], _position[1]-_height, _position[2]+count);
	glEnd();
	//render the buttons and give them their ID, they will have 2 so far
	float spacing = (_height-.16) / float(buttons.size());
	int buttonCount = 0;
	//now draw the buttons
	for(map<int,Button *>::iterator it = buttons.begin(); it!=buttons.end(); it++)
	{	
		it->second->_width = _width-.06;	// want to draw the buttons with .03 width on either side
		it->second->_height = spacing;
		Vector3f dif = _position - it->second->getPosition();
		it->second->move( (dif[0]+.03), (dif[1]-.1-(buttonCount*spacing)), dif[2]);
		it->second->selectionDraw(count);
		buttonCount++;	
	}
	glPopName();
}

//to be called whenever working in a data structure that contains Object * instead of Menu *
void Menu:: selectionObjDraw(float count){
	count += .00001;
	if( !_open ) return;
	glPushName(getObjectID());
	glBegin(GL_QUADS);
		glVertex3f(_position[0], _position[1], _position[2]+count);
		glVertex3f(_position[0]+_width, _position[1], _position[2]+count);
		glVertex3f(_position[0]+_width, _position[1]-_height, _position[2]+count);
		glVertex3f(_position[0], _position[1]-_height, _position[2]+count);
	glEnd();
	//render the buttons and give them their ID, they will have 2 so far
	float spacing = (_height-.16) / float(buttons.size());
	int buttonCount = 0;
	//now draw the buttons
	for(map<int,Button *>::iterator it = buttons.begin(); it!=buttons.end(); it++)
	{	
		it->second->_width = _width-.06;	// want to draw the buttons with .03 width on either side
		it->second->_height = spacing;
		Vector3f dif = _position - it->second->getPosition();
		it->second->move( (dif[0]+.03), (dif[1]-.1-(buttonCount*spacing)), dif[2]);
		it->second->selectionObjDraw(count);
		buttonCount++;
	}
	glPopName();
}

// Again im sloppy but learning, all objects that are capable of holding other objects must have this function or one similar to it
Object * Menu::findObject(GLuint * ptr, GLuint nameLength)	//a must for all objects that can contain other objects 
{
	
	if(buttons.find(*ptr) != buttons.end() && nameLength == 1){
		//cout<<"returning "<<buttons.find(*ptr)->first<<endl;
		return buttons.find(*ptr)->second;
	}		
	
	//something terrible happened, so tell us an error occured and return a blank object
	cout<<"Something terrible happened in Menu's findObject call"<<endl;
	Object * a = new Button();
	return a;
}

void Menu:: draw(void * font){
	//cout<<"in menu draw"<<endl;
	if( !_open ) return;
	glColor3f(_color[0], _color[1], _color[2]);
	glBegin(GL_QUADS);
		glVertex3f(_position[0], _position[1], _position[2]);
		glVertex3f(_position[0]+_width, _position[1], _position[2]);
		glVertex3f(_position[0]+_width, _position[1]-_height, _position[2]);
		glVertex3f(_position[0], _position[1]-_height, _position[2]);
	glEnd();

	//title will be over by 2 spaces and down a little
	glColor3f(1,1,1);
	drawString(font, _title, (_position[0]+.01), (_position[1]-.08), _position[3]);
	
	//now draw the buttons
	float spacing = (_height-.16) / float(buttons.size());
	int count = 0;
	for(map<int,Button *>::iterator it = buttons.begin(); it!=buttons.end(); it++)
	{	count++;
		it->second->_width = _width-.06;	// want to draw the buttons with .03 width on either side
		it->second->_height = spacing;
		Vector3f dif = _position - it->second->getPosition();
		it->second->move( (dif[0]+.03), (dif[1]-.1-(count*spacing)), dif[2]);
		it->second->draw();
	}

}

void Menu::drawString(void * font, string s, float x, float y, float z){
	unsigned int i;
	glRasterPos3f(x,y,z);
	
	for(i= 0; i < s.length(); i++)
          glutBitmapCharacter (font, s[i]);
}

void Menu::addButton(Button* newButton, const int ID){

	buttons[ID] = newButton;
}

void Menu::resizeSide(float newlength, int side){
	switch (side){
		case 1:	//move the top by newlength
			_height += newlength;
			move(0, newlength, 0);
			break;
		case 2: //move the right side
			_width += newlength;
			break;
		case 3: //move the bottom up
			_height -= newlength; // - because + is up in window cordinates
			break;
		case 4: 
			_width -= newlength; //- because youll be squeezing the window if you drag it forward
			move(newlength, 0, 0);
			break;
		default:
			cout<<"error in the code for which side of the window has been touched to resize"<<endl;
		}
}

void Menu::resizeCorner(float newWidth, float newHeight, int corner){
	switch (corner){
		case 1:
			_height += newHeight;
			_width -= newWidth;
			move(newWidth, newHeight, 0);
			break;
		case 2:
			_height += newHeight;
			_width += newWidth;
			move(0, newHeight, 0);
			break;
		case 3:
			_height -= newHeight;
			_width += newWidth;
			break;
		case 4:
			_height -= newHeight;
			_width -= newWidth;
			move(newWidth, 0, 0);
			break;
		default:
			cout<<"error in the code for which corner of the window has be touched to resize"<<endl;
		}
}

void Menu::removeButton(const int ID){
	map<const int, Button*>::iterator it = buttons.find(ID);	
	delete buttons[ID];	
	buttons.erase(it);
}

//this should only be used if the menu is minimized
void Menu::open(){
	_open = true;
}

void Menu::minimize(){
	_open = false;
}

//every time we exit the menu we are deleting it, that is not minimizing it
void Menu::exit(){
	_clear();
}

Menu const & Menu::operator= (Menu const & other){

	if(&other != this)
	{	_clear();
		_copy(other);
	}
	return *this;
}

void Menu::_copy(Menu const & other){
	_width = other._width;
	_height = other._height;
	_title = other._title;
	_color = other._color;
	_position = other._position;
	_open = other._open;
	//safety check
	if(buttons.size() >= 1){
		cout<<"Menu copy was called but its buttons vector was not cleared"<<endl;
		return;
	}
	//copy the buttons: the buttons will have been cleared first;
	buttons = other.buttons;
}

void Menu::_clear(){
	_width = 0;
	_height = 0;
	_title = "";
	_color = Vector3f();
	_position = Vector3f();
	_open = false;

	if(buttons.size()!= 0)
	{	map<int, Button *>::iterator it = buttons.begin();
		while(it != buttons.end()){
			delete it->second;
			buttons.erase(it);
		}	
	}
}







		








