#include "Junction.h"



Junction::Junction() :AppObject(JUNCTION)
{
}

Junction::Junction(int id) : AppObject(JUNCTION), id(id)
{
}

Junction::Junction(Point _point, Road* mainRoad, int id) : AppObject(JUNCTION)
{
	this->point = _point;
	this->id = id;
	roads.push_back(ConnectedRoad{ LANE, mainRoad }); //main actualRoad could be lane, the rest have right or left berm param
	roadIds.push_back(mainRoad->id);
	char name[100];
	sprintf(name, "Wêze³ numer: %d", id);
	strcpy(this->name, name);
}


Junction::~Junction()
{
}

QPointF Junction::returnCrossPointsForBerm(QLineF theLine, QPointF oppositePoint)
{
	QLineF::IntersectType intersectType;
	std::vector<QPointF> points;
	QPointF crossPoint(0, 0);
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
	//clear default connections made for car spawn while any other roads connected
	if (roads.size() == 1 && _isCarSpawn) {
		connections.clear();
	}
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
	auto connIt = connections.begin();
	while (connIt < connections.end()) {
		if ((*connIt).nextRoad == deletedRoad || (*connIt).previousRoad == deletedRoad) {
			connections.erase(connIt);
			connIt = connections.begin();
			continue;
		}
		connIt++;
	}
}

void Junction::addConnection(Connection added)
{
	for (auto connectionIt = connections.begin(); connectionIt < connections.end(); connectionIt++) {
		if ((*connectionIt).previousRoad->id == added.previousRoad->id && (*connectionIt).previousLaneType == added.previousLaneType
						&& (*connectionIt).nextRoad->id == added.nextRoad->id && (*connectionIt).nextLaneType == added.nextLaneType) { //if the same connection then update
			(*connectionIt) = added;
			return;
		}
	}
	//if couldn't be updated
	connections.push_back(added);
}

void Junction::updateConnectionsForRoad(int roadId)
{
	int pointIdx;
	Junction* nextJunction = NULL;
	for (auto connIt = connections.begin(); connIt < connections.end(); connIt++) {
		if ((*connIt).nextRoad->id == roadId) {
			pointIdx = (*connIt).nextRoad->getPointIndex(point, MID);
			nextJunction = (Junction*)(*connIt).nextRoad->getNextJunction((*connIt).nextLaneType, pointIdx);
			if (nextJunction != NULL) {
				(*connIt).nextJunction = nextJunction;
				(*connIt).distanceToNextJunction = pointIdx;
			}
			nextJunction = NULL;
		}
	}
}

void Junction::drawJunction()
{
	if (!(_isCarSpawn && roads.size() == 1)) {
		clearJunctionArea();
		for (auto connIt = connections.begin(); connIt < connections.end(); connIt++) {
			drawConnection(getPointsToDrawConnection(*connIt), (*connIt).red, (*connIt).green, (*connIt).blue);
		}
	}
	if (_isCarSpawn)
		drawCircle(point);
}

bool Junction::isPoint(QPointF point)
{
	return this->point == point;
}

bool Junction::isPoint(Point point)
{
	return this->point == point;
}

bool Junction::isCarSpawn()
{
	return _isCarSpawn;
}

int Junction::getNumberOfRoads()
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

CarSpawnSettings Junction::getCarSpawnSettings()
{
	if (_isCarSpawn)
		return carSpawnSettings;
	else
		return CarSpawnSettings();
}

char* Junction::getName()
{
	return name;
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
		newConnection.red = red, newConnection.green = green, newConnection.blue = blue;
		red = (red + 0.15) <= 1 ? red + 0.15 : red + 0.15 - 1; green = (green + 0.25) <= 1 ? green + 0.25 : green + 0.25 - 1; blue = (blue + 0.35) <= 1 ? blue + 0.35 : blue + 0.35 - 1;
		addConnection(newConnection);
		drawConnection(getPointsToDrawConnection(connections[connections.size() - 1]), newConnection.red, newConnection.green, newConnection.blue);
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

void Junction::validateConnections()
{
	int pointIndexOnTheRoad;
	auto connIt = connections.begin();
	while (connIt < connections.end()) {
		pointIndexOnTheRoad = (*connIt).nextRoad->getPointIndex(point, MID);
		if ((*connIt).nextRoad->getNextJunction((*connIt).nextLaneType, pointIndexOnTheRoad) == NULL) {
			connections.erase(connIt);
			connIt = connections.begin();
			continue;
		}
		connIt++;
	}
}

void Junction::setName(char *name)
{
	strcpy(this->name, name);
}

void Junction::setAsCarSpawn()
{
	_isCarSpawn = true;
}

void Junction::notCarSpawn()
{
	_isCarSpawn = false;
}

void Junction::editCarSpawn(CarSpawnSettings settings)
{
	carSpawnSettings = settings;
}

void Junction::forgetJunction(Junction *deleted)
{
	for (auto junctionIt = carSpawnSettings.destinations.begin(); junctionIt < carSpawnSettings.destinations.end(); junctionIt++)
		if ((*junctionIt)->getId() == deleted->getId()) {
			carSpawnSettings.destinations.erase(junctionIt);
			break;
		}
}

void Junction::makeConnectionsForCarSpawn()
{
	Road* road;
	if (_isCarSpawn) {
		for (auto roadIt = roads.begin(); roadIt < roads.end(); roadIt++) {
			Connection newConnection;
			road = (*roadIt).road;
			int pointIndexOnTheRoad;
			switch (road->getRoadType()) {
			case OneWayRoadWithOneLane: case TwoWayRoadWithOneLane: {
				pointIndexOnTheRoad = road->getPointIndex(point, MID);
				newConnection.nextPoint = pointIndexOnTheRoad;
				newConnection.nextJunction = (Junction*)road->getNextJunction(LANE, pointIndexOnTheRoad);
				newConnection.distanceToNextJunction = pointIndexOnTheRoad;
				newConnection.previousLaneType = LANE;
				newConnection.previousRoad = road;
				newConnection.nextLaneType = LANE;
				newConnection.nextRoad = road;
				newConnection.red = 1; newConnection.green = 1; newConnection.blue = 1;
				if (newConnection.nextJunction != NULL) addConnection(newConnection);
				if (road->getRoadType() == TwoWayRoadWithOneLane) {
					pointIndexOnTheRoad = road->getPointIndex(point, MID);
					newConnection.nextPoint = pointIndexOnTheRoad;
					newConnection.nextJunction = (Junction*)road->getNextJunction(BACK_LANE, pointIndexOnTheRoad);
					newConnection.distanceToNextJunction = pointIndexOnTheRoad;
					newConnection.previousLaneType = BACK_LANE;
					newConnection.previousRoad = road;
					newConnection.nextLaneType = BACK_LANE;
					newConnection.nextRoad = road;
					newConnection.red = 1; newConnection.green = 1; newConnection.blue = 1;
					if (newConnection.nextJunction != NULL) addConnection(newConnection);
				}
				break;
			}
			case OneWayRoadWithTwoLanes: {
				//leftLane
				pointIndexOnTheRoad = road->getPointIndex(point, MID);
				newConnection.nextPoint = pointIndexOnTheRoad;
				newConnection.nextJunction = (Junction*)road->getNextJunction(LEFT_LANE, pointIndexOnTheRoad);
				newConnection.distanceToNextJunction = pointIndexOnTheRoad;
				newConnection.previousLaneType = LEFT_LANE;
				newConnection.previousRoad = road;
				newConnection.nextLaneType = LEFT_LANE;
				newConnection.nextRoad = road;
				newConnection.red = 1; newConnection.green = 1; newConnection.blue = 1;
				if (newConnection.nextJunction != NULL) addConnection(newConnection);
				//right lane
				pointIndexOnTheRoad = road->getPointIndex(point, MID);
				newConnection.nextPoint = pointIndexOnTheRoad;
				newConnection.nextJunction = (Junction*)road->getNextJunction(RIGHT_LANE, pointIndexOnTheRoad);
				newConnection.distanceToNextJunction = pointIndexOnTheRoad;
				newConnection.previousLaneType = RIGHT_LANE;
				newConnection.previousRoad = road;
				newConnection.nextLaneType = RIGHT_LANE;
				newConnection.nextRoad = road;
				newConnection.red = 1; newConnection.green = 1; newConnection.blue = 1;
				if (newConnection.nextJunction != NULL) addConnection(newConnection);
			}
			}
		}
	}
}

//deprecated
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
	if (newConnection.nextJunction != NULL)
		addConnection(newConnection);
}

//deprecated
void Junction::connectToOneWayTwoLanes(Road *, LaneType, Road* previousRoad)
{
}

void Junction::updateOtherJunctionsOnMainRoad()
{
	Road* mainRoad = roads[0].road;
	int pointIdx = mainRoad->getPointIndex(point, MID);
	if (mainRoad->getRoadType() == TwoWayRoadWithOneLane) {
		Junction* nextJunction = (Junction*)mainRoad->getNextJunction(LANE, pointIdx);
		if (nextJunction != NULL) {
			if (nextJunction->isCarSpawn())
				nextJunction->makeConnectionsForCarSpawn();
			else {
				nextJunction->updateConnectionsForRoad(mainRoad->id);
			}
		}
	}
	pointIdx = mainRoad->getPointIndex(point, MID);
	Junction* previousJunction = (Junction*)mainRoad->getPreviousJunction( mainRoad->getRoadType() == TwoWayRoadWithOneLane || mainRoad->getRoadType() == OneWayRoadWithOneLane ? LANE : LEFT_LANE, pointIdx );
	if (previousJunction != NULL) {
		if (previousJunction->isCarSpawn())
			previousJunction->makeConnectionsForCarSpawn();
		else {
			previousJunction->updateConnectionsForRoad(mainRoad->id);
		}
	}
}

void Junction::clearJunctionArea()
{
	drawWhiteQuads(getPointsToClearJunctionArea());
}

void Junction::drawConnection(std::vector<Point> points, float red, float green , float blue)
{
	if (points.size() > 0) {
		if (points[1].x() != 0)
			drawBezierCurve(points[0], points[1], points[2], red, green, blue);
		else
			drawLine(points[0], points[2], red, green, blue);
	}
}

std::vector<Point> Junction::getPointsToDrawConnection(Connection connection)
{
	std::vector<Point> pointsToDraw;
	int index = connection.previousRoad->getPointIndex(point, MID);
	Point start = connection.previousRoad->getStartPointForConnection(index, connection.previousLaneType);
	index = connection.nextRoad->getPointIndex(point, MID);
	Point end = connection.nextRoad->getEndPointForConnection(index, connection.nextLaneType);
	if (start.x() != 0 && end.x() != 0) {
		pointsToDraw.push_back(start);
		//if connecting road with itself
		if (connection.previousRoad->id == connection.nextRoad->id)
			pointsToDraw.push_back(Point(0, 0));
		else
			pointsToDraw.push_back(point);
		pointsToDraw.push_back(end);
	}
	return pointsToDraw;
}

std::vector<Point> Junction::getPointsToClearJunctionArea()
{
	QLineF rightBerm, leftBerm;
	std::vector<Point> pointsToPass;
	bool start = false;
	for (auto roadIt = roads.begin(); roadIt < roads.end(); roadIt++) {
		if ((*roadIt).whichSide != LANE) {
			rightBerm = (*roadIt).road->getLineParams(RIGHT_BERM);
			leftBerm = (*roadIt).road->getLineParams(LEFT_BERM);
			if ((*roadIt).road->getPointIndex(point, MID) == 0) {
				pointsToPass.push_back(Point(rightBerm.p1().x(), rightBerm.p1().y()));
				pointsToPass.push_back(Point(leftBerm.p1().x(), leftBerm.p1().y()));
			}
			else {
				pointsToPass.push_back(Point(rightBerm.p2().x(), rightBerm.p2().y()));
				pointsToPass.push_back(Point(leftBerm.p2().x(), leftBerm.p2().y()));
			}
		}
	}
	if (pointsToPass.size() == 2) {
		int multiplier = 0;
		switch (roads[0].road->getRoadType()) {
		case OneWayRoadWithOneLane: { multiplier = 2; break; }
		case OneWayRoadWithTwoLanes: case TwoWayRoadWithOneLane: {multiplier = 4; break; }
		}
		vectors parallel_vectors = roads[0].road->getParallelVectors();

		if (roads[1].whichSide == LEFT_BERM) {
			pointsToPass.push_back(Point(pointsToPass[1].x() + parallel_vectors.xr*multiplier, pointsToPass[1].y() + parallel_vectors.yr*multiplier));
			pointsToPass.push_back(Point(pointsToPass[0].x() + parallel_vectors.xr*multiplier, pointsToPass[0].y() + parallel_vectors.yr*multiplier));
		}
		else {
			pointsToPass.push_back(Point(pointsToPass[1].x() + parallel_vectors.xl*multiplier, pointsToPass[1].y() + parallel_vectors.yl*multiplier));
			pointsToPass.push_back(Point(pointsToPass[0].x() + parallel_vectors.xl*multiplier, pointsToPass[0].y() + parallel_vectors.yl*multiplier));
		}
	}
	return pointsToPass;
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

void drawBezierCurve(Point _point1/*first*/, Point _point2/*control*/, Point _point3/*last*/, float red, float green, float blue) {
	QPointF p1(_point1.x(), _point1.y()), p2, point1(_point1.x(), _point1.y()), point2(_point2.x(), _point2.y()), point3(_point3.x(), _point3.y());
	glColor3f(red, green, blue);
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

void drawLine(Point p1, Point p2, float red, float green, float blue)
{
	glBegin(GL_LINES);
	glColor3f(red, green, blue);
	glVertex2f(p1.x(), p1.y());
	glVertex2f(p2.x(), p2.y());
	glEnd();
	glFlush();
}

void drawCircle(Point point) {
	GLfloat twicePi = 2.0f * M_PI;

	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i <= 360; i++) {
		glVertex2f(point.x() + (2*distance * cos(i *  twicePi / 360)), point.y() + (2*distance* sin(i * twicePi / 360)));
	}
	glEnd();
}

void drawWhiteQuads(std::vector<Point> points)
{
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	for (auto pointIt = points.begin(); pointIt < points.end(); pointIt++) {
		glVertex2f((*pointIt).x(), (*pointIt).y());
	}
	glEnd();
	glFlush();
}
