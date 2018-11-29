#pragma once
#include <vector>
#include "Point.h"
#include <string>
#include <QPoint>

#define distance 10

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
	int id;

	virtual void addPoint(Point, LaneType) = 0;
	//virtual bool drawRoad(QPoint, QPoint, bool, Junction*);// LineParams startBermParams = LineParams{ 0.0, 0.0, false }, LineParams endBermParams = LineParams{ 0.0, 0.0, false }); // , int, int);//, int, int) = 0;
	Point searchPoint(QPoint);
	void addOtherRoad(int);
	LineParams getBermParams(LaneType);
	LaneType returnCloserBerm(QPoint); //returns berm basing on distance calculated on opposite point to connecting to this road (if firstPoint connecting, lastPoint is the calculation base)
	RoadType getRoadType();
protected:
	RoadType roadType;
	std::vector<Point> mid;
	std::vector<int> otherRoads;
	vectors parallel_segments;
	LineParams midLineParams, bermLParams, bermRParams;
	bool horizontal;
private:
	virtual vectors calc_vectors(QPoint, QPoint) = 0;
};
