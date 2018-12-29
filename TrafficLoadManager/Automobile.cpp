#include "Automobile.h"



Automobile::Automobile(Junction* from, Junction* to, Way way):from(from), to(to), theWay(way)
{
	actualStep = theWay.steps[step];
	actualRoad = actualStep.nextRoad;
	actualLaneType = actualStep.nextLaneType;
	actualPoint = from->getPoint();
	indexOnRoad = actualStep.nextPoint;
	nextStepWhileIndex = indexOnRoad + actualStep.distanceToNextJunction;
}


Automobile::~Automobile()
{
}

bool Automobile::continueJourney()
{
	if (indexOnRoad + 1 < nextStepWhileIndex) {
		if (actualRoad->getPoint(indexOnRoad + 1, actualLaneType).isFree()) {
			actualRoad->freePoint(actualLaneType, indexOnRoad);
			indexOnRoad++;
			actualRoad->reservePoint(actualLaneType, this, indexOnRoad);
			actualPoint = actualRoad->getPoint(indexOnRoad, actualLaneType);
		}
		draw();
		return true;
	}
	else if (indexOnRoad + 1 == nextStepWhileIndex && step < theWay.steps.size()) { //and not last step
		Light theLight = actualStep.nextJunction->getLightState(actualRoad->id, actualLaneType);
		if ( theLight == GREEN || theLight == YELLOW) { //if green light
			if (theWay.steps[step + 1].nextRoad->getPoint(theWay.steps[step + 1].nextPoint, theWay.steps[step + 1].nextLaneType).isFree()) { //if point is free
				actualRoad->freePoint(actualLaneType, indexOnRoad);
				step++;
				actualStep = theWay.steps[step];
				actualRoad = actualStep.nextRoad;
				actualLaneType = actualStep.nextLaneType;
				indexOnRoad = actualStep.nextPoint;
				nextStepWhileIndex = indexOnRoad + actualStep.distanceToNextJunction;
				actualRoad->reservePoint(actualLaneType, this, indexOnRoad);
				actualPoint = actualRoad->getPoint(indexOnRoad, actualLaneType);
			}
		}
		else {
			//stay on red light
		}
		draw();
		return true;
	}
	else { //if end of the journey, just delete yourself - return false
		return false;
	}
}

void Automobile::draw()
{
	glColor3f(red, green, blue);
	glBegin(GL_POLYGON);
	glVertex2f(actualPoint.x() - 1.2*distance / 2, actualPoint.y() - 1.2*distance / 2);
	glVertex2f(actualPoint.x() + 1.2*distance / 2, actualPoint.y() - 1.2*distance / 2);
	glVertex2f(actualPoint.x() + 1.2*distance / 2, actualPoint.y() + 1.2*distance / 2);
	glVertex2f(actualPoint.x() - 1.2*distance / 2, actualPoint.y() + 1.2*distance / 2);
	glEnd();
	glFlush();
}
