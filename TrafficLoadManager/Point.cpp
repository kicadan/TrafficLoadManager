#include "Point.h"



Point::Point()
{
}

Point::Point(float x, float y)
{
	this->_x = x;
	this->_y = y;
}

Point::~Point()
{
}

float Point::x()
{
	return this->_x;
}

float Point::y()
{
	return this->_y;
}

bool Point::isFree()
{
	return this->free;
}

bool Point::operator==(Point other)
{
	return this->_x == other._x && this->_y == other._y;
}
