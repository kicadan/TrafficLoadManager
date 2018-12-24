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
	float red, green, blue;
};

struct CarSpawnSettings {
	int carsPerMinute = 0;
	std::vector<Junction*> junctions;
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
	void addRoad(Road*);
	void deleteRoad(Road*);
	void updateConnectionsForRoad(int);
	void updateOtherJunctionsOnMainRoad();
	void drawJunction();
	void forgetAboutMe();
	void validateConnections();
	void setAsCarSpawn();
	void notCarSpawn();
	void editCarSpawn(CarSpawnSettings);
	void makeConnectionsForCarSpawn(); //makes connections if CarSpawn has 1 road in it
	bool connectRoads(Road* roadFrom, LaneType laneFrom, Road* roadTo, LaneType laneTo);
	bool isPoint(QPointF);
	bool isPoint(Point);
	bool isCarSpawn();
	int numberOfRoads();
	int getId();
	Point getPoint();
	ObjectType getObjectType();
	char* getName();
	std::vector<int> getRoadIds();
	std::vector<Connection> getConnectionsFrom(int);
private:
	int id;
	bool _isCarSpawn = false;
	Point point;
	CarSpawnSettings carSpawnSettings;
	char name[100];
	std::vector<Connection> connections;
	std::vector<ConnectedRoad> roads;
	std::vector<int> roadIds;
	float red = 0.6, green = 0.0, blue = 0.45;

	std::vector<Point> getPointsToDrawConnection(Connection);
	std::vector<Point> getPointsToClearJunctionArea();
	void connectToOneWayOneLane(Road*, LaneType, Road*);//deprecated
	void connectToOneWayTwoLanes(Road*, LaneType, Road*);//deprecated
	void clearJunctionArea();
	void drawConnection(std::vector<Point>, float, float, float);
	bool checkIfBelongs(int);
};

void drawBezierCurve(Point, Point, Point, float, float, float);
void drawLine(Point, Point, float, float, float);
void drawCircle(Point);
void drawWhiteQuads(std::vector<Point>);
void copyLanePointVectorToPointVector(std::vector<LanePoint>, std::vector<Point>&); //deprecated

