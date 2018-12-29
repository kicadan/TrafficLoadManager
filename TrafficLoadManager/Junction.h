#pragma once
#include "AppObject.h"
#include "Roads.h"

class Junction;

struct Way;

struct LanePoint {
	Point point;
	Junction* junction;
};

struct Connection {
	Road* previousRoad;
	LaneType previousLaneType;
	int nextPoint; //junction point in road's lane while car spawn, but in normal junction connection made by user it is next point on the road (1 after junction)
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
	YELLOW = 1,
	GREEN = 2
};

struct LightSequence {
	Light light;
	int time; //seconds
};

struct Lights {
	LightSequence redLight{ RED, 0 };
	LightSequence greenLight{ GREEN, 0 };
	Point lightsPoint;
	QLineF berm;
	bool haveConnection = false;
	short direction; //on which side of the road it is
	int roadId;
	int lightsId;
	char lightsName[20];
};

struct TrafficLightsSettings {
	std::vector<Lights> lights;
	std::vector<Lights> lightsSequence;
	bool upToDate = false;
};

struct Traffics { //stores information about actual state of traffic lights during animation
	Light actualLight;
	Point lightsPoint;
	int counter = 0; //modulo maxGreenTime + maxRedTime
	int greenStarts; //when previous green in sequence ends
	int greenEnds; //after greenStart + green.time
	int timeModulo; // green + red time
	int lightsId;
	int roadId;
	short direction;
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
	Point getPointForTrafficLight(short, Point); //searches for closest berms cross point on main road
	Light getLightState(int, LaneType);
	void notTrafficLights();
	void setTrafficLights();
	void setTrafficLightsSettings(TrafficLightsSettings);
	void drawTrafficLights();
	//void drawTrafficLights(float, float, float);
	void updateLightsSettings();
	void updateLights();
	void initTrafficLights();
	void showFocusedLights(Point); //growing and shrinking lights, while focused on setting lights
	bool hasTrafficLights();

	//junction
	QPointF returnCrossPointsForBerm(QLineF, QPointF, int roadId = -1);
	Point getPoint();
	std::vector<int> getRoadIds();
	void addRoad(Road*);
	void deleteRoad(Road*);
	void drawJunction();
	void forgetAboutMe();
	void updateJunctionRoadsBerms();
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
	std::vector<Traffics> trafficLights; //next second
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
void drawCircle(Point, float);
void drawFilledCircle(Point, float, float, float, float);
void drawSquare(Point);
void drawWhiteQuads(std::vector<Point>);
void copyLanePointVectorToPointVector(std::vector<LanePoint>, std::vector<Point>&); //deprecated


struct Way {
	Junction from;
	Junction to;
	std::vector<Connection> steps;
	int length = 0;
};