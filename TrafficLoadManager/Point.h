#pragma once
#include "AppObject.h"

class Point :
	public AppObject
{
private:
	float _x, _y;
	bool free = true;
public:
	Point();
	Point(float, float);
	~Point();

	float x();
	float y();
	bool isFree();

	bool operator ==(Point);
};

