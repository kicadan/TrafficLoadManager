#pragma once
#include <vector>
#include "Point.h"
#include <string>
#include <QPoint>

#define distance 20

enum RoadType {
	OneWayRoadWithOneLane = 0
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
	RIGHT_LANE = 4
};

class Road :
	public AppObject
{
public:
	Road(RoadType);

	int id;

	//virtual bool setRoad(QPoint, QPoint, bool, Junction*);// LineParams startBermParams = LineParams{ 0.0, 0.0, false }, LineParams endBermParams = LineParams{ 0.0, 0.0, false }); // , int, int);//, int, int) = 0;
	Point searchPoint(Point);
	void addOtherRoad(int);
	//LaneType returnCloserBerm(QPoint); //returns berm basing on distance calculated on opposite point to connecting to this road (if _firstPoint connecting, _lastPoint is the calculation base)
	bool doLineCrosses(LineParams);
	

	virtual Point getPoint(int, LaneType) = 0;
	virtual Point getLastPointOf(LaneType) = 0;
	virtual Point getFirstPointOf(LaneType) = 0;
	virtual int getPointIndex(Point) = 0;
	virtual void* getNextJunction(LaneType, int&) = 0;
	virtual void addJunction(Point, LaneType, void*) = 0;
	virtual void deleteJunction(void*) = 0;
	virtual void deleteFromJunctions() = 0;
	RoadType getRoadType();
	ObjectType getObjectType();
	LineParams getLineParams(LaneType);
protected:
	RoadType _roadType;
	std::vector<Point> mid, bermL, bermR;
	std::vector<int> otherRoads;
	vectors parallel_segments;
	LineParams coreLineParams, bermLParams, bermRParams;
	bool horizontal;

	virtual void addPoint(Point, LaneType) = 0;
	virtual vectors calc_vectors(Point, Point) = 0;
};
