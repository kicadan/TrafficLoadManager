#pragma once

#include "Junction.h"


class OneWayOneLane :
	public Road
{
public:
	OneWayOneLane(int);
	~OneWayOneLane();

	void setRoad(Point, Point, bool, Junction*, Junction*);// LineParams startBermParams = LineParams{ 0.0, 0.0, false }, LineParams endBermParams = LineParams{ 0.0, 0.0, false }); // , int, int);
	void drawRoad();
	//int getPoint(Point);

	Point getPoint(int, LaneType);
	int getPointIndex(Point);
	Point getFirstPointOf(LaneType);
	Point getLastPointOf(LaneType); 
	void* getNextJunction(LaneType, int&);
	void addJunction(Point, LaneType, void*);
	void deleteJunction(void*);
	void deleteFromJunctions();

private:
	std::vector<LanePoint> lane;

	void addPoint(Point, LaneType);
	vectors calc_vectors(Point, Point);
};