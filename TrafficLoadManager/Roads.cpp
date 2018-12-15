#include "Roads.h"

Road::Road(RoadType roadType):AppObject(ROAD), _roadType(roadType)
{
}

Point Road::searchPoint(Point point)
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

LineParams Road::getLineParams(LaneType laneType)
{
	switch (laneType) {
	case LEFT_BERM: return bermLParams;
	case RIGHT_BERM: return bermRParams;
	case LANE: return coreLineParams;
	}
}

 LaneType Road::getCloserBerm(Point point)
{
	double distanceFromLeftBerm = 0.0;
	double distanceFromRightBerm = 0.0;
	distanceFromLeftBerm = fabs(-bermLParams.a*point.x() + point.y() - bermLParams.b) / (double)sqrt(pow(bermLParams.a, 2) + 1);
	distanceFromRightBerm = fabs(-bermRParams.a*point.x() + point.y() - bermRParams.b) / (double)sqrt(pow(bermRParams.a, 2) + 1);
	return distanceFromLeftBerm > distanceFromRightBerm ? RIGHT_BERM : LEFT_BERM;
}

 RoadType Road::getRoadType()
 {
	 return _roadType;
 }

 ObjectType Road::getObjectType()
 {
	 return _objectType;
 }

 bool Road::doLineCrosses(LineParams lineParams)
 {
	 bool crossing = false;
	 double x = 0, y = 0;
	 //avoid zero dividing
	 if (lineParams.a == bermLParams.a || lineParams.a == bermRParams.a || lineParams.a == coreLineParams.a)
		 return false;
	 //cross point with left berm
	 if (!lineParams.upright && !bermLParams.upright && bermLParams.a != lineParams.a) { //first and second line are not parallel and not upright
		 x = (bermLParams.b - lineParams.b) / (lineParams.a - bermLParams.a);
		 y = x * bermLParams.a + bermLParams.b;
		 if ( (x >= min(bermLParams.firstPoint.x(), bermLParams.lastPoint.x()) - 1 && x <= max(bermLParams.firstPoint.x(), bermLParams.lastPoint.x()) + 1 && y >= min(bermLParams.firstPoint.y(), bermLParams.lastPoint.y()) - 1 && y <= max(bermLParams.firstPoint.y(), bermLParams.lastPoint.y()) + 1) 
			&& (x >= min(lineParams.firstPoint.x(), lineParams.lastPoint.x()) - 1 && x <= max(lineParams.firstPoint.x(), lineParams.lastPoint.x()) + 1 && y >= min(lineParams.firstPoint.y(), lineParams.lastPoint.y()) - 1 && y <= max(lineParams.firstPoint.y(), lineParams.lastPoint.y()) + 1) ) { //if is on both! shorts
			 crossing = true;
		 }
	 }
	 else if (lineParams.upright) {
		 x = (bermLParams.b - lineParams.b) / (lineParams.a - bermLParams.a);
		 y = x * bermLParams.a + bermLParams.b;
		 if ((x >= min(bermLParams.firstPoint.x(), bermLParams.lastPoint.x()) - 1 && x <= max(bermLParams.firstPoint.x(), bermLParams.lastPoint.x()) + 1 && y >= min(bermLParams.firstPoint.y(), bermLParams.lastPoint.y()) - 1 && y <= max(bermLParams.firstPoint.y(), bermLParams.lastPoint.y()) + 1)
			 && (x >= min(lineParams.firstPoint.x(), lineParams.lastPoint.x()) - 1 && x <= max(lineParams.firstPoint.x(), lineParams.lastPoint.x()) + 1 && y >= min(lineParams.firstPoint.y(), lineParams.lastPoint.y()) - 1 && y <= max(lineParams.firstPoint.y(), lineParams.lastPoint.y()) + 1)) { //if is on both! shorts
			 crossing = true;
		 }
	 }
	 else {  //if (bermLParams.upright)
		 x = (bermLParams.b - lineParams.b) / (lineParams.a - bermLParams.a);
		 y = x * lineParams.a + lineParams.b;
		 if ((x >= min(bermLParams.firstPoint.x(), bermLParams.lastPoint.x()) - 1 && x <= max(bermLParams.firstPoint.x(), bermLParams.lastPoint.x()) + 1 && y >= min(bermLParams.firstPoint.y(), bermLParams.lastPoint.y()) - 1 && y <= max(bermLParams.firstPoint.y(), bermLParams.lastPoint.y()) + 1)
			 && (x >= min(lineParams.firstPoint.x(), lineParams.lastPoint.x()) - 1 && x <= max(lineParams.firstPoint.x(), lineParams.lastPoint.x()) + 1 && y >= min(lineParams.firstPoint.y(), lineParams.lastPoint.y()) - 1 && y <= max(lineParams.firstPoint.y(), lineParams.lastPoint.y()) + 1)) { //if is on both! shorts
			 crossing = true;
		 }
	 }

	 //cross point with right berm
	 if (!lineParams.upright && !bermRParams.upright && bermRParams.a != lineParams.a) { //first and second line are not parallel and not upright
		 x = (bermRParams.b - lineParams.b) / (lineParams.a - bermRParams.a);
		 y = x * bermRParams.a + bermRParams.b;
		 if ((x >= min(bermLParams.firstPoint.x(), bermLParams.lastPoint.x()) - 1 && x <= max(bermLParams.firstPoint.x(), bermLParams.lastPoint.x()) + 1 && y >= min(bermLParams.firstPoint.y(), bermLParams.lastPoint.y()) - 1 && y <= max(bermLParams.firstPoint.y(), bermLParams.lastPoint.y()) + 1)
			 && (x >= min(lineParams.firstPoint.x(), lineParams.lastPoint.x()) - 1 && x <= max(lineParams.firstPoint.x(), lineParams.lastPoint.x()) + 1 && y >= min(lineParams.firstPoint.y(), lineParams.lastPoint.y()) - 1 && y <= max(lineParams.firstPoint.y(), lineParams.lastPoint.y()) + 1)) { //if is on both! shorts
			 crossing = true;
		 }
	 }
	 else if (lineParams.upright) {
		 x = (bermRParams.b - lineParams.b) / (lineParams.a - bermRParams.a);
		 y = x * bermRParams.a + bermRParams.b;
		 if ((x >= min(bermLParams.firstPoint.x(), bermLParams.lastPoint.x()) - 1 && x <= max(bermLParams.firstPoint.x(), bermLParams.lastPoint.x()) + 1 && y >= min(bermLParams.firstPoint.y(), bermLParams.lastPoint.y()) - 1 && y <= max(bermLParams.firstPoint.y(), bermLParams.lastPoint.y()) + 1)
			 && (x >= min(lineParams.firstPoint.x(), lineParams.lastPoint.x()) - 1 && x <= max(lineParams.firstPoint.x(), lineParams.lastPoint.x()) + 1 && y >= min(lineParams.firstPoint.y(), lineParams.lastPoint.y()) - 1 && y <= max(lineParams.firstPoint.y(), lineParams.lastPoint.y()) + 1)) { //if is on both! shorts
			 crossing = true;
		 }
	 }
	 else {  //if (bermRParams.upright)
		 x = (bermRParams.b - lineParams.b) / (lineParams.a - bermRParams.a);
		 y = x * lineParams.a + lineParams.b;
		 if ((x >= min(bermLParams.firstPoint.x(), bermLParams.lastPoint.x()) - 1 && x <= max(bermLParams.firstPoint.x(), bermLParams.lastPoint.x()) + 1 && y >= min(bermLParams.firstPoint.y(), bermLParams.lastPoint.y()) - 1 && y <= max(bermLParams.firstPoint.y(), bermLParams.lastPoint.y()) + 1)
			 && (x >= min(lineParams.firstPoint.x(), lineParams.lastPoint.x()) - 1 && x <= max(lineParams.firstPoint.x(), lineParams.lastPoint.x()) + 1 && y >= min(lineParams.firstPoint.y(), lineParams.lastPoint.y()) - 1 && y <= max(lineParams.firstPoint.y(), lineParams.lastPoint.y()) + 1)) { //if is on both! shorts
			 crossing = true;
		 }
	 }

	 //cross point with core lane
	 if (!lineParams.upright && !coreLineParams.upright && coreLineParams.a != lineParams.a) { //first and second line are not parallel and not upright
		 x = (coreLineParams.b - lineParams.b) / (lineParams.a - coreLineParams.a);
		 y = x * coreLineParams.a + coreLineParams.b;
		 if ((x >= min(bermLParams.firstPoint.x(), bermLParams.lastPoint.x()) - 1 && x <= max(bermLParams.firstPoint.x(), bermLParams.lastPoint.x()) + 1 && y >= min(bermLParams.firstPoint.y(), bermLParams.lastPoint.y()) - 1 && y <= max(bermLParams.firstPoint.y(), bermLParams.lastPoint.y()) + 1)
			 && (x >= min(lineParams.firstPoint.x(), lineParams.lastPoint.x()) - 1 && x <= max(lineParams.firstPoint.x(), lineParams.lastPoint.x()) + 1 && y >= min(lineParams.firstPoint.y(), lineParams.lastPoint.y()) - 1 && y <= max(lineParams.firstPoint.y(), lineParams.lastPoint.y()) + 1)) { //if is on both! shorts
			 crossing = true;
		 }
	 }
	 else if (lineParams.upright) {
		 x = (coreLineParams.b - lineParams.b) / (lineParams.a - coreLineParams.a);
		 y = x * coreLineParams.a + coreLineParams.b;
		 if ((x >= min(bermLParams.firstPoint.x(), bermLParams.lastPoint.x()) - 1 && x <= max(bermLParams.firstPoint.x(), bermLParams.lastPoint.x()) + 1 && y >= min(bermLParams.firstPoint.y(), bermLParams.lastPoint.y()) - 1 && y <= max(bermLParams.firstPoint.y(), bermLParams.lastPoint.y()) + 1)
			 && (x >= min(lineParams.firstPoint.x(), lineParams.lastPoint.x()) - 1 && x <= max(lineParams.firstPoint.x(), lineParams.lastPoint.x()) + 1 && y >= min(lineParams.firstPoint.y(), lineParams.lastPoint.y()) - 1 && y <= max(lineParams.firstPoint.y(), lineParams.lastPoint.y()) + 1)) { //if is on both! shorts
			 crossing = true;
		 }
	 }
	 else {  //if (bermLParams.upright)
		 x = (coreLineParams.b - lineParams.b) / (lineParams.a - coreLineParams.a);
		 y = x * lineParams.a + lineParams.b;
		 if ((x >= min(bermLParams.firstPoint.x(), bermLParams.lastPoint.x()) - 1 && x <= max(bermLParams.firstPoint.x(), bermLParams.lastPoint.x()) + 1 && y >= min(bermLParams.firstPoint.y(), bermLParams.lastPoint.y()) - 1 && y <= max(bermLParams.firstPoint.y(), bermLParams.lastPoint.y()) + 1)
			 && (x >= min(lineParams.firstPoint.x(), lineParams.lastPoint.x()) - 1 && x <= max(lineParams.firstPoint.x(), lineParams.lastPoint.x()) + 1 && y >= min(lineParams.firstPoint.y(), lineParams.lastPoint.y()) - 1 && y <= max(lineParams.firstPoint.y(), lineParams.lastPoint.y()) + 1)) { //if is on both! shorts
			 crossing = true;
		 }
	 }
	 return crossing;
 }

 Point Road::getOppositePoint(Point point)
 {
	 Point oppositePoint;
	 if (point == coreLineParams.firstPoint)
		 oppositePoint = coreLineParams.lastPoint;
	 else
		 oppositePoint = coreLineParams.firstPoint;
	 return oppositePoint;
 }
