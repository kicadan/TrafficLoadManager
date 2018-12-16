#include "Junction.h"



Junction::Junction():AppObject(JUNCTION)
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
	roads.push_back(ConnectedRoad{ roads[0].road->getCloserBerm(road->getFurtherPoint(point)), road }); //if in the middle, getCloserBerm returns RIGHT_LANE or LEFT_LANE, based on which side it's closer to be, or LANE if it is exactly in the middle
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
		if ((*connectionIt).previousRoadId == roadFrom)
			workingConnections.push_back(*connectionIt);
	if (roadFrom == -1)
		workingConnections = connections;
	return workingConnections;
}

void Junction::makeConnections()
{
	connections.clear();
	for(auto roadFromIt = roads.begin(); roadFromIt < roads.end(); roadFromIt++)
		for (auto roadIt = roads.begin(); roadIt < roads.end(); roadIt++) {
			connectRoad(*roadFromIt, *roadIt);
		}
}

void Junction::connectRoad(ConnectedRoad roadFrom, ConnectedRoad roadTo)
{ //check previous road, next road and connectRoads with all accurate lane types f.e. OneWayRoadWithTwoLanes : RIGHT_LANE -> right_side_road->ACCURATE_LANES, LEFT_LANE -> left_side_road->ACCURATE_LANES 
	if (roadTo.road->getRoadType() == OneWayRoadWithOneLane && roadFrom.road->getRoadType() == OneWayRoadWithOneLane)
		if(roadFrom.road->getPointIndex(point) != 0) //road cannot goes in this direction
			connectOneWayOneLane(roadTo.road, LANE, roadFrom.road->id);
	//for other type roads connectTYPEOFROAD(roadTo.road, LANE, roadFrom.road->id, ROADFROMLANE)   ==> ROADFROMLANE is important as input for getNextJunction in function body
}

//function used only while deleting junction and one actualRoad left from it
void Junction::forgetAboutMe()
{
	for (auto roadIt = roads.begin(); roadIt < roads.end(); roadIt++)
		(*roadIt).road->deleteJunction(this);
}

void Junction::connectOneWayOneLane(Road *newRoad, LaneType previousType, int previousRoadId)
{
	Connection newConnection;
	newConnection.previousLaneType = previousType;
	newConnection.previousRoadId = previousRoadId;
	int pointIndexOnTheRoad = newRoad->getPointIndex(point);
	if (pointIndexOnTheRoad == 0 && newRoad->id == previousRoadId) //break if u try to connect beggining of the same road
		return;
	newConnection.nextLaneType = LANE;
	newConnection.nextRoadId = newRoad->id;
	newConnection.direction = 1;
	newConnection.nextPoint = pointIndexOnTheRoad;
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
