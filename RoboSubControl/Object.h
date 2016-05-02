#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include <cv.h>
#include <highgui.h>

using namespace std;
using namespace cv;

class Object
{
public:
	Object();
	~Object(void);

	Object(string name);

	int getXPos();
	void setXPos(int x);

	int getYPos();
	void setYPos(int y);

	Vec4f getVector();
	void setVector(Vec4f);

	Rect getBoundRect();
	void setBoundRect(Rect);

	Scalar getHSVmin();
	Scalar getHSVmax();

	void setHSVmin(Scalar min);
	void setHSVmax(Scalar max);

	string getType(){return type;}
	void setType(string t){type = t;}

	Scalar getColor(){
		return Color;
	}
	void setColor(Scalar c){

		Color = c;
	}

private:

	int xPos, yPos;
	Vec4f vector;
	Rect rect;
	string type;
	Scalar HSVmin, HSVmax;
	Scalar Color;
};
#endif