#pragma once
#include "Vehicle.h"
#include "Junction.h"

class Automobile :
	public Vehicle
{
public:
	Automobile(Way);
	~Automobile();

	//returns false didn't change road; true if turned to another road
	bool continueJourney();
	void setWay(Way);

private:

	Connection actualStep;
	Road* actualRoad;
	LaneType actualLaneType;
	Point actualPoint;
	Way theWay;

	void draw();
};

