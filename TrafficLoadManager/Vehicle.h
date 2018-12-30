#pragma once
#include "AppObject.h"

struct Way;
class Junction;

class Vehicle :
	public AppObject
{
public:
	Vehicle();
	~Vehicle();

	ObjectType getObjectType();
	virtual void setWay(Way) = 0;
	virtual bool continueJourney() = 0;

protected:
	float red, green, blue;
	bool initialized = false;
	int step = 0, 
		indexOnRoad = 0,
		nextStepWhileIndex = 0; //when indexOnRoad reaches next step -> step++ and take theWay[step]
};

