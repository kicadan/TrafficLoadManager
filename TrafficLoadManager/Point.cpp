#include "Point.h"



Point::Point():AppObject(APP_POINT)
{
}

Point::Point(QPointF point):_x(point.x()), _y(point.y())
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

double Point::x()
{
	return this->_x;
}

double Point::y()
{
	return this->_y;
}

Vehicle * Point::getVehicle()
{
	return vehicle;
}

void Point::setFree()
{
	free = true;
	vehicle = NULL;
}

bool Point::occupy(Vehicle *vehicle)
{
	bool succesfull = free;
	if (free) {
		free = false;
		this->vehicle = vehicle;
	}
	return succesfull;
}

bool Point::isFree()
{
	return this->free;
}

int Point::junctionNumber()
{
	return this->_junctionNumber;
}

ObjectType Point::getObjectType()
{
	return _objectType;
}

bool Point::operator==(Point other)
{
	return this->_x == other._x && this->_y == other._y;
}

bool Point::operator==(QPointF other)
{
	return this->_x == other.x() && this->_y == other.y();
}
