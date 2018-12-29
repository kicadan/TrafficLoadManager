#pragma once
#include "AppObject.h"

class Vehicle :
	public AppObject
{
public:
	Vehicle();
	~Vehicle();

	virtual bool continueJourney() = 0;

protected:
	float red, green, blue;
	bool initialized = false;
	int step = 0, 
		indexOnRoad = 0,
		nextStepWhileIndex = 0; //when indexOnRoad reaches next step -> step++ and take theWay[step]
};

