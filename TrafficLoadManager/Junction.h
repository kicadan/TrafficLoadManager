#pragma once
#include "AppObject.h"
#include "Roads.h"

class Junction;

struct LanePoint {
	Point point;
	Junction* junction;
};

struct Connection {
	//int roadActualIndex;
	//LaneType actualLaneType;
	//int roadNextIndex;
	//LaneType nextLaneType;
	//int nextLaneIndex;
	//byte direction; //+/- 1 (direction while iterating on index)
	int previousRoadId;
	LaneType previousLaneType;
	Point nextPoint; //junction point
	int nextRoadId;
	LaneType nextLaneType;
	short direction;
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
	Junction(Point, Road*, int);
	~Junction();
	Point returnCrossPointsForBerm(LineParams, Point);
	void addRoad(Road*);
	void deleteRoad(Road*);
	void makeConnections();
	void forgetAboutMe();
	bool isPoint(QPoint);
	bool isPoint(Point);
	int numberOfRoads();
	int getId();
	ObjectType getObjectType();
	std::vector<int> getRoadIds();
	std::vector<Connection> getConnectionsFrom(int);
private:
	Point point;
	int id;
	std::vector<Connection> connections;
	std::vector<ConnectedRoad> roads;
	std::vector<int> roadIds;

	void connectRoad(ConnectedRoad, ConnectedRoad);
	void connectOneWayOneLane(Road*, LaneType, int);
	void connectFromOneWayOneLane(Road*);
	void connectToOneWayOneLane(Road*);
};

void copyLanePointVectorToPointVector(std::vector<LanePoint>, std::vector<Point>&);

