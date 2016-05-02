#include "Object.h"

Object::Object()
{
	//set values for default constructor
	setType("Object");
	setColor(Scalar(0,0,0));

}

Object::Object(string name){

	setType(name);
	
	if(name=="orange"){ //*** RANDI "orange" used to be "blue"

		//TODO: use "calibration mode" to find HSV min
		//and HSV max values

		setHSVmin(Scalar(0,82,219));
		setHSVmax(Scalar(14,256,256));

		//BGR value for Green: //*** RANDI used to be 255,0,0
		setColor(Scalar(0,128,128));

	}
	if(name=="green"){

		//TODO: use "calibration mode" to find HSV min
		//and HSV max values

		setHSVmin(Scalar(95, 65, 212));
		setHSVmax(Scalar(123, 188, 256));
		
		//setHSVmin(Scalar(72, 96, 107));
		//setHSVmax(Scalar(132, 200, 242));



		//BGR value for Yellow:
		setColor(Scalar(0,255,0));

	}
	if(name=="yellow"){

		//TODO: use "calibration mode" to find HSV min
		//and HSV max values

		setHSVmin(Scalar(19,86,174));
		setHSVmax(Scalar(42,256,256));

		//BGR value for Red:
		setColor(Scalar(0,255,255));

	}
	if(name=="red"){

		//TODO: use "calibration mode" to find HSV min
		//and HSV max values

		setHSVmin(Scalar(160,96,135));
		setHSVmax(Scalar(256,256,256));

		//BGR value for Red:
		setColor(Scalar(0,0,255));

	}
}

Object::~Object(void)
{
}

int Object::getXPos(){

	return Object::xPos;

}

void Object::setXPos(int x){

	Object::xPos = x;

}

int Object::getYPos(){

	return Object::yPos;

}

void Object::setYPos(int y){

	Object::yPos = y;

}

Vec4f Object::getVector(){

	return Object::vector;

}

void Object::setVector(Vec4f x){

	Object::vector = x;

}

Rect Object::getBoundRect(){

	return Object::rect;

}

void Object::setBoundRect(Rect x){

	Object::rect = x;

}

Scalar Object::getHSVmin(){

	return Object::HSVmin;

}
Scalar Object::getHSVmax(){

	return Object::HSVmax;
}

void Object::setHSVmin(Scalar min){

	Object::HSVmin = min;
}


void Object::setHSVmax(Scalar max){

	Object::HSVmax = max;
}
