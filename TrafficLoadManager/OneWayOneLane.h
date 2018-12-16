#pragma once

#include "Junction.h"


class OneWayOneLane :
	public Road
{
public:
	OneWayOneLane(int);
	~OneWayOneLane();

	void setRoad(QPointF, QPointF, bool, Junction*, Junction*);// LineParams startBermParams = LineParams{ 0.0, 0.0, false }, LineParams endBermParams = LineParams{ 0.0, 0.0, false }); // , int, int);
	void drawRoad();
	//int getPoint(Point);

	Point getPoint(int, LaneType);
	int getPointIndex(Point);
	int getUsageOfLane(LaneType);
	Point getFirstPointOf(LaneType);
	Point getLastPointOf(LaneType); 
	void* getNextJunction(LaneType, int&);
	void addJunction(Point, LaneType, void*);
	void deleteJunction(void*);
	void deleteFromJunctions();
	void freePoint(LaneType, int);
	bool reservePoint(LaneType, int);
	void updateLane();

private:
	std::vector<LanePoint> lane;
	int percentageLaneUsage = 0;

	void addPoint(Point, LaneType);
	vectors calc_vectors(QPointF, QPointF);
};