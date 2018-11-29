#pragma once
#include "AppObject.h"
#include "Roads.h"


struct Connection {
	int roadFirst;
	int roadSecond;
	float x1;
	float y1;
	float x2;
	float y2;
};

class Junction :
	public AppObject
{
public:
	Junction();
	Junction(QPoint, Road*, Road*);
	~Junction();
	Point returnCrossPointsForBerm(LineParams, LaneType, QPoint);
	void connectRoads();
	bool isPoint(QPoint);
	bool isPoint(Point);
private:
	Point point;
	std::vector<Connection> connections;
	std::vector<Road*> roads;
};

