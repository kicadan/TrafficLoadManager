#pragma once
#include "Vehicle.h"
#include "Junction.h"

class Automobile :
	public Vehicle
{
public:
	Automobile(Junction*, Junction*, Way);
	~Automobile();

	bool continueJourney();

private:

	Junction * from;
	Junction * to;
	Connection actualStep;
	Road* actualRoad;
	LaneType actualLaneType;
	Point actualPoint;
	Way theWay;

	void draw();
};

