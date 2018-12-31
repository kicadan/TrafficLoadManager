#include "Automobile.h"



Automobile::Automobile(Way way):theWay(way)
{
	step = theWay.steps.size() - 1;
	actualStep = theWay.steps[step];
	actualRoad = actualStep.nextRoad;
	actualLaneType = actualStep.nextLaneType;
	actualPoint = theWay.from->getPoint();
	indexOnRoad = actualStep.nextPoint;
	nextStepWhileIndex = actualLaneType == BACK_LANE ? indexOnRoad - actualStep.distanceToNextJunction : indexOnRoad + actualStep.distanceToNextJunction;
}


Automobile::~Automobile()
{
}

bool Automobile::continueJourney()
{
	if ( (indexOnRoad + 1 < nextStepWhileIndex && (actualLaneType == LANE || actualLaneType == RIGHT_LANE || actualLaneType == LEFT_LANE) ) || (indexOnRoad - 1 > nextStepWhileIndex && actualLaneType == BACK_LANE)) { //nextStepWhileIndex means juntion point so do it without junction point
		if (actualRoad->getPoint(indexOnRoad + 1, actualLaneType).isFree()) {
			actualRoad->freePoint(actualLaneType, indexOnRoad);
			indexOnRoad = actualLaneType == BACK_LANE ? indexOnRoad - 1 : indexOnRoad + 1;
			actualRoad->reservePoint(actualLaneType, this, indexOnRoad);
			actualPoint = actualRoad->getPoint(indexOnRoad, actualLaneType);
		}
		draw();
		return false;
	}
	else if ( ((indexOnRoad + 1 == nextStepWhileIndex && (actualLaneType == LANE || actualLaneType == RIGHT_LANE || actualLaneType == LEFT_LANE)) || (indexOnRoad - 1 == nextStepWhileIndex && actualLaneType == BACK_LANE)) 
		&& step > 0) { //and not last step
		Light theLight = actualStep.nextJunction->getLightState(actualRoad->id, actualLaneType);
		if ( theLight == GREEN || theLight == YELLOW) { //if green light
			if (theWay.steps[step - 1].nextRoad->getPoint(theWay.steps[step - 1].nextPoint, theWay.steps[step - 1].nextLaneType).isFree()) { //if point is free
				actualRoad->freePoint(actualLaneType, indexOnRoad);
				theWay.steps[step].nextJunction->carPassedJunction(actualRoad, actualLaneType);
				step--;
				actualStep = theWay.steps[step];
				actualRoad = actualStep.nextRoad;
				actualLaneType = actualStep.nextLaneType;
				indexOnRoad = actualStep.nextPoint;
				nextStepWhileIndex = actualLaneType == BACK_LANE ? indexOnRoad - actualStep.distanceToNextJunction : indexOnRoad + actualStep.distanceToNextJunction;
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
		actualRoad->freePoint(actualLaneType, indexOnRoad);
		return false;
	}
}

void Automobile::setWay(Way way)
{
	theWay = way;
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
