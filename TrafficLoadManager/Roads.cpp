#include "Roads.h"

Point Road::searchPoint(QPoint point)
{
	for (auto &existingPoint : this->mid) {
		if ((point.x() > floor(existingPoint.x() - distance / 2) && point.x() < ceil(existingPoint.x() + distance / 2)) && (point.y() > floor(existingPoint.y() - distance / 2) && point.y() < ceil(existingPoint.y() + distance / 2)))
			return Point(existingPoint);
	}
	return Point(0, 0);
}

void Road::addOtherRoad(int roadNumber)
{
	this->otherRoads.push_back(roadNumber);
}

LineParams Road::getBermParams(LaneType laneType)
{
	return laneType == LEFT_BERM ? bermLParams : bermRParams;
}

 LaneType Road::returnCloserBerm(QPoint point)
{
	double distanceFromLeftBerm = 0.0;
	double distanceFromRightBerm = 0.0;
	distanceFromLeftBerm = fabs(-bermLParams.a*point.x() + point.y() - bermLParams.b) / (double)sqrt(pow(bermLParams.a, 2) + 1);
	distanceFromRightBerm = fabs(-bermRParams.a*point.x() + point.y() - bermRParams.b) / (double)sqrt(pow(bermRParams.a, 2) + 1);
	return distanceFromLeftBerm > distanceFromRightBerm ? RIGHT_BERM : LEFT_BERM;
}

 RoadType Road::getRoadType()
 {
	 return roadType;
 }

