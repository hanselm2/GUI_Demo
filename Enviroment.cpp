#include "Enviroment.h"
#include <iostream>
#include <map>

using namespace std;
using std::vector;
using std::multimap;
using std::map;

Enviroment::Enviroment(): enviro_ID(enviro_number){

	_controls = NULL;
	_mouse = NULL;
	_width = 0;
	_height = 0;
	_color = Vector3f();
	_position = Vector3f();
	enviro_number ++;
	object_number ++;
	_open = false;
}

Enviroment::Enviroment(multimap<string, map<int, Object *> > objects, void (* controls)(unsigned char, int, int), float width, float height, Vector3f position, Vector3f color,  void (* mouse)(int, int)) : enviro_ID(enviro_number){
	_objects = objects;	
	_controls = controls;
	_width = width;
	_height = height;
	_position = position;
	enviro_number ++;
	object_number ++;
	_color = color;
	_mouse = mouse;
	_open = true;
}
void Enviroment::move(float x, float y, float z){
	Vector3f move = Vector3f(x, y, z);
	_position = _position + move;
	for(multimap<string, map<int, Object *> >::iterator it1=_objects.begin(); it1!=_objects.end(); it1++)
		for(map<int, Object *>::iterator it2=it1->second.begin(); it2!=it1->second.end(); it2++)
			it2->second->move(x,y,z);
};

//we are subtracting .1 from the z component in the selection, so whenever an object is called it will be moved 
void Enviroment::selectionDraw(float count){
	count += .00001;
	if( !_open ) return;
	glPushName(enviro_ID);
	glBegin(GL_QUADS);
		glVertex3f(_position[0], _position[1], _position[2]+count);
		glVertex3f(_position[0]+_width, _position[1], _position[2]+count);
		glVertex3f(_position[0]+_width, _position[1]-_height, _position[2]+count);
		glVertex3f(_position[0], _position[1]-_height, _position[2]+count);
	glEnd();
	//assumes all objects will be kept inside the enviroment, need to do clipping somehow to ensure this. 
	for(multimap<string, map<int, Object *> >::iterator it1=_objects.begin(); it1!=_objects.end(); it1++)
		for(map<int, Object *>::iterator it2=it1->second.begin(); it2!=it1->second.end(); it2++)
			it2->second->selectionDraw(count);
	glPopName();	
}

//to be called whenever working in a data structure that contains Object * instead of Enviroment *
void Enviroment::selectionObjDraw(float count){
	count += .00001;
	if( !_open ) return;
	glPushName(getObjectID());
	glBegin(GL_QUADS);
		glVertex3f(_position[0], _position[1], _position[2]+count);
		glVertex3f(_position[0]+_width, _position[1], _position[2]+count);
		glVertex3f(_position[0]+_width, _position[1]-_height, _position[2]+count);
		glVertex3f(_position[0], _position[1]-_height, _position[2]+count);
	glEnd();
	//assumes all objects will be kept inside the enviroment, need to do clipping somehow to ensure this. 
	for(multimap<string, map<int, Object *> >::iterator it1=_objects.begin(); it1!=_objects.end(); it1++)
		for(map<int, Object *>::iterator it2=it1->second.begin(); it2!=it1->second.end(); it2++)
			it2->second->selectionObjDraw(count);
	glPopName();	
}

// Again im sloppy but learning, all objects that are capable of holding other objects must have this function or one similar to it
Object * Enviroment::findObject(GLuint * ptr, GLuint nameLength)	//a must for all objects that can contain other objects 
{
	for(multimap<string, map<int, Object *> >::iterator it = _objects.begin(); it!=_objects.end(); it++){
		map<int, Object*> mymap = it->second;	//just to keep it neat, and not mess up on pointers
		if(mymap.find(*ptr)!=mymap.end())	//Because we use ObjectID when using _objects only one map will contain the ID
		{	nameLength --; //now we go down to the next object, IT MUST BE A CONTAINER OR NAMELENGTH SHOULD = 0 NOW
			if(nameLength != 0){
			/* IMPORTANT: If we want large scalability we would need to make a way to find out what type the object should 
			be cast too. I was going to take the getType() function, and try to make a function that "hacks" the string
			to be set as code to the compiler. But the internet says i cant do that. So the other option is to make a 
			seperate small class that will store all children of Object, in a map with their GetType() and then call that to
			return the dynamic cast. So this sloppy code is the result, it can be interchaned for a method if it gets larger
			*/
				string type = mymap.find(*ptr)->second->getType();
				GLuint * prev = ptr;
				ptr++;
				cout<<*prev<<endl;
				cout<<*ptr<<endl;
				if( type == "Menu"){
					Object * holder = mymap.find(*prev)->second;
					
					//cout<<"returning "<<dynamic_cast<Menu*>(holder)->findObject(prev, nameLength)->getObjectID()<<endl;
					return dynamic_cast<Menu*>(holder)->findObject(ptr, nameLength);
					}
		
				else if( type == "Enviroment"){
					Object * holder = mymap.find(*ptr)->second;					
					dynamic_cast<Enviroment*>(holder)->findObject(prev, nameLength);
					}

				else cout<<"No type match in Enviroment's findObject()"<<endl;
			}
			else //namelength is 0 and we have reached the object we actually clicked
				return mymap.find(*ptr)->second;
		}
	}
	
	//something terrible happened, so tell us an error occured and return a blank object
	cout<<"Something terrible happened in Enviroments findObject call"<<endl;
	Object * a = new Button();
	return a;
}

//Again a major problem is we might need to clip objects if they are off screen
void Enviroment::draw(){
	if( !_open ) return;
	glColor3f(_color[0], _color[1], _color[2]);
	glBegin(GL_QUADS);
		glVertex3f(_position[0], _position[1], _position[2]);
		glVertex3f(_position[0]+_width, _position[1], _position[2]);
		glVertex3f(_position[0]+_width, _position[1]-_height, _position[2]);
		glVertex3f(_position[0], _position[1]-_height, _position[2]);
	glEnd();
	
	for(multimap<string, map<int, Object *> >::iterator it1=_objects.begin(); it1!=_objects.end(); it1++){
		for(map<int, Object *>::iterator it2=it1->second.begin(); it2!=it1->second.end(); it2++){			
			it2->second->draw();
		}
	}
}

Enviroment const & Enviroment::operator = (Enviroment const & other){
	if(this != &other)
	{	_clear();
		_copy(other);
	}
	return *this;
}

/* many data structures going on for Enviroment's class. There will be a multimap with the type of each object, for the key of the 
multimap. This will allow for further scaling as more objects are introduced to the library. Each type of object will have its own map
that will allow for ordered look up. Ie we will have linear time (i think, not sure how maps [] operator's run time is) when we go through
our picking process where the names will be the objects ID. 
POSSIBLE BUG: unsure if the multimap will be initialized for all maps that are blank.
*/
void Enviroment::addObject(Object * newObject, const int ID){
	string type = newObject->getType();
	_objects.find(newObject->getType())->second[ID] = newObject;	//dereference because it returns map iterator
}

void Enviroment::removeObject(string type, const int ID){
	map<int, Object *> typeMap = _objects.find(type)->second;
	map<int, Object *>::iterator it = typeMap.find(ID);
	delete typeMap[ID];
	typeMap.erase(it);
}

void Enviroment::resizeSide(float newlength, int side){
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

void Enviroment::resizeCorner(float newWidth, float newHeight, int corner){
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

//this should only be called when not minimized, which has not been defined yet
void Enviroment::open(){
	_open = true;
}

void Enviroment::minimize(){
	_open = false;
}

//This means that every time we close the program we are deleting it, closing is not minimizing
void Enviroment::close(){
	_clear();
}

void Enviroment::_copy(Enviroment const & other){
	_width = other._width;
	_height = other._height;
	_controls = other._controls;
	_mouse = other._mouse;
	_color = other._color;
	_position = other._position;
	_open = other._open;
	if(_objects.size() >= 1){
		cout<<"Enviro copy was called, without calling clear first"<<endl;
		return;
	}
	_objects = other._objects;
}

void Enviroment::_clear(){
	_width = 0;
	_height = 0;
	_controls = NULL;
	_mouse = NULL;	
	_color = Vector3f();
	_position = Vector3f ();
	_open = false;

	if(_objects.size() !=0){
		multimap<string, map< int, Object *> >::iterator it1 = _objects.begin();
		while(it1 != _objects.end()){
			map<int, Object *>::iterator it2 = it1->second.begin();
			while( it2 != it1->second.end() ){
				delete it2->second;
				it1->second.erase(it2);
			}
			_objects.erase(it1);
		}
	}
}






