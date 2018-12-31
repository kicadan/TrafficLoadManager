#include "OneWayTwoLanes.h"



OneWayTwoLanes::OneWayTwoLanes(int _id) :Road(OneWayRoadWithTwoLanes)
{
	this->Road::id = _id;
}


OneWayTwoLanes::~OneWayTwoLanes()
{
}

void OneWayTwoLanes::addPoint(Point point, LaneType pointType)
{
	switch (pointType) {
	case LEFT_LANE: { leftLane.push_back(LanePoint{ point }); break; }
	case RIGHT_LANE: { rightLane.push_back(LanePoint{ point }); break; }
	case MID: { mid.push_back(point); break; }
	case RIGHT_BERM: { rightBermVector.push_back(Point(point.x() + 2 * parallel_segments.xr, point.y() + 2 * parallel_segments.yr)); break; }
	case LEFT_BERM: { leftBermVector.push_back(Point(point.x() + 2 * parallel_segments.xl, point.y() + 2 * parallel_segments.yl)); break; }
	}
}

void OneWayTwoLanes::setRoad(QPointF firstPoint, QPointF lastPoint, Junction* startJunction, Junction* endJunction)
{
	this->parallel_segments = calc_vectors(lastPoint, firstPoint);

	this->Road::leftBerm = QLineF(firstPoint.x() + 2*parallel_segments.xl, firstPoint.y() + 2*parallel_segments.yl, lastPoint.x() + 2*parallel_segments.xl, lastPoint.y() + 2*parallel_segments.yl);
	this->Road::rightBerm = QLineF(firstPoint.x() + 2*parallel_segments.xr, firstPoint.y() + 2*parallel_segments.yr, lastPoint.x() + 2*parallel_segments.xr, lastPoint.y() + 2*parallel_segments.yr);
	this->Road::coreLine = QLineF(firstPoint.x(), firstPoint.y(), lastPoint.x(), lastPoint.y());
	double A = coreLine.dy() / coreLine.dx();
	double Bm = (coreLine.y1()*coreLine.x2() - coreLine.y2()*coreLine.x1()) / (coreLine.x2() - coreLine.x1());
	double Bl = ((coreLine.y1() + parallel_segments.yl)*(coreLine.x2() + parallel_segments.xl) - (coreLine.y2() + parallel_segments.yl)*(coreLine.x1() + parallel_segments.xl)) / (coreLine.x2() - coreLine.x1());
	double Br = ((coreLine.y1() + parallel_segments.yr)*(coreLine.x2() + parallel_segments.xr) - (coreLine.y2() + parallel_segments.yr)*(coreLine.x1() + parallel_segments.xr)) / (coreLine.x2() - coreLine.x1());

	//check if it is more horizontal or vertical lane
	double dx = (lastPoint.x() - firstPoint.x())> 0 ? (lastPoint.x() - firstPoint.x()) : -(lastPoint.x() - firstPoint.x());
	double dy = (lastPoint.y() - firstPoint.y())> 0 ? (lastPoint.y() - firstPoint.y()) : -(lastPoint.y() - firstPoint.y());
	if (dx > dy)
		horizontal = true;
	else
		horizontal = false;
	//create vectors of the roads
	addPoint(Point(firstPoint.x() + parallel_segments.xl, firstPoint.y() + parallel_segments.yl), LEFT_LANE);
	addPoint(Point(firstPoint.x() + parallel_segments.xr, firstPoint.y() + parallel_segments.yr), RIGHT_LANE);
	addPoint(Point(firstPoint.x(), firstPoint.y()), RIGHT_BERM);
	addPoint(Point(firstPoint.x(), firstPoint.y()), LEFT_BERM);
	addPoint(Point(firstPoint.x(), firstPoint.y()), MID);
	double xl = firstPoint.x() + parallel_segments.xl;
	double xr = firstPoint.x() + parallel_segments.xr;
	double xm = firstPoint.x();
	double yl, yr, ym;

	if (horizontal)
	{
		// y = A * x + B
		double x_last = lastPoint.x() + parallel_segments.xl;
		if (lastPoint.x() > firstPoint.x()) {
			xl += distance; //one point later than first
			xr += distance; //one point later than first
			xm += distance; //one point later than first
			for (xl; xl < x_last; xm += distance, xl += distance, xr += distance) {
				addPoint(Point(xl, A*xl + Bl), LEFT_LANE);
				addPoint(Point(xr, A*xr + Br), RIGHT_LANE);
				addPoint(Point(xm, A*xm + Bm), MID);
				addPoint(Point(xm, A*xm + Bm), RIGHT_BERM);
				addPoint(Point(xm, A*xm + Bm), LEFT_BERM);
			}
		}
		else {
			xl -= distance; //one point later than first
			xr -= distance; //one point later than first
			xm -= distance; //one point later than first
			for (xl; xl > x_last; xm -= distance, xl -= distance, xr -= distance) {
				addPoint(Point(xl, A*xl + Bl), LEFT_LANE);
				addPoint(Point(xr, A*xr + Br), RIGHT_LANE);
				addPoint(Point(xm, A*xm + Bm), MID);
				addPoint(Point(xm, A*xm + Bm), RIGHT_BERM);
				addPoint(Point(xm, A*xm + Bm), LEFT_BERM);
			}
		}
	}
	else
	{
		// x = (y - B) / A if dx != 0 else x = point.x
		double y_last = lastPoint.y() + parallel_segments.yl;
		if (lastPoint.y() > firstPoint.y()) {
			yl = firstPoint.y() + parallel_segments.yl + distance; //one point later than first;
			yr = firstPoint.y() + parallel_segments.yr + distance; //one point later than first;
			ym = firstPoint.y() + distance;
			for (yl; yl < y_last; ym += distance, yl += distance, yr += distance) {
				addPoint(Point(dx != 0 ? (yl - Bl) / A : xl, yl), LEFT_LANE);
				addPoint(Point(dx != 0 ? (yr - Br) / A : xr, yr), RIGHT_LANE);
				addPoint(Point(dx != 0 ? (ym - Bm) / A : xm, ym), MID);
				addPoint(Point(dx != 0 ? (ym - Bm) / A : xm, ym), RIGHT_BERM);
				addPoint(Point(dx != 0 ? (ym - Bm) / A : xm, ym), LEFT_BERM);
			}
		}
		else {
			yl = firstPoint.y() + parallel_segments.yl - distance; //one point later than first;
			yr = firstPoint.y() + parallel_segments.yr - distance; //one point later than first;
			ym = firstPoint.y() - distance;
			for (yl; yl > y_last; ym -= distance, yl -= distance, yr -= distance) {
				addPoint(Point(dx != 0 ? (yl - Bl) / A : xl, yl), LEFT_LANE);
				addPoint(Point(dx != 0 ? (yr - Br) / A : xr, yr), RIGHT_LANE);
				addPoint(Point(dx != 0 ? (ym - Bm) / A : xm, ym), MID);
				addPoint(Point(dx != 0 ? (ym - Bm) / A : xm, ym), RIGHT_BERM);
				addPoint(Point(dx != 0 ? (ym - Bm) / A : xm, ym), LEFT_BERM);
			}
		}
	}
	addPoint(Point(lastPoint.x() + parallel_segments.xl, lastPoint.y() + parallel_segments.yl), LEFT_LANE);
	addPoint(Point(lastPoint.x() + parallel_segments.xr, lastPoint.y() + parallel_segments.yr), RIGHT_LANE);
	addPoint(Point(lastPoint.x(), lastPoint.y()), MID);
	addPoint(Point(lastPoint.x(), lastPoint.y()), RIGHT_BERM);
	addPoint(Point(lastPoint.x(), lastPoint.y()), LEFT_BERM);

	QPointF _firstBermLeftPoint(0, 0), _firstBermRightPoint(0, 0), _lastBermLeftPoint(0, 0), _lastBermRightPoint(0, 0);
	if (startJunction != NULL) {
		_firstBermLeftPoint = startJunction->returnCrossPointsForBerm(leftBerm, lastPoint);
		_firstBermRightPoint = startJunction->returnCrossPointsForBerm(rightBerm, lastPoint);
		leftLane[0].junction = startJunction;
		rightLane[0].junction = startJunction;
	}
	if (endJunction != NULL) {
		_lastBermLeftPoint = endJunction->returnCrossPointsForBerm(leftBerm, firstPoint);
		_lastBermRightPoint = endJunction->returnCrossPointsForBerm(rightBerm, firstPoint);
		leftLane[leftLane.size() - 1].junction = endJunction;
		rightLane[rightLane.size() - 1].junction = endJunction;
	}
	if (_firstBermLeftPoint.x() != 0) leftBerm.setP1(_firstBermLeftPoint);
	if (_firstBermRightPoint.x() != 0) rightBerm.setP1(_firstBermRightPoint);
	if (_lastBermLeftPoint.x() != 0) leftBerm.setP2(_lastBermLeftPoint);
	if (_lastBermRightPoint.x() != 0) rightBerm.setP2(_lastBermRightPoint);
}

void OneWayTwoLanes::drawRoad() //poprawiæ na dwie drogi
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

	//main lane
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex2f(coreLine.p2().x(), coreLine.p2().y());
	glVertex2f(coreLine.p1().x(), coreLine.p1().y());
	glEnd();

	//berm left
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex2f(leftBerm.p2().x(), leftBerm.p2().y());
	glVertex2f(leftBerm.p1().x(), leftBerm.p1().y());
	glEnd();

	//berm right
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex2f(rightBerm.p2().x(), rightBerm.p2().y());
	glVertex2f(rightBerm.p1().x(), rightBerm.p1().y());
	glEnd();
	glFlush();
}

Point OneWayTwoLanes::getPoint(int index, LaneType laneType)
{
	Point point;
	if (laneType == LEFT_LANE) {
		if (index < leftLane.size())
			point = leftLane[index].point;
	}
	else if (laneType == RIGHT_LANE) {
		if (index < rightLane.size())
			point = rightLane[index].point;
	}
	else if (laneType == MID) {
		if (index < mid.size())
			point = mid[index];
	}
	else if (laneType == RIGHT_BERM) {
		if (index < rightBermVector.size())
			point = rightBermVector[index];
	}
	else if (laneType == LEFT_BERM) {
		if (index < leftBermVector.size())
			point = leftBermVector[index];
	}
	else
		point = Point(0, 0);
	return point;
}

int OneWayTwoLanes::getPointIndex(Point point, LaneType laneType) //if not found returns -1
{
	int index = 0;
	if(laneType == LEFT_LANE)
		for (auto pointIt = leftLane.begin(); pointIt < leftLane.end(); pointIt++) {
			if ((*pointIt).point == point)
				return index;
			index++;
		}
	else if (laneType == RIGHT_LANE)
		for (auto pointIt = rightLane.begin(); pointIt < rightLane.end(); pointIt++) {
			if ((*pointIt).point == point)
				return index;
			index++;
		}
	else if (laneType == MID)
		for (auto pointIt = mid.begin(); pointIt < mid.end(); pointIt++) {
			if ((*pointIt) == point)
				return index;
			index++;
		}
	return -1;
}

int OneWayTwoLanes::getUsageOfLane(LaneType laneType)
{//only one lane
	int usage = 0;
	if (laneType == LEFT_LANE)
		usage = percentageLeftLaneUsage;
	else if (laneType == RIGHT_LANE)
		usage = percentageRightLaneUsage;
	return usage;
}

Point OneWayTwoLanes::getLastPointOf(LaneType laneType)
{
	switch (laneType) {
	case LEFT_LANE: return leftLane[leftLane.size() - 1].point;
	case RIGHT_LANE: return rightLane[rightLane.size() - 1].point;
	case MID: return mid[mid.size() - 1];
	case RIGHT_BERM: return rightBermVector[rightBermVector.size() - 1];
	case LEFT_BERM: return leftBermVector[leftBermVector.size() - 1];
	}
}

Point OneWayTwoLanes::getStartPointForConnection(int index, LaneType laneType)
{
	if (laneType == RIGHT_LANE) {
		int range = -3;
		for (int i = range; i <= 0; i++) {
			if (index + i < rightLane.size() - 1 && index + i >= 0) {
				return rightLane[index + i].point;
			}
		}
	}
	else if (laneType == LEFT_LANE) {
		int range = -3;
		for (int i = range; i <= 0; i++) {
			if (index + i < leftLane.size() - 1 && index + i >= 0) {
				return leftLane[index + i].point;
			}
		}
	}
	return Point(0, 0);
}

Point OneWayTwoLanes::getEndPointForConnection(int index, LaneType laneType)
{
	if (laneType == RIGHT_LANE) {
		int range = 3;
		for (int i = range; i >= 0; i--) {
			if (index + range < rightLane.size() - 1 && index + range >= 0) {
				return rightLane[index + range].point;
			}
		}
	}
	else if (laneType == LEFT_LANE) {
		int range = 3;
		for (int i = range; i >= 0; i--) {
			if (index + range < leftLane.size() - 1 && index + range >= 0) {
				return leftLane[index + range].point;
			}
		}
	}
	return Point(0, 0);
}

LaneType OneWayTwoLanes::searchPointForLaneType(Point point)
{
	LaneType foundLaneType;
	for (auto &existingPoint : leftLane) {
		if ((point.x() > floor(existingPoint.point.x() - distance / 2) && point.x() < ceil(existingPoint.point.x() + distance / 2)) && (point.y() > floor(existingPoint.point.y() - distance / 2) && point.y() < ceil(existingPoint.point.y() + distance / 2)))
			return LEFT_LANE;
	}
	for (auto &existingPoint : rightLane) {
		if ((point.x() > floor(existingPoint.point.x() - distance / 2) && point.x() < ceil(existingPoint.point.x() + distance / 2)) && (point.y() > floor(existingPoint.point.y() - distance / 2) && point.y() < ceil(existingPoint.point.y() + distance / 2)))
			return RIGHT_LANE;
	}
	return NOTHING;
}

void* OneWayTwoLanes::getNextJunction(LaneType laneType, int &pointIdx)
{
	bool found = false;
	Junction* junction;
	if (laneType == LEFT_LANE) {
		auto pointIt = leftLane.begin() + pointIdx;
		while (pointIt < leftLane.end() - 1 && !found) {
			pointIt++;
			if ((*pointIt).junction != NULL) {
				pointIdx = pointIt - (leftLane.begin() + pointIdx);
				found = true;
				junction = (*pointIt).junction;
				break;
			}
		}
	}
	else if (laneType == RIGHT_LANE) {
		auto pointIt = rightLane.begin() + pointIdx;
		while (pointIt < rightLane.end() - 1 && !found) {
			pointIt++;
			if ((*pointIt).junction != NULL) {
				pointIdx = pointIt - (rightLane.begin() + pointIdx);
				found = true;
				junction = (*pointIt).junction;
				break;
			}
		}
	}
	return found ? junction : NULL;
}

void * OneWayTwoLanes::getPreviousJunction(LaneType laneType, int &pointIdx)
{
	bool found = false;
	Junction* junction;
	if (laneType == LEFT_LANE) {
		auto pointIt = leftLane.begin() + pointIdx;
		while (pointIt > leftLane.begin() && !found) {
			pointIt--;
			if ((*pointIt).junction != NULL) {
				pointIdx = (leftLane.begin() + pointIdx) - pointIt;
				found = true;
				junction = (*pointIt).junction;
				break;
			}
		}
	}
	else if (laneType == RIGHT_LANE) {
		auto pointIt = rightLane.begin() + pointIdx;
		while (pointIt > rightLane.begin() && !found) {
			pointIt--;
			if ((*pointIt).junction != NULL) {
				pointIdx = (rightLane.begin() + pointIdx) - pointIt;
				found = true;
				junction = (*pointIt).junction;
				break;
			}
		}
	}
	return found ? junction : NULL;
}

void * OneWayTwoLanes::searchForClosestJunction(Point point, LaneType laneType)
{ //algorithm is searching for possibleJunctions untill it finds exactly ONE junction in a specified range
	Point foundPoint(0, 0);
	if (laneType == LEFT_LANE) {
		for (auto &existingPoint : leftLane) {
			if ((point.x() > floor(existingPoint.point.x() - distance / 2) && point.x() < ceil(existingPoint.point.x() + distance / 2)) && (point.y() > floor(existingPoint.point.y() - distance / 2) && point.y() < ceil(existingPoint.point.y() + distance / 2))) {
				foundPoint = existingPoint.point;
				break;
			}
		}
	}
	else if (laneType == RIGHT_LANE) {
		for (auto &existingPoint : rightLane) {
			if ((point.x() > floor(existingPoint.point.x() - distance / 2) && point.x() < ceil(existingPoint.point.x() + distance / 2)) && (point.y() > floor(existingPoint.point.y() - distance / 2) && point.y() < ceil(existingPoint.point.y() + distance / 2))) {
				foundPoint = existingPoint.point;
				break;
			}
		}
	}
	int pointIdx = -1;
	if (foundPoint.x() == 0)
		//if not found, then search on mid
		pointIdx = getPointIndex(point, MID);
	else
		pointIdx = getPointIndex(foundPoint, laneType);
	void *foundJunction = NULL, *resultJunction = NULL;
	if (pointIdx != -1) {
		int foundJunctions = 0;
		int range = -40;
		if (point.x() != 0) {
			while (foundJunction == NULL && range <= 40 && pointIdx != -1) {
				for (int i = range; i <= -range; i++) {
					if (laneType == LEFT_LANE) foundJunction = leftLane[(pointIdx + i >= 0) && (pointIdx + i < leftLane.size()) ? pointIdx + i : pointIdx].junction;
					if (laneType == RIGHT_LANE) foundJunction = rightLane[(pointIdx + i >= 0) && (pointIdx + i < rightLane.size()) ? pointIdx + i : pointIdx].junction;
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
	}
	return resultJunction;
}

void OneWayTwoLanes::addJunction(Point point, void* junction)
{
	int idx = getPointIndex(point, MID);
	rightLane[idx].junction = (Junction*)junction;
	leftLane[idx].junction = (Junction*)junction;
}

void OneWayTwoLanes::deleteJunction(void *junction)
{
	//right and left are same length
	auto leftLaneIt = leftLane.begin();
	auto rightLaneIt = rightLane.begin();
	while (leftLaneIt < leftLane.end() && rightLaneIt < rightLane.end()) {
		if ((*leftLaneIt).junction == junction)
			(*leftLaneIt).junction = NULL;
		if ((*rightLaneIt).junction == junction)
			(*rightLaneIt).junction = NULL;
		leftLaneIt++;
		rightLaneIt++;
	}
}

void OneWayTwoLanes::deleteFromJunctions()
{
	//right and left are same length
	auto rightLaneIt = rightLane.begin();
	for (auto leftLaneIt = leftLane.begin(); leftLaneIt < leftLane.end(); leftLaneIt++, rightLaneIt++) {
		if ((*leftLaneIt).junction != NULL)
			(*leftLaneIt).junction->deleteRoad(this);
		if ((*rightLaneIt).junction != NULL)
			(*rightLaneIt).junction->deleteRoad(this);
	}
}

void OneWayTwoLanes::processAllVehicles(std::vector<Vehicle*> &alreadyProcessed/*alreadyProcessed contains vehicles which just turned and changed road*/)
{
	Point actualPoint;
	Vehicle* vehicle;
	bool processed, turned;
	auto rightLaneIt = rightLane.end(); //same length as leftLane
	auto laneIt = leftLane.end(); 
	while ( laneIt > leftLane.begin() && rightLaneIt > rightLane.begin()) {
		laneIt--; 
		rightLaneIt--;
		actualPoint = (*laneIt).point;
		processed = false;
		turned = false;
		if (!actualPoint.isFree()) {
			vehicle = actualPoint.getVehicle();
			for (auto vehiclesIt = alreadyProcessed.begin(); vehiclesIt < alreadyProcessed.end(); vehiclesIt++) {
				if ((*vehiclesIt) == vehicle) {
					processed = true;
					break;
				}
			}
			if (!processed) turned = vehicle->continueJourney();
			if (turned) alreadyProcessed.push_back(vehicle);
		}
		processed = false;
		turned = false;
		actualPoint = (*rightLaneIt).point;
		if (!actualPoint.isFree()) {
			vehicle = actualPoint.getVehicle();
			for (auto vehiclesIt = alreadyProcessed.begin(); vehiclesIt < alreadyProcessed.end(); vehiclesIt++) {
				if ((*vehiclesIt) == vehicle) {
					processed = true;
					break;
				}
			}
			if(!processed) turned = vehicle->continueJourney();
			if (turned) alreadyProcessed.push_back(vehicle);
		}
	}
}

void OneWayTwoLanes::freePoint(LaneType laneType, int index)
{
	if (laneType == LEFT_LANE)
		leftLane[index].point.setFree();
	else if (laneType == RIGHT_LANE)
		rightLane[index].point.setFree();
}

bool OneWayTwoLanes::reservePoint(LaneType laneType, Vehicle* vehicle, int index)
{
	bool success = false;
	if (laneType == LEFT_LANE)
		success = leftLane[index].point.occupy(vehicle);
	else if (laneType == RIGHT_LANE)
		success = rightLane[index].point.occupy(vehicle);
	return success;
}

void OneWayTwoLanes::updateLane()
{
	//update lane usage
	percentageLeftLaneUsage = 0;
	percentageRightLaneUsage = 0;
	auto rightLaneIt = rightLane.begin();
	for (auto leftLaneIt = leftLane.begin(); leftLaneIt < leftLane.end(); leftLaneIt++, rightLaneIt++) {
		if (!(*leftLaneIt).point.isFree())
			percentageLeftLaneUsage++;
		if (!(*rightLaneIt).point.isFree())
			percentageRightLaneUsage++;
	}
	percentageLeftLaneUsage = (percentageLeftLaneUsage * 100) / leftLane.size();
	percentageRightLaneUsage = (percentageRightLaneUsage * 100) / rightLane.size();
}

Point OneWayTwoLanes::getFirstPointOf(LaneType laneType)
{
	switch (laneType) {
	case LEFT_LANE: return leftLane[0].point;
	case RIGHT_LANE: return rightLane[0].point;
	case MID: return mid[0];
	case RIGHT_BERM: return rightBermVector[0];
	case LEFT_BERM: return leftBermVector[0];
	}
}

vectors OneWayTwoLanes::calc_vectors(QPointF A, QPointF B)
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