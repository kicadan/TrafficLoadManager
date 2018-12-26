#include "OneWayOneLane.h"

OneWayOneLane::OneWayOneLane(int _id):Road(OneWayRoadWithOneLane)
{
	this->Road::id = _id;
}

OneWayOneLane::~OneWayOneLane()
{
}

void OneWayOneLane::addPoint(Point point, LaneType pointType)
{
	switch (pointType) {
	case LANE: { lane.push_back(LanePoint{ point }); mid.push_back(point); }
	}
}

void OneWayOneLane::setRoad(QPointF firstPoint, QPointF lastPoint, bool endingToOtherRoad, Junction* startJunction, Junction* endJunction)//std::vector<Road*> startRoads, std::vector<Road*> endRoads)//LineParams startBermParams, LineParams endBermParams)//, int beginRoad, int endRoad)
{
	this->parallel_segments = calc_vectors(lastPoint, firstPoint);
	
	this->Road::leftBerm = QLineF(firstPoint.x() + parallel_segments.xl, firstPoint.y() + parallel_segments.yl, lastPoint.x() + parallel_segments.xl, lastPoint.y() + parallel_segments.yl);
	this->Road::rightBerm = QLineF(firstPoint.x() + parallel_segments.xr, firstPoint.y() + parallel_segments.yr, lastPoint.x() + parallel_segments.xr, lastPoint.y() + parallel_segments.yr);
	this->Road::coreLine = QLineF(firstPoint.x(), firstPoint.y(), lastPoint.x(), lastPoint.y());
	double A = coreLine.dy() / coreLine.dx();
	double B = (coreLine.y1()*coreLine.x2() - coreLine.y2()*coreLine.x1()) / (coreLine.x2() - coreLine.x1());

	//if ( (A == startBermParams.a && upright == startBermParams.upright) || (A == endBermParams.a && upright == endBermParams.upright) )
	//{//dont draw if parallel to any connected line
	//	return false;
	//}

	//check if it is more horizontal or vertical lane
	double dx = (lastPoint.x() - firstPoint.x())> 0 ? (lastPoint.x() - firstPoint.x()) : -(lastPoint.x() - firstPoint.x());
	double dy = (lastPoint.y() - firstPoint.y())> 0 ? (lastPoint.y() - firstPoint.y()) : -(lastPoint.y() - firstPoint.y());
	if (dx > dy)
		horizontal = true;
	else
		horizontal = false;
	//create vectors of the roads
	addPoint(Point(firstPoint.x(), firstPoint.y()), LANE);
	double xm = firstPoint.x() + distance; //one point later than first
	double _x = xm;
	double _y = 0.0;
	
	if (horizontal)
	{
		// y = A * x + B
		double x_last = lastPoint.x();
		if (lastPoint.x() > firstPoint.x())
			for (_x = xm; _x < x_last; _x += distance) {
				addPoint(Point(_x, A*_x + B), LANE);
			}
		else
			for (_x = xm; _x > x_last; _x -= distance) {
				addPoint(Point(_x, A*_x + B), LANE);
			}
	}
	else
	{
		// x = (y - B) / A if dx != 0 else x = point.x
		double ym = firstPoint.y() + distance; //one point later than first
		double yl = firstPoint.y() + parallel_segments.yl;
		double yr = firstPoint.y() + parallel_segments.yr;
		double y_last = lastPoint.y();
		if (lastPoint.y() > firstPoint.y())
			for (_y = ym; _y < y_last; _y += distance) {
				addPoint(Point(dx != 0 ? (_y - B) / A : xm, _y), LANE);
			}
		else
			for (_y = ym; _y > y_last; _y -= distance) {
				addPoint(Point(dx != 0 ? (_y - B) / A : xm, _y), LANE);
			}
	}
	addPoint(Point(lastPoint.x(), lastPoint.y()), LANE);

	QPointF _firstBermLeftPoint(0, 0), _firstBermRightPoint(0, 0), _lastBermLeftPoint(0, 0), _lastBermRightPoint(0, 0);
	if (startJunction != NULL) {
		_firstBermLeftPoint = startJunction->returnCrossPointsForBerm(leftBerm, lastPoint);
		_firstBermRightPoint = startJunction->returnCrossPointsForBerm(rightBerm, lastPoint);
		lane[0].junction = startJunction;
	}
	if (endJunction != NULL) {
		_lastBermLeftPoint = endJunction->returnCrossPointsForBerm(leftBerm, firstPoint);
		_lastBermRightPoint = endJunction->returnCrossPointsForBerm(rightBerm, firstPoint);
		lane[lane.size() - 1].junction = endJunction;
	}
	if (_firstBermLeftPoint.x() != 0) leftBerm.setP1(_firstBermLeftPoint);
	if (_firstBermRightPoint.x() != 0) rightBerm.setP1(_firstBermRightPoint);
	if (_lastBermLeftPoint.x() != 0) leftBerm.setP2(_lastBermLeftPoint);
	if (_lastBermRightPoint.x() != 0) rightBerm.setP2(_lastBermRightPoint);
}

void OneWayOneLane::drawRoad() //poprawiæ na jedn¹ drogê
{
	//fulfill with colour 2 polygons
	glBegin(GL_POLYGON);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(leftBerm.p2().x(), leftBerm.p2().y());
	glVertex2f(leftBerm.p1().x(), leftBerm.p1().y());
	glVertex2f(coreLine.p1().x(), coreLine.p1().y());
	glVertex2f(coreLine.p2().x(), coreLine.p2().y());
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(coreLine.p1().x(), coreLine.p1().y());
	glVertex2f(coreLine.p2().x(), coreLine.p2().y());
	glVertex2f(rightBerm.p2().x(), rightBerm.p2().y());
	glVertex2f(rightBerm.p1().x(), rightBerm.p1().y());
	glEnd();
	glFlush();

	glLineWidth(3);

	//berm right
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(rightBerm.p2().x(), rightBerm.p2().y());
	glVertex2f(rightBerm.p1().x(), rightBerm.p1().y());
	glEnd();

	//berm left
	glBegin(GL_LINES);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2f(leftBerm.p2().x(), leftBerm.p2().y());
	glVertex2f(leftBerm.p1().x(), leftBerm.p1().y());
	glEnd();

	//main lane
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(coreLine.p2().x(), coreLine.p2().y());
	glVertex2f(coreLine.p1().x(), coreLine.p1().y());
	glEnd();
}

Point OneWayOneLane::getPoint(int index, LaneType laneType)
{
	if (laneType == LANE || laneType == MID) {
		if (index < lane.size())
			return lane[index].point;
	}
	else
		return Point(0, 0);
}

int OneWayOneLane::getPointIndex(Point point, LaneType laneType) //if not found returns -1
{
	int index = 0;
	for (auto pointIt = lane.begin(); pointIt < lane.end(); pointIt++) {
		if ((*pointIt).point == point)
			return index;
		index++;
	}
	return -1;
}

int OneWayOneLane::getUsageOfLane(LaneType)
{//only one lane
	return percentageLaneUsage;
}

Point OneWayOneLane::getLastPointOf(LaneType laneType)
{
	switch (laneType) {
	case LANE: return lane[lane.size() - 1].point;
	case MID: return mid[mid.size() - 1];
	}
}

Point OneWayOneLane::getStartPointForConnection(int index, LaneType laneType)
{
	int range = -3;
	for (int i = range; i <= 0; i++) {
		if (index + i < lane.size() - 1 && index + i >= 0) {
			return lane[index + i].point;
		}
	}
	return Point(0, 0);
}

Point OneWayOneLane::getEndPointForConnection(int index, LaneType laneType)
{
	int range = 3;
	for (int i = range; i >= 0; i--) {
		if (index + i < lane.size() - 1 && index + i >= 0) {
			return lane[index + i].point;
		}
	}
	return Point(0, 0);
}

LaneType OneWayOneLane::searchPointForLaneType(Point point)
{
	Point foundPoint;
	foundPoint = searchPoint(point);
	if (foundPoint.x() != 0)
		return LANE;
	else
		return NOTHING;
}

//it is only one lane type in this case; returns distance by pointIdx parameter
void* OneWayOneLane::getNextJunction(LaneType laneType, int &pointIdx)
{
	auto pointIt = lane.begin() + pointIdx;
	while (pointIt < lane.end() - 1) {
		pointIt++;
		if ((*pointIt).junction != NULL) {
			pointIdx = pointIt - (lane.begin() + pointIdx);
			return (*pointIt).junction;
		}
	}
	return NULL;
}

void * OneWayOneLane::getPreviousJunction(LaneType laneType, int &pointIdx)
{
	auto pointIt = lane.begin() + pointIdx;
	while (pointIt > lane.begin()) {
		pointIt--;
		if ((*pointIt).junction != NULL) {
			pointIdx = (lane.begin() + pointIdx) - pointIt;
			return (*pointIt).junction;
		}
	}
	return NULL;
}

void * OneWayOneLane::searchForClosestJunction(Point point, LaneType)
{ //algorithm is searching for possibleJunctions untill it finds exactly ONE junction in a specified range
	void *foundJunction = NULL, *resultJunction = NULL;
	int foundJunctions = 0;
	int range = -40;
	point = searchPoint(point);
	if (point.x() != 0) {
		int pointIdx = getPointIndex(point, LANE);
		while (foundJunction == NULL && range <= 40 && pointIdx != -1) {
			for (int i = range; i <= -range; i++){
					foundJunction = lane[(pointIdx + i >= 0) && (pointIdx + i < lane.size()) ? pointIdx + i : pointIdx].junction;
					if (foundJunction != NULL) { foundJunctions++; resultJunction = foundJunction; }
					foundJunction = NULL;
				}
			if (foundJunctions != 1) //if zero or more than 1 junction found; set it NULL and closer range
				resultJunction = NULL;
			else
				break;
			foundJunctions = 0;
			range++;
		}
	}
	return resultJunction;
}

void OneWayOneLane::addJunction(Point point, void* junction)
{
	for (auto pointIt = lane.begin(); pointIt < lane.end(); pointIt++) {
		if ((*pointIt).point == point)
			(*pointIt).junction = (Junction*)junction;
	}
}

void OneWayOneLane::deleteJunction(void *junction)
{
	for (auto laneIt = lane.begin(); laneIt < lane.end(); laneIt++) {
		if ((*laneIt).junction == junction)
			(*laneIt).junction = NULL;
	}
}

void OneWayOneLane::deleteFromJunctions()
{
	for (auto laneIt = lane.begin(); laneIt < lane.end(); laneIt++) {
		if ((*laneIt).junction != NULL)
			(*laneIt).junction->deleteRoad(this);
	}
}

void OneWayOneLane::freePoint(LaneType, int index)
{
	lane[index].point.setFree();
}

bool OneWayOneLane::reservePoint(LaneType, int index)
{
	return lane[index].point.occupy();
}

void OneWayOneLane::updateLane()
{
	//update lane usage
	percentageLaneUsage = 0;
	for (auto laneIt = lane.begin(); laneIt < lane.end(); laneIt++) {
		if (!(*laneIt).point.isFree())
			percentageLaneUsage++;
	}
	percentageLaneUsage = percentageLaneUsage / lane.size();
}

Point OneWayOneLane::getFirstPointOf(LaneType laneType)
{
	switch (laneType) {
	case LANE: return lane[0].point;
	case MID: return mid[0];
	}
}

vectors OneWayOneLane::calc_vectors(QPointF A, QPointF B)
{
	//int xa = A.x() - B.x();
	//int ya = A.y() - B.y();
	//double xd = pow(odl, 2);
	//double delta = sqrt(1 + 4 * pow(odl, 2) * (pow(ya, 2) / pow(xa, 2) ));
	//double ya_sq = ya ^ 2;
	//return vectors { /*x1*/(1 - delta) / (2 * (pow(ya, 2) / pow(xa, 2))),
	///*y1*/ (-xa * (1 - delta) / (2 * (pow(ya, 2) / pow(xa, 2)))) / ya,
	///*x2*/ (1 + delta) / (2 * (pow(ya, 2) / pow(xa, 2))),
	///*y2*/ (-xa * (1 + delta) / (2 * (pow(ya, 2) / pow(xa, 2)))) / ya };
	double xa = A.x() - B.x();
	double ya = A.y() - B.y();
	double radius = sqrt(xa*xa + ya * ya);
	return vectors{ ya*distance / radius, -xa * distance / radius, -ya * distance / radius, xa * distance / radius };
}