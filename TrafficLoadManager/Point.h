#pragma once
#include "AppObject.h"
#include "Vehicle.h"

class Point :
	public AppObject
{
private:
	double _x, _y;
	bool free = true;
	int _junctionNumber = -1;
	Vehicle* vehicle;

public:
	Point();
	Point(QPointF);
	Point(float, float);
	~Point();

	double x();
	double y();
	//bool setVehicle(Vehicle*);
	Vehicle* getVehicle();
	void setFree();
	bool occupy(Vehicle*);
	bool isFree();
	int junctionNumber();
	ObjectType getObjectType();

	bool operator ==(Point);
	bool operator ==(QPointF);
};

