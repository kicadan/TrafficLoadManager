#pragma once
#include "Roads.h"
#include "Junction.h"

class TwoWayOneLane :
	public Road
{
public:
	TwoWayOneLane(int);
	~TwoWayOneLane(); 
	void setRoad(QPointF, QPointF, Junction*, Junction*);// LineParams startBermParams = LineParams{ 0.0, 0.0, false }, LineParams endBermParams = LineParams{ 0.0, 0.0, false }); // , int, int);
	void drawRoad();
	//int getPoint(Point);

	Point getPoint(int, LaneType);
	Point getFirstPointOf(LaneType);
	Point getLastPointOf(LaneType);
	Point getStartPointForConnection(int, LaneType);
	Point getEndPointForConnection(int, LaneType);
	LaneType searchPointForLaneType(Point);
	int getPointIndex(Point, LaneType);
	int getUsageOfLane(LaneType);
	void* getNextJunction(LaneType, int&);
	void* getPreviousJunction(LaneType, int&);
	void* searchForClosestJunction(Point, LaneType);
	void addJunction(Point, void*);
	void deleteJunction(void*);
	void deleteFromJunctions();
	void processAllVehicles(std::vector<Vehicle*>&);
	void freePoint(LaneType, int);
	bool reservePoint(LaneType, Vehicle*, int);
	void updateLane();

private:
	std::vector<LanePoint> backLane, lane;
	int percentageBackLaneUsage = 0;
	int percentageLaneUsage = 0;

	void addPoint(Point, LaneType);
	vectors calc_vectors(QPointF, QPointF);
};
