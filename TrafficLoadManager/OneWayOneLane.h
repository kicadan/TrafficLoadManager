#pragma once

#include "Junction.h"


class OneWayOneLane :
	public Road
{
public:
	OneWayOneLane(int);
	~OneWayOneLane();

	void addPoint(Point, LaneType);
	void drawRoad(QPoint, QPoint, bool, Junction*, Junction*);// LineParams startBermParams = LineParams{ 0.0, 0.0, false }, LineParams endBermParams = LineParams{ 0.0, 0.0, false }); // , int, int);

private:
	std::vector<Point> bermL, bermR, lane;

	vectors calc_vectors(QPoint, QPoint);
};