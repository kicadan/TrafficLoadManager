#include "Vehicle.h"
#include <QRandomGenerator>

Vehicle::Vehicle():AppObject(VEHICLE)
{
	QRandomGenerator random;
	red = random.generateDouble();
	green = random.generateDouble();
	blue = random.generateDouble();
}


Vehicle::~Vehicle()
{
}

