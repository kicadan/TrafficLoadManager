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
	case LANE: { lane.push_back(LanePoint{ point }); break; }
	case LEFT_BERM: { bermL.push_back(point); break; }
	case RIGHT_BERM: bermR.push_back(point);
	}
}

void OneWayOneLane::setRoad(QPointF firstPoint, QPointF lastPoint, bool endingToOtherRoad, Junction* startJunction, Junction* endJunction)//std::vector<Road*> startRoads, std::vector<Road*> endRoads)//LineParams startBermParams, LineParams endBermParams)//, int beginRoad, int endRoad)
{
	this->parallel_segments = calc_vectors(lastPoint, firstPoint);
	double x, y, A, B, bermLeftA, bermLeftB, bermRightA, bermRightB;
	bool upright = false;
	//equation of straight line
	if (!(firstPoint.x() == lastPoint.x()))
	{
		A = (lastPoint.y() - firstPoint.y()) / (double)(lastPoint.x() - firstPoint.x());
		B = firstPoint.y() - A * firstPoint.x();
		bermLeftA = A;
		bermLeftB = firstPoint.y() + parallel_segments.yl - A * (firstPoint.x() + parallel_segments.xl);
		bermRightA = A;
		bermRightB = (firstPoint.y() + parallel_segments.yr) - A * (firstPoint.x() + parallel_segments.xr);
	}
	else
	{
		A = 1;
		B = -firstPoint.x();
		upright = true;
	}
	this->Road::coreLineParams.a = A;
	this->Road::coreLineParams.b = B;
	this->Road::coreLineParams.upright = upright;

	this->Road::bermLParams.a = bermLeftA;
	this->Road::bermLParams.b = bermLeftB;
	this->Road::bermLParams.upright = upright;
	this->Road::bermRParams.a = bermRightA;
	this->Road::bermRParams.b = bermRightB;
	this->Road::bermRParams.upright = upright;
	
	this->Road::leftBerm = QLineF(firstPoint.x() + parallel_segments.xl, firstPoint.y() + parallel_segments.yl, lastPoint.x() + parallel_segments.xl, lastPoint.y() + parallel_segments.yl);
	this->Road::rightBerm = QLineF(firstPoint.x() + parallel_segments.xr, firstPoint.y() + parallel_segments.yr, lastPoint.x() + parallel_segments.xr, lastPoint.y() + parallel_segments.yr);
	this->Road::coreLine = QLineF(firstPoint.x(), firstPoint.y(), lastPoint.x(), lastPoint.y());


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
	addPoint(Point(firstPoint.x() + parallel_segments.xr, firstPoint.y() + parallel_segments.yr), RIGHT_BERM);
	addPoint(Point(firstPoint.x() + parallel_segments.xl, firstPoint.y() + parallel_segments.yl), LEFT_BERM);
	double xm = firstPoint.x() + distance; //one point later than first
	double xl = firstPoint.x() + parallel_segments.xl;
	double xr = firstPoint.x() + parallel_segments.xr;
	double _x = xm;
	double _y = 0.0;
	if (horizontal)
	{
		// y = A * x + B
		double x_last = lastPoint.x();
		if (lastPoint.x() > firstPoint.x())
			for (_x = xm; _x < x_last; _x += distance, xl += distance, xr += distance) {
				addPoint(Point(_x, A*_x + B), LANE);
				addPoint(Point(xl, A*xl + B), LEFT_BERM);
				addPoint(Point(xr, A*xr + B), RIGHT_BERM);
			}
		else
			for (_x = xm; _x > x_last; _x -= distance, xl -= distance, xr -= distance) {
				addPoint(Point(_x, A*_x + B), LANE);
				addPoint(Point(xl, A*xl + B), LEFT_BERM);
				addPoint(Point(xr, A*xr + B), RIGHT_BERM);
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
			for (_y = ym; _y < y_last; _y += distance, yl += distance, yr += distance) {
				addPoint(Point(dx != 0 ? (_y - B) / A : xm, _y), LANE);
				addPoint(Point(dx != 0 ? (yl - B) / A : xl, yl), LEFT_BERM); //deprecated
				addPoint(Point(dx != 0 ? (yr - B) / A : xr, yr), RIGHT_BERM); //deprecated
			}
		else
			for (_y = ym; _y > y_last; _y -= distance, yl -= distance, yr -= distance) {
				addPoint(Point(dx != 0 ? (_y - B) / A : xm, _y), LANE);
				addPoint(Point(dx != 0 ? (yl - B) / A : xl, yl), LEFT_BERM); //deprecated
				addPoint(Point(dx != 0 ? (yr - B) / A : xr, yr), RIGHT_BERM); //deprecated
			}
	}
	addPoint(Point(lastPoint.x(), lastPoint.y()), LANE);
	addPoint(Point(lastPoint.x() + parallel_segments.xr, lastPoint.y() + parallel_segments.yr), RIGHT_BERM);
	addPoint(Point(lastPoint.x() + parallel_segments.xl, lastPoint.y() + parallel_segments.yl), LEFT_BERM);

	//assing real values FROM VECTOR to _lastPoint if road is not connecting to the other at its end
	if (!endingToOtherRoad) {
		if (_y == 0)
		{
			if (lastPoint.x() > firstPoint.x())
				_x -= distance;
			else
				_x += distance;
			_y = A * _x + B;
		}
		else
		{
			if (lastPoint.y() > firstPoint.y())
				_y -= distance;
			else
				_y += distance;
			_x = dx != 0 ? (_y - B) / A : xm;
		}
		lastPoint = QPoint(_x, _y);
	}

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
	//main lane's points
	coreLineParams.firstPoint = Point(firstPoint.x(), firstPoint.y());
	coreLineParams.lastPoint = Point(lastPoint.x(), lastPoint.y());
	//left berm's points
	x = _firstBermLeftPoint.x() == 0 ? firstPoint.x() + parallel_segments.xl : _firstBermLeftPoint.x();
	y = _firstBermLeftPoint.y() == 0 ? firstPoint.y() + parallel_segments.yl : _firstBermLeftPoint.y();
	bermLParams.firstPoint = Point(x, y);
	x = _lastBermLeftPoint.x() == 0 ? lastPoint.x() + parallel_segments.xl : _lastBermLeftPoint.x();
	y = _lastBermLeftPoint.y() == 0 ? lastPoint.y() + parallel_segments.yl : _lastBermLeftPoint.y();
	bermLParams.lastPoint = Point(x, y);
	//right berm's points
	x = _lastBermRightPoint.x() == 0 ? lastPoint.x() + parallel_segments.xr : _lastBermRightPoint.x();
	y = _lastBermRightPoint.y() == 0 ? lastPoint.y() + parallel_segments.yr : _lastBermRightPoint.y();
	bermRParams.lastPoint = Point(x, y);
	x = _firstBermRightPoint.x() == 0 ? firstPoint.x() + parallel_segments.xr : _firstBermRightPoint.x();
	y = _firstBermRightPoint.y() == 0 ? firstPoint.y() + parallel_segments.yr : _firstBermRightPoint.y();
	bermRParams.firstPoint = Point(x, y);

	//assign lane to core of the road
	copyLanePointVectorToPointVector(lane, mid);
}

void OneWayOneLane::drawRoad()
{
	//fulfill with colour 2 polygons
	/*glBegin(GL_POLYGON);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(bermLParams._lastPoint.x(), bermLParams._lastPoint.y());
	glVertex2f(bermLParams._firstPoint.x(), bermLParams._firstPoint.y());
	glVertex2f(coreLineParams._firstPoint.x(), coreLineParams._firstPoint.y());
	glVertex2f(coreLineParams._lastPoint.x(), coreLineParams._lastPoint.y());
	glEnd;

	glBegin(GL_POLYGON);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(coreLineParams._lastPoint.x(), coreLineParams._lastPoint.y());
	glVertex2f(coreLineParams._firstPoint.x(), coreLineParams._firstPoint.y());
	glVertex2f(bermRParams._firstPoint.x(), bermRParams._firstPoint.y());
	glVertex2f(bermRParams._lastPoint.x(), bermRParams._lastPoint.y());
	glEnd;*/

	glLineWidth(3);

	//main lane
	glBegin(GL_LINES); 
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(coreLineParams.lastPoint.x(), coreLineParams.lastPoint.y());
	glVertex2f(coreLineParams.firstPoint.x(), coreLineParams.firstPoint.y());
	glEnd();

	//berm left
	glBegin(GL_LINES);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2f(bermLParams.lastPoint.x(), bermLParams.lastPoint.y());
	glVertex2f(bermLParams.firstPoint.x(), bermLParams.firstPoint.y());
	glEnd();

	//berm right
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(bermRParams.lastPoint.x(), bermRParams.lastPoint.y());
	glVertex2f(bermRParams.firstPoint.x(), bermRParams.firstPoint.y());
	glEnd();
}

Point OneWayOneLane::getPoint(int index, LaneType laneType)
{
	if (laneType == LANE) {
		if (index < lane.size())
			return lane[index].point;
	}
	else
		return Point(0, 0);
}

int OneWayOneLane::getPointIndex(Point point) //if not found returns -1
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
	return percentageLaneUsage = 0;
}

Point OneWayOneLane::getLastPointOf(LaneType laneType)
{
	switch (laneType) {
	case LANE: return lane[lane.size() - 1].point;
	}
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

void OneWayOneLane::addJunction(Point point, LaneType laneType, void* junction)
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