#pragma once
#include <vector>
#include "Point.h"
#include <string>
#include <QPoint>
#include <QLineF>
#include <QtCore/qmath.h>

#define distance 20

enum ElementType {
	OneWayRoadWithOneLane = 0,
	OneWayRoadWithTwoLanes = 1,
	TwoWayRoadWithOneLane = 2,
	CarSpawn = 3,
	TrafficLights = 4,
	JunctionConnection = 5
};

struct vectors {
	double xl;
	double yl;
	double xr;
	double yr;
};

struct LineParams {
	double a;
	double b;
	bool upright;
	Point firstPoint;
	Point lastPoint;
};

enum LaneType
{
	LEFT_BERM = 0,
	RIGHT_BERM = 1,
	LANE = 2,
	LEFT_LANE = 3,
	RIGHT_LANE = 4,
	BACK_LANE = 5,
	LEFT_BACK_LANE = 6,
	RIGHT_BACK_LANE = 7,
	MID = 8,
	NOTHING = 9
};

class Road :
	public AppObject
{
public:
	Road(ElementType);

	int id;

	//virtual bool setRoad(QPoint, QPoint, bool, Junction*);// LineParams startBermParams = LineParams{ 0.0, 0.0, false }, LineParams endBermParams = LineParams{ 0.0, 0.0, false }); // , int, int);//, int, int) = 0;
	Point searchPoint(Point);
	void addOtherRoad(int);
	void setBerm(LaneType, QLineF);
	LaneType getCloserBerm(Point); //returns berm basing on distance calculated on opposite point to connecting to this road (if _firstPoint connecting, _lastPoint is the calculation base)
	bool doLineCrosses(QLineF);
	Point getFurtherPoint(Point); //returns lastPoint if parameter point is firstPoint and firstPoint if parameter is lastPoint
	

	virtual Point getPoint(int, LaneType) = 0;
	virtual Point getLastPointOf(LaneType) = 0;
	virtual Point getFirstPointOf(LaneType) = 0;
	virtual Point getStartPointForConnection(int, LaneType) = 0;
	virtual Point getEndPointForConnection(int, LaneType) = 0;
	virtual LaneType searchPointForLaneType(Point) = 0;
	virtual int getPointIndex(Point, LaneType) = 0;
	virtual int getUsageOfLane(LaneType) = 0;
	virtual void* getNextJunction(LaneType, int&) = 0;
	virtual void* getPreviousJunction(LaneType, int&) = 0;
	virtual void* searchForClosestJunction(Point, LaneType) = 0;
	virtual void addJunction(Point, void*) = 0;
	virtual void deleteJunction(void*) = 0;
	virtual void deleteFromJunctions() = 0;
	virtual void processAllVehicles(std::vector<Vehicle*>&) = 0;
	virtual void freePoint(LaneType, int) = 0;
	virtual bool reservePoint(LaneType, Vehicle*, int) = 0;
	ElementType getRoadType();
	ObjectType getObjectType();
	QLineF getLineParams(LaneType);
	vectors getParallelVectors();
protected:
	ElementType _roadType;
	std::vector<Point> mid, rightBermVector, leftBermVector;
	vectors parallel_segments;
	QLineF leftBerm, rightBerm, coreLine;
	bool horizontal;

	virtual void addPoint(Point, LaneType) = 0;
	virtual vectors calc_vectors(QPointF, QPointF) = 0;
};
