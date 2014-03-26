#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "Vector3f.h"
#include <string>
using std::string;

/* I'm unsure of the necissity of this base class, perhaps it is just inexperience with casting and
polymorphism, but i believe i can use this to allow all objects to interact from this main base class 
and make their classes and function defenitions be much cleaner and reusable in other programs, from 
games to guis to w/e i want to do */

/* Important notes, in order to get a pointer from Object to a subclass we should use dynamic casts. Further The object_number can be used
in place of the subclasses counter. This MUST be the case whenever we have a data structure that takes in objects specifically rather than a derived class. 

Selection draw is in here to guarentee every object can be picked, and _fixed will be used to tell if when we pick and object, and hold
the mouse down, if we should also relocate the object.

More properties will be added here only if i believe these are properties that ALL objects might at one point use. 
*/

class Object
{
	public: 
		Object(): object_ID(object_number){
			_position = Vector3f();
		};

		virtual void move(float x, float y, float z) = 0;
		virtual void draw() = 0;
		//count is a solution to having to pick from a flat service, in the render scene each subsequent call will be closer
		virtual void selectionDraw(float count) = 0;	//MUST run glInitNames() before calling this
		virtual void selectionObjDraw(float count) = 0;	//same as selectin Draw but uses Obj_ID
		virtual const string getType() {
			return "Object";
		};
		virtual const int getID() = 0;

		const int getObjectID(){
		return object_ID;
		};
		
		virtual ~Object(){
			_position = Vector3f();
		};
		virtual Object const & operator = (Object const & other){
			if(this != &other)	_position = other._position;
			return *this;
		};
		virtual void takeHit() = 0;
				
		void unfix(){
		_fixed = false;
		};
		
		void fix(){
		_fixed = true;
		};

		Vector3f getPosition(){
			return _position;
		};
		
		
	protected:

	Vector3f _position;
	static bool _fixed;	// the default for all objects is that they are fixed, unless explicitly said otherwise
	//i ran into a problem where you cant differentiate between two objects with the same ID in opengl unless i use some weird 
	// casting method, so this is how i resolve this. 	
	static int object_number;
	const int object_ID;
};

#endif
