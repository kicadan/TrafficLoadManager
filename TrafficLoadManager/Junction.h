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
	int smartDrivers = 0;
	std::vector<Junction*> destinations;
};

struct ConnectedRoad {
	LaneType whichSide;
	Road* road;
};

enum Light {
	RED = 0,
	GREEN = 1
};

struct LightSequence {
	Light light;
	int time; //seconds
};

struct TrafficLightsSettings {
	LightSequence redLight { RED, 0 };
	LightSequence greenLight{ GREEN, 0 };
};

class Junction :
	public AppObject
{
public:
	Junction();
	Junction(int);
	Junction(Point, Road*, int);
	~Junction();
	//appobject
	ObjectType getObjectType();

	//connections
	std::vector<Connection> getConnectionsFrom(int roadId);
	void addConnection(Connection);
	void updateConnectionsForRoad(int);
	void updateOtherJunctionsOnMainRoad();
	void validateConnections();
	bool connectRoads(Road* roadFrom, LaneType laneFrom, Road* roadTo, LaneType laneTo);

	//car spawn
	CarSpawnSettings getCarSpawnSettings();
	void setAsCarSpawn();
	void notCarSpawn();
	void editCarSpawn(CarSpawnSettings);
	void makeConnectionsForCarSpawn(); //makes connections if CarSpawn has 1 road in it
	bool isCarSpawn();

	//traffic lights
	TrafficLightsSettings getTrafficLightsSettings();
	void setTrafficLights();
	void notTrafficLights();
	void setTrafficLightsSettings(TrafficLightsSettings);
	void drawTrafficLights();
	bool gotTrafficLights();

	//junction
	QPointF returnCrossPointsForBerm(QLineF, QPointF);
	Point getPoint();
	std::vector<int> getRoadIds();
	void addRoad(Road*);
	void deleteRoad(Road*);
	void drawJunction();
	void forgetAboutMe();
	void setName(char*);
	void forgetJunction(Junction*);
	bool isPoint(QPointF);
	bool isPoint(Point);
	char* getName();
	int getNumberOfRoads();
	int getId();

private:
	TrafficLightsSettings trafficLightsSettings;
	CarSpawnSettings carSpawnSettings;
	Point point;
	std::vector<Connection> connections;
	std::vector<ConnectedRoad> roads;
	std::vector<int> roadIds;
	float red = 0.6, green = 0.0, blue = 0.45;
	bool _isCarSpawn = false;
	bool _gotTrafficLights = false;
	char name[100];
	int id;

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

