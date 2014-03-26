#ifndef _MENU_H_
#define _MENU_H_

#include "Object.h"
#include "Button.h"
#include <vector>
#include <map>
using std::vector;

class Menu : public Object {

	public:
		//for both creations we will increment the menu number and set the menu_ID to be equal to it
		Menu();
		//the menu may have a title, and color. Its position will be the position of its upper left
		// hand corner, and will be a rectangle of width and height
		Menu(Vector3f position, Vector3f color, float width, float height, string title);
		virtual ~Menu();

		virtual void move(float x, float y, float z);
		virtual void draw();
		virtual void selectionDraw(float count);
		virtual void selectionObjDraw(float count);	//same as selectin Draw but uses Obj_ID
		Object * findObject(GLuint * ptr, GLuint nameLength);	//a must for all objects that can contain other objects 
		void draw(void * font);
		virtual const string getType() {
			return "Menu";
		};
		virtual void takeHit(){
		};

		float getWidth(){
			return _width;
		};
		float getHeight(){
			return _height;
		};
		virtual const int getID(){
			return menu_ID;
		};

		void addButton(Button* newButton, const int ID);
		
		void resizeSide(float newlength, int side); //1 is top, 2 is right, 3 is bottom, 4 is left
		
		void resizeCorner(float newWidth, float newHeight, int corner);//1=NW, 2=NE, 3=SE, 4=SW

		void removeButton(const int ID);
		
		bool isOpen(){
			return _open;
		};
		
		void open();
		void minimize();
		void exit();

		//again not sure if i need this but its supposed to be good style to always create this
		virtual Menu const & operator=(Menu const & other);
		
		/*Things to consider: do we want to allow traversal of the menu through arrows?
		be carefull about how much functionality we also give this class, this is just the 
		shell where we store buttons (graphical things that launch functions)*/
		Vector3f _color;

	private:
		bool _open;
		std::map<int, Button *> buttons;
		float _width;
		float _height;
		string _title;
		
		const int menu_ID;
		void _copy(Menu const & other);
		void _clear();
		static int menu_number;
		void drawString(void * font, string s, float x, float y, float z);
};
#endif

