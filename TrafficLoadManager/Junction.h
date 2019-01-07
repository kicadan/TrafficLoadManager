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
	int timeBetweenCars = 0;
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
	char lightsName[20];
};

struct LightsStatistics {
	int lightsId;
	char lightsName[20];
	int roadId;
	short direction;
	int carsPassed = 0;
};

struct JunctionStatistics {
	char junctionName[100];
	int junctionId;
	std::vector<LightsStatistics> lightsStats;
	int overallPassed = 0;
	int shouldBeEmitted = 0;
	int emitted = 0;
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
	void addWay(Way);
	void clearWays();
	Way updateCarSpawn();
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
	void colourLightsForEditing();
	bool hasTrafficLights();

	//junction
	QPointF returnCrossPointsForBerm(QLineF, QPointF, int roadId = -1);
	Point getPoint();
	std::vector<int> getRoadIds();
	std::vector<LightsStatistics> getLightsStatistics();
	void carPassedJunction(Road*, LaneType);
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
	int getNumberOfCars(int /*1- emitted, 2-shouldBeEmitted*/);
	int getNumberOfRoads();
	int getId();

private:
	TrafficLightsSettings trafficLightsSettings;
	CarSpawnSettings carSpawnSettings;
	Point point;
	std::vector<Connection> connections;
	std::vector<ConnectedRoad> roads;
	std::vector<Traffics> trafficLights; //next second
	std::vector<LightsStatistics> lightsStatistics;
	std::vector<Way> ways;
	std::vector<int> roadIds;
	float red = 0.6, green = 0.0, blue = 0.45;
	bool _isCarSpawn = false;
	bool _gotTrafficLights = false;
	char name[100];
	int wayCounter = 0;
	int emitTimeCounter = 0;;
	int carsEmitted = 0;
	int carsShouldBeEmitted = 0;
	int id;

	std::vector<Point> getPointsToDrawConnection(Connection);
	std::vector<Point> getPointsToClearJunctionArea();
	void clearJunctionArea();
	void drawConnection(std::vector<Point>, float, float, float);
	bool checkIfBelongs(int);
};

void drawBezierCurve(Point, Point, Point, float, float, float);
void drawLine(Point, Point, float, float, float);
void drawCircle(Point, float);
void drawFilledCircle(Point, float, float, float, float);
void drawColouredSquare(Point, float, float, float);
void drawSquare(Point);
void drawWhiteQuads(std::vector<Point>);


struct Way {
	Junction* from;
	Junction* to;
	std::vector<Connection> steps;
	int length = 0;
};