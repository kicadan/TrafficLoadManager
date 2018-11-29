#pragma once
#include "AppObject.h"

class Point :
	public AppObject
{
private:
	double _x, _y;
	bool free = true;
public:
	Point();
	Point(float, float);
	~Point();

	double x();
	double y();
	bool isFree();

	bool operator ==(Point);
	bool operator ==(QPoint);
};

