#pragma once
#include "AppObject.h"
#include "Point.h"

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
	Junction(Point);
	~Junction();
private:
	Point point;
	std::vector<Connection> connections;
};

