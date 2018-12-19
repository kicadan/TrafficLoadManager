#include "Junction.h"



Junction::Junction():AppObject(JUNCTION)
{
}

Junction::Junction(int id) : AppObject(JUNCTION), id(id)
{
}

Junction::Junction(Point _point, Road* mainRoad, int id): AppObject(JUNCTION)
{
	this->point = _point;
	this->id = id;
	roads.push_back(ConnectedRoad{ LANE, mainRoad }); //main actualRoad could be lane, the rest have right or left berm param
	roadIds.push_back(mainRoad->id);
}


Junction::~Junction()
{
}

QPointF Junction::returnCrossPointsForBerm(QLineF theLine, QPointF oppositePoint)
{
	QLineF::IntersectType intersectType;
	std::vector<QPointF> points;
	QPointF crossPoint(0,0);
	for (auto actualRoad : this->roads) {
		QLineF leftBerm = actualRoad.road->getLineParams(LEFT_BERM);
		QLineF rightBerm = actualRoad.road->getLineParams(RIGHT_BERM);
		
		intersectType = theLine.intersect(rightBerm, &crossPoint);
		if (intersectType == QLineF::BoundedIntersection)
			points.push_back(crossPoint);
		intersectType = theLine.intersect(leftBerm, &crossPoint);
		if (intersectType == QLineF::BoundedIntersection)
			points.push_back(crossPoint);
	}
	double minimumDistance = 9999999.0, minimumRightDistance = 9999999.0;
	double dist;
	crossPoint = QPointF(0, 0);
	for (auto _point : points) {
		dist = sqrt(pow(oppositePoint.x() - _point.x(), 2) + pow(oppositePoint.y() - _point.y(), 2));
		if (dist < minimumDistance) {
			minimumDistance = dist;
			crossPoint = _point;
		}
	}
	return crossPoint;
}

void Junction::addRoad(Road *road)
{
	roads.push_back(ConnectedRoad{ roads[0].road->getCloserBerm(road->getFurtherPoint(point)), road }); //getCloserBerm returns RIGHT_LANE or LEFT_LANE, based on which side it's closer to be
	roadIds.push_back(road->id);
}

void Junction::deleteRoad(Road * deletedRoad)
{
	for (auto roadIt = roadIds.begin(); roadIt < roadIds.end(); roadIt++)
		if ((*roadIt) == deletedRoad->id) {
			roadIds.erase(roadIt);
			break;
		}
	for (auto roadIt = roads.begin(); roadIt < roads.end(); roadIt++)
		if ((*roadIt).road->id == deletedRoad->id) {
			roads.erase(roadIt);
			break;
		}
	for (auto connIt = connections.begin(); connIt < connections.end(); connIt) {
		if ((*connIt).nextRoad == deletedRoad|| (*connIt).previousRoad == deletedRoad)
			connections.erase(connIt);
	}
}

void Junction::drawConnections()
{
	for (auto connIt = connections.begin(); connIt < connections.end(); connIt++) {
		drawConnection(getPointsToDrawConnection(*connIt));
	}
}

bool Junction::isPoint(QPointF point)
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

int Junction::getId()
{
	return id;
}

Point Junction::getPoint()
{
	return point;
}

ObjectType Junction::getObjectType()
{
	return _objectType;
}

std::vector<int> Junction::getRoadIds()
{
	return roadIds;
}

std::vector<Connection> Junction::getConnectionsFrom(int roadFrom)
{
	std::vector<Connection> workingConnections;
	for (auto connectionIt = connections.begin(); connectionIt < connections.end(); connectionIt++)
		if ((*connectionIt).previousRoad->id == roadFrom)
			workingConnections.push_back(*connectionIt);
	if (roadFrom == -1)
		workingConnections = connections;
	return workingConnections;
}

bool Junction::connectRoads(Road* roadFrom, LaneType laneFrom, Road* roadTo, LaneType laneTo)
{
	if (!checkIfBelongs(roadFrom->id) || !checkIfBelongs(roadTo->id))
		return false;
	if (((laneFrom == LANE || laneFrom == RIGHT_LANE || laneFrom == LEFT_LANE) && roadFrom->getPointIndex(point, MID) == 0)
		|| ((laneTo == BACK_LANE || laneTo == LEFT_BACK_LANE || laneTo == RIGHT_BACK_LANE) && roadTo->getPointIndex(point, MID) == 0)
		|| ((laneTo == LANE || laneTo == RIGHT_LANE || laneTo == LEFT_LANE) && roadTo->getLastPointOf(MID) == point)
		|| ((laneFrom == BACK_LANE || laneFrom == LEFT_BACK_LANE || laneFrom == RIGHT_BACK_LANE) && roadFrom->getLastPointOf(MID) == point))//connection cannot goes in this direction
		return false;
	Connection newConnection;
	newConnection.previousLaneType = laneFrom;
	newConnection.previousRoad = roadFrom;
	int pointIndexOnTheRoad = roadTo->getPointIndex(point, MID);
	newConnection.nextLaneType = laneTo;
	newConnection.nextRoad = roadTo;
	newConnection.nextPoint = pointIndexOnTheRoad;
	if (pointIndexOnTheRoad > -1) {
		newConnection.nextJunction = (Junction*)roadTo->getNextJunction(laneTo, pointIndexOnTheRoad);
		newConnection.distanceToNextJunction = pointIndexOnTheRoad;
	}
	if (newConnection.nextJunction != NULL) {
		connections.push_back(newConnection);
		drawConnection(getPointsToDrawConnection(connections[connections.size() - 1]));
		return true;
	}
	return false;
}

//function used only while deleting junction and one actualRoad left from it
void Junction::forgetAboutMe()
{
	for (auto roadIt = roads.begin(); roadIt < roads.end(); roadIt++)
		(*roadIt).road->deleteJunction(this);
}

void Junction::connectToOneWayOneLane(Road *newRoad, LaneType previousType, Road* previousRoad) //to OneWayOneLane
{
	Connection newConnection;
	newConnection.previousLaneType = previousType;
	newConnection.previousRoad = previousRoad;
	int pointIndexOnTheRoad = newRoad->getPointIndex(point, LANE);
	if (pointIndexOnTheRoad == 0 && newRoad->id == previousRoad->id) //break if u try to connect beggining of the same road
		return;
	newConnection.nextLaneType = LANE;
	newConnection.nextRoad = newRoad;
	newConnection.nextPoint = pointIndexOnTheRoad;
	if (pointIndexOnTheRoad > -1) {
		newConnection.nextJunction = (Junction*)newRoad->getNextJunction(LANE, pointIndexOnTheRoad);
		newConnection.distanceToNextJunction = pointIndexOnTheRoad;
	}
	if(newConnection.nextJunction != NULL)
		connections.push_back(newConnection);
}

void Junction::connectToOneWayTwoLanes(Road *, LaneType, Road* previousRoad)
{
}

void Junction::drawConnection(std::vector<Point> points)
{
	if (points.size() > 0) drawBezierCurve(points[0], points[1], points[2]);
}

std::vector<Point> Junction::getPointsToDrawConnection(Connection connection)
{
	std::vector<Point> pointsToDraw;
	for (auto connIt = connections.begin(); connIt < connections.end(); connIt++) {
		if ((*connIt).previousRoad == connection.previousRoad && (*connIt).nextRoad == connection.nextRoad)
			connection = *connIt;
	}
	int index = connection.previousRoad->getPointIndex(point, MID);
	Point start = connection.previousRoad->getStartPointForConnection(index, connection.previousLaneType);
	index = connection.nextRoad->getPointIndex(point, MID);
	Point end = connection.nextRoad->getEndPointForConnection(index, connection.nextLaneType);
	if (start.x() != 0 && end.x() != 0) {
		pointsToDraw.push_back(start);
		pointsToDraw.push_back(point);
		pointsToDraw.push_back(end);
	}
	return pointsToDraw;
}

void copyLanePointVectorToPointVector(std::vector<LanePoint> vectorFrom, std::vector<Point> &vectorTo)
{
	for (auto vectorFromIt = vectorFrom.begin(); vectorFromIt < vectorFrom.end(); vectorFromIt++)
		vectorTo.push_back((*vectorFromIt).point);
}

bool Junction::checkIfBelongs(int roadId) {
	for (auto roadIt = roadIds.begin(); roadIt < roadIds.end(); roadIt++) {
		if ((*roadIt) == roadId)
			return true;
	}
	return false;
}


QPointF getBezierPoint(QPointF point1, QPointF point2, QPointF point3, double t) {
	QPointF point;
	point.setX(qPow(1 - t, 3)*point1.x() + 3 * t*qPow(1 - t, 2)*(-(point1.x() - 8 * point2.x() + point3.x()) / 6) + 3 * qPow(t, 2)*(1 - t)*(-(point1.x() - 8 * point2.x() + point3.x()) / 6) + qPow(t, 3)*(point3.x()));

	point.setY(qPow(1 - t, 3)*point1.y() + 3 * t*qPow(1 - t, 2)*(-(point1.y() - 8 * point2.y() + point3.y()) / 6) + 3 * qPow(t, 2)*(1 - t)*(-(point1.y() - 8 * point2.y() + point3.y()) / 6) + qPow(t, 3)*point3.y());
	return point;
}

void drawBezierCurve(Point _point1/*first*/, Point _point2/*control*/, Point _point3/*last*/) {
	QPointF p1(_point1.x(), _point1.y()), p2, point1(_point1.x(), _point1.y()), point2(_point2.x(), _point2.y()), point3(_point3.x(), _point3.y());
	glColor3f(0.0f, 1.0f, 0.0f);
	for (double t = 0.0; t <= 1.0; t += 0.02)
	{
		QPointF p2 = getBezierPoint(point1, point2, point3, t);
		glBegin(GL_LINES);
		glVertex2f(p1.x(), p1.y());
		glVertex2f(p2.x(), p2.y());
		glEnd();
		glFlush();
		p1 = p2;
	}
}
