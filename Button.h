#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "Object.h"
#include <vector>
using std::vector;

class Button : public Object{
	
	public:
		Button();
		Button(const string& name, Vector3f position, Vector3f color, float width, float height, void (* function)() );
		Button(const string& name, Vector3f position, Vector3f color, Vector3f textColor, float width, float height, void (* function)() );
		virtual ~Button();

		//the object functions, move can be used if we want a dynamic menu or screen where we can click and drag to move buttons
		virtual void move(float x, float y, float z);
		virtual void draw();	//defualt draw will have default font of helvitica 12
		virtual const string getType() {
			return "Button";
		};

		virtual void selectionDraw(float count);	//render for selection mode, text doesnt need to be drawn
		virtual void selectionObjDraw(float count);	//same as selectin Draw but uses Obj_ID		
		void draw(void * font);
		
		virtual void takeHit(){
		Press();
		};
		
		
		//allows us to take a button and repurpose it if we want to change its functino
		void loadfunction(void(* function)() ){
			_function = function;
		};

		//executes w/e function is associated with the button
		void Press();

		virtual const int getID(){
			return button_ID;
		};

		//equal operator, to make one button equal to the other, not really usefully but you should always include this
		virtual Button const & operator = (Button const & other);

		//much more usefull operators, two buttons will be declared the same if they have the same function (ie they do the same thing)
		bool operator ==(Button const & other) const; 
		bool operator !=(Button const & other) const;
		
		//the user should be able to change his buttons color and text and get full acces to them.
		string _name;
		Vector3f _color;
		Vector3f _textColor;	//defualt text color is white
		float _width;
		float _height;

	//	Vector3f position is the only thing that is inheritited from object-- its a very sparse class	
	private:
		const int button_ID;			
		static int button_number;
		void (* _function)();	//_function is the variable name, it points to a function that 
					//returns void and takes no arguments
		void _copy(Button const & other);
		void drawString(void * font, string s, float x, float y, float z);
		void _clear();	//not sure if clear is needed, but idk how the compiler handles function variables
};

#endif

