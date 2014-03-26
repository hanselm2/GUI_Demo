#ifndef _ENVIROMENT_H_
#define _ENVIROMENT_H_

#include "Object.h"
#include <vector>
#include <map>
#include "Button.h"
#include "Menu.h"
using std::vector;
using std::multimap;
using std::map;

class Enviroment : public Object {
	
	public:
		Enviroment();

		Enviroment(multimap<string, map<int, Object *> > objects, void (* controls)(unsigned char, int, int), float width, float height, Vector3f position, Vector3f color,  void (* mouse)(int, int) );

		
		virtual void move(float x, float y, float z);
		//will execute the controls then draw. I dont think i need to just call the controls ever
		virtual void draw();
		virtual void selectionDraw(float count);
		virtual void selectionObjDraw(float count);	//same as selectin Draw but uses Obj_ID
		Object * findObject(GLuint *ptr, GLuint nameLength);	//a must for all objects that can contain other objects 
		virtual const string getTyper(){
			return "Enviroment";
		};

		virtual ~Enviroment(){
			_clear();
		};

		virtual Enviroment const & operator = (Enviroment const & other);
			
		//now enviroments unique functions
		void doControls(unsigned char key, int x, int y){
		_controls(key,x,y);
		};
		
		void doMouse(int x, int y){
		_mouse(x, y);	
		};	

		void addObject(Object * newObject, const int ID);
		
		void removeObject(string type, const int ID);
		
		void resizeSide(float newlength, int side); //1 is top, 2 is right, 3 is bottom, 4 is left
		
		void resizeCorner(float newWidth, float newHeight, int corner);//1=NW, 2=NE, 3=SE, 4=SW

		bool isOpen(){
			return _open;
		};
		
		virtual void takeHit(){
		};

		virtual const int getID(){
			return enviro_ID;
		};
		
		//the simple open and close functions
		void open();
		void minimize();
		void close();

		float _width, _height;	
		Vector3f _color; //color for the backround of the enviro
	private:
		bool _open;
		static int enviro_number;
		const int enviro_ID;
		multimap<string, map<int, Object *> > _objects;
		void (* _controls)(unsigned char, int, int);	//keeping these private so you must accurately fix these on creation
		void (* _mouse)(int, int);		
		
		void _copy(Enviroment const & other);
		void _clear();

};




#endif
