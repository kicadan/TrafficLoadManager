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
	Point nextPoint;
	int nextRoadId;
	LaneType nextLaneType;
	short direction;
	Junction* nextJunction = NULL;
	int distanceToNextJunction;
};

class Junction :
	public AppObject
{
public:
	Junction();
	Junction(Point, Road*, Road*);
	~Junction();
	Point returnCrossPointsForBerm(LineParams, Point);
	void addRoad(Road*);
	void deleteRoad(Road*);
	void makeConnections();
	void forgetAboutMe(int);
	bool isPoint(QPoint);
	bool isPoint(Point);
	int numberOfRoads();
	ObjectType getObjectType();
	std::vector<int> getRoadIds();
private:
	Point point;
	std::vector<Connection> connections;
	std::vector<Road*> roads;
	std::vector<int> roadIds;

	void connectRoad(Road*);
	void connectOneWayOneLane(Road*);
	void connectFromOneWayOneLane(Road*);
	void connectToOneWayOneLane(Road*);
};

void copyLanePointVectorToPointVector(std::vector<LanePoint>, std::vector<Point>&);

