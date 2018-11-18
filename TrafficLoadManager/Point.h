#pragma once
#include "AppObject.h"

class Point :
	public AppObject
{
private:
	float x, y;
	bool free = true;
public:
	Point(float, float);
	~Point();
};

