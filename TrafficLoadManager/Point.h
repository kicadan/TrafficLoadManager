#pragma once
#include "AppObject.h"

class Point :
	public AppObject
{
private:
	double _x, _y;
	bool free = true;
	int _junctionNumber = -1;
public:
	Point();
	Point(QPoint);
	Point(float, float);
	~Point();

	double x();
	double y();
	bool isFree();
	int junctionNumber();
	ObjectType getObjectType();

	bool operator ==(Point);
	bool operator ==(QPoint);
};

