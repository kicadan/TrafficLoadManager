#include "Junction.h"



Junction::Junction():AppObject(JUNCTION)
{
}

Junction::Junction(Point _point, Road* firstRoad, Road* secondRoad): AppObject(JUNCTION)
{
	this->point = _point;
	roads.push_back(firstRoad);
	roads.push_back(secondRoad);
	roadIds.push_back(firstRoad->id);
	roadIds.push_back(secondRoad->id);
}


Junction::~Junction()
{
}

Point Junction::returnCrossPointsForBerm(LineParams lineParams, Point oppositePoint)
{
	std::vector<Point> points;
	LineParams rightBermParams;
	LineParams leftBermParams;
	Point point = Point(0, 0);
	double x, y;
	for (auto road : this->roads) {
		LineParams leftBermParams = road->getLineParams(LEFT_BERM);
		LineParams rightBermParams = road->getLineParams(RIGHT_BERM);
			//left point
			if (!lineParams.upright && !leftBermParams.upright && leftBermParams.a != lineParams.a) { //first and second line are not parallel and not upright
				x = (leftBermParams.b - lineParams.b) / (lineParams.a - leftBermParams.a);
				y = x * leftBermParams.a + leftBermParams.b;
				if (!(x >= min(leftBermParams.firstPoint.x(), leftBermParams.lastPoint.x()) - 1 && x <= max(leftBermParams.firstPoint.x(), leftBermParams.lastPoint.x()) + 1 && y >= min(leftBermParams.firstPoint.y(), leftBermParams.lastPoint.y()) - 1 && y <= max(leftBermParams.firstPoint.y(), leftBermParams.lastPoint.y()) + 1)) { //if not on short
					x = 0;
					y = 0;
				}
			}
			else if (lineParams.upright) {
				x = (leftBermParams.b - lineParams.b) / (lineParams.a - leftBermParams.a);
				y = x * leftBermParams.a + leftBermParams.b;
				if (!(x >= min(leftBermParams.firstPoint.x(), leftBermParams.lastPoint.x()) - 1 && x <= max(leftBermParams.firstPoint.x(), leftBermParams.lastPoint.x()) + 1 && y >= min(leftBermParams.firstPoint.y(), leftBermParams.lastPoint.y()) - 1 && y <= max(leftBermParams.firstPoint.y(), leftBermParams.lastPoint.y()) + 1)) { //if not on short
					x = 0;
					y = 0;
				}
			}
			else if (leftBermParams.upright) {
				x = (leftBermParams.b - lineParams.b) / (lineParams.a - leftBermParams.a);
				y = x * lineParams.a + lineParams.b;
				if (!(x >= min(leftBermParams.firstPoint.x(), leftBermParams.lastPoint.x()) - 1 && x <= max(leftBermParams.firstPoint.x(), leftBermParams.lastPoint.x()) + 1 && y >= min(leftBermParams.firstPoint.y(), leftBermParams.lastPoint.y()) - 1 && y <= max(leftBermParams.firstPoint.y(), leftBermParams.lastPoint.y()) + 1)) { //if not on short
					x = 0;
					y = 0;
				}
			}
			else { //lines are parallel
				continue;
			}
			if (x != 0 && y != 0)
				points.push_back(Point(x, y));

			//right point
			if (!lineParams.upright && !rightBermParams.upright && rightBermParams.a != lineParams.a) { //first and second line are not parallel and not upright
				x = (rightBermParams.b - lineParams.b) / (lineParams.a - rightBermParams.a);
				y = x * rightBermParams.a + rightBermParams.b;
				if (!(x >= min(rightBermParams.firstPoint.x(), rightBermParams.lastPoint.x()) - 1 && x <= max(rightBermParams.firstPoint.x(), rightBermParams.lastPoint.x()) + 1 && y >= min(rightBermParams.firstPoint.y(), rightBermParams.lastPoint.y()) - 1 && y <= max(rightBermParams.firstPoint.y(), rightBermParams.lastPoint.y()) + 1)) { //if not on short
					x = 0;
					y = 0;
				}
			}
			else if (lineParams.upright) {
				x = (rightBermParams.b - lineParams.b) / (lineParams.a - rightBermParams.a);
				y = x * rightBermParams.a + rightBermParams.b;
				if (!(x >= min(rightBermParams.firstPoint.x(), rightBermParams.lastPoint.x()) - 1 && x <= max(rightBermParams.firstPoint.x(), rightBermParams.lastPoint.x()) + 1 && y >= min(rightBermParams.firstPoint.y(), rightBermParams.lastPoint.y()) - 1 && y <= max(rightBermParams.firstPoint.y(), rightBermParams.lastPoint.y()) + 1)) { //if not on short
					x = 0;
					y = 0;
				}
			}
			else if (rightBermParams.upright) {
				x = (rightBermParams.b - lineParams.b) / (lineParams.a - rightBermParams.a);
				y = x * lineParams.a + lineParams.b;
				if (!(x >= min(rightBermParams.firstPoint.x(), rightBermParams.lastPoint.x()) - 1 && x <= max(rightBermParams.firstPoint.x(), rightBermParams.lastPoint.x()) + 1 && y >= min(rightBermParams.firstPoint.y(), rightBermParams.lastPoint.y()) - 1 && y <= max(rightBermParams.firstPoint.y(), rightBermParams.lastPoint.y()) + 1)) { //if not on short
					x = 0;
					y = 0;
				}
			}
			else { //lines are parallel
				continue;
			}
			if (x != 0 && y != 0)
				points.push_back(Point(x, y));
	}
	double minimumDistance = 9999999.0, minimumRightDistance = 9999999.0;
	double dist;
	for (auto _point : points) {
		dist = sqrt(pow(oppositePoint.x() - _point.x(), 2) + pow(oppositePoint.y() - _point.y(), 2));
		if (dist < minimumDistance) {
			minimumDistance = dist;
			point = _point;
		}
	}
	return point;
}

void Junction::addRoad(Road *road)
{
	roads.push_back(road);
	roadIds.push_back(road->id);
}

void Junction::deleteRoad(Road * deletedRoad)
{
	for (auto roadIt = roadIds.begin(); roadIt <= roadIds.end(); roadIt++)
		if ((*roadIt) == deletedRoad->id) {
			roadIds.erase(roadIt);
			break;
		}
	for (auto roadIt = roads.begin(); roadIt <= roads.end(); roadIt++)
		if ((*roadIt)->id == deletedRoad->id) {
			roads.erase(roadIt);
			break;
		}
}

bool Junction::isPoint(QPoint point)
{
	return this->point == point;
}

bool Junction::isPoint(Point point)
{
	return this->point == point;
}

int Junction::numberOfRoads()
{
	return roadIds.size();
}

ObjectType Junction::getObjectType()
{
	return _objectType;
}

std::vector<int> Junction::getRoadIds()
{
	return roadIds;
}

void Junction::makeConnections()
{
	connections.clear();
	for (auto roadIt = roads.begin(); roadIt < roads.end(); roadIt++) {
		connectRoad(*roadIt);
	}
}

void Junction::connectRoad(Road* connectingRoad)
{
	if (connectingRoad->getRoadType() == OneWayRoadWithOneLane)
		connectOneWayOneLane(connectingRoad);
}

//function used only while deleting junction and one road left from it
void Junction::forgetAboutMe(int notThisRoad)
{
	roads[0]->deleteJunction(this);
}

void Junction::connectOneWayOneLane(Road *newRoad)
{
	Connection newConnection;
	int pointIndexOnTheRoad = newRoad->getPointIndex(point);
	newConnection.nextLaneType = LANE;
	newConnection.nextRoadId = newRoad->id;
	newConnection.direction = 1;
	newConnection.nextPoint = point;
	if (pointIndexOnTheRoad > -1) {
		newConnection.nextJunction = (Junction*)newRoad->getNextJunction(LANE, pointIndexOnTheRoad);
		newConnection.distanceToNextJunction = pointIndexOnTheRoad;
	}
	if(newConnection.nextJunction != NULL)
		connections.push_back(newConnection);
}

void Junction::connectFromOneWayOneLane(Road *)
{
}

void Junction::connectToOneWayOneLane(Road *)
{
}

void copyLanePointVectorToPointVector(std::vector<LanePoint> vectorFrom, std::vector<Point> &vectorTo)
{
	for (auto vectorFromIt = vectorFrom.begin(); vectorFromIt < vectorFrom.end(); vectorFromIt++)
		vectorTo.push_back((*vectorFromIt).point);
}
