#pragma once
#include "AppObject.h"
#include "Roads.h"

class Junction;

struct LanePoint {
	Point point;
	Junction* junction;
};

struct Connection {
	Road* previousRoad;
	LaneType previousLaneType;
	int nextPoint; //junction point in road's lane
	Road* nextRoad;
	LaneType nextLaneType;
	Junction* nextJunction = NULL;
	int distanceToNextJunction;
};

struct ConnectedRoad {
	LaneType whichSide;
	Road* road;
};

class Junction :
	public AppObject
{
public:
	Junction();
	Junction(int);
	Junction(Point, Road*, int);
	~Junction();
	QPointF returnCrossPointsForBerm(QLineF, QPointF);
	bool connectRoads(Road* roadFrom, LaneType laneFrom, Road* roadTo, LaneType laneTo);
	void addRoad(Road*);
	void deleteRoad(Road*);
	void drawConnections();
	void forgetAboutMe();
	bool isPoint(QPointF);
	bool isPoint(Point);
	int numberOfRoads();
	int getId();
	Point getPoint();
	ObjectType getObjectType();
	std::vector<int> getRoadIds();
	std::vector<Connection> getConnectionsFrom(int);
private:
	Point point;
	int id;
	std::vector<Connection> connections;
	std::vector<ConnectedRoad> roads;
	std::vector<int> roadIds;

	std::vector<Point> getPointsToDrawConnection(Connection);
	void connectToOneWayOneLane(Road*, LaneType, Road*);
	void connectToOneWayTwoLanes(Road*, LaneType, Road*);
	void drawConnection(std::vector<Point>);
	bool checkIfBelongs(int);
};

void drawBezierCurve(Point, Point, Point);
void copyLanePointVectorToPointVector(std::vector<LanePoint>, std::vector<Point>&); //deprecated

