#include "Junction.h"



Junction::Junction()
{
}

Junction::Junction(QPoint _point, Road* firstRoad, Road* secondRoad)
{
	this->point = Point(_point.x(), _point.y());
	roads.push_back(firstRoad);
	roads.push_back(secondRoad);
}


Junction::~Junction()
{
}

Point Junction::returnCrossPointsForBerm(LineParams lineParams, LaneType laneType, QPoint oppositePoint)
{
	std::vector<Point> points;
	LineParams rightBermParams;
	LineParams leftBermParams;
	Point point = Point(0, 0);
	double x, y;
	for (auto road : this->roads) {
		LineParams leftBermParams = road->getBermParams(LEFT_BERM);
		LineParams rightBermParams = road->getBermParams(RIGHT_BERM);
			//left point
			if (!lineParams.upright && !leftBermParams.upright && leftBermParams.a != lineParams.a) { //first and second line are not parallel and not upright
				x = (leftBermParams.b - lineParams.b) / (lineParams.a - leftBermParams.a);
				y = x * leftBermParams.a + leftBermParams.b;
				if (!(x >= min(leftBermParams.firstPoint.x(), leftBermParams.lastPoint.x()) && x <= max(leftBermParams.firstPoint.x(), leftBermParams.lastPoint.x()) && y >= min(leftBermParams.firstPoint.y(), leftBermParams.lastPoint.y()) && y <= max(leftBermParams.firstPoint.y(), leftBermParams.lastPoint.y()))) { //if not on short
					x = 0;
					y = 0;
				}
			}
			else if (lineParams.upright) {
				x = (leftBermParams.b - lineParams.b) / (lineParams.a - leftBermParams.a);
				y = x * leftBermParams.a + leftBermParams.b;
				if (!(x >= min(leftBermParams.firstPoint.x(), leftBermParams.lastPoint.x()) && x <= max(leftBermParams.firstPoint.x(), leftBermParams.lastPoint.x()) && y >= min(leftBermParams.firstPoint.y(), leftBermParams.lastPoint.y()) && y <= max(leftBermParams.firstPoint.y(), leftBermParams.lastPoint.y()))) { //if not on short
					x = 0;
					y = 0;
				}
			}
			else if (leftBermParams.upright) {
				x = (leftBermParams.b - lineParams.b) / (lineParams.a - leftBermParams.a);
				y = x * lineParams.a + lineParams.b;
				if (!(x >= min(leftBermParams.firstPoint.x(), leftBermParams.lastPoint.x()) && x <= max(leftBermParams.firstPoint.x(), leftBermParams.lastPoint.x()) && y >= min(leftBermParams.firstPoint.y(), leftBermParams.lastPoint.y()) && y <= max(leftBermParams.firstPoint.y(), leftBermParams.lastPoint.y()))) { //if not on short
					x = 0;
					y = 0;
				}
			}
			else { //lines are parallel
				continue;
			}
			if (x != 0 && y != 0)
				points.push_back(Point(x, y));

			//right point
			if (!lineParams.upright && !rightBermParams.upright && rightBermParams.a != lineParams.a) { //first and second line are not parallel and not upright
				x = (rightBermParams.b - lineParams.b) / (lineParams.a - rightBermParams.a);
				y = x * rightBermParams.a + rightBermParams.b;
				if (!(x >= min(rightBermParams.firstPoint.x(), rightBermParams.lastPoint.x()) && x <= max(rightBermParams.firstPoint.x(), rightBermParams.lastPoint.x()) && y >= min(rightBermParams.firstPoint.y(), rightBermParams.lastPoint.y()) && y <= max(rightBermParams.firstPoint.y(), rightBermParams.lastPoint.y()))) { //if not on short
					x = 0;
					y = 0;
				}
			}
			else if (lineParams.upright) {
				x = (rightBermParams.b - lineParams.b) / (lineParams.a - rightBermParams.a);
				y = x * rightBermParams.a + rightBermParams.b;
				if (!(x >= min(rightBermParams.firstPoint.x(), rightBermParams.lastPoint.x()) && x <= max(rightBermParams.firstPoint.x(), rightBermParams.lastPoint.x()) && y >= min(rightBermParams.firstPoint.y(), rightBermParams.lastPoint.y()) && y <= max(rightBermParams.firstPoint.y(), rightBermParams.lastPoint.y()))) { //if not on short
					x = 0;
					y = 0;
				}
			}
			else if (rightBermParams.upright) {
				x = (rightBermParams.b - lineParams.b) / (lineParams.a - rightBermParams.a);
				y = x * lineParams.a + lineParams.b;
				if (!(x >= min(rightBermParams.firstPoint.x(), rightBermParams.lastPoint.x()) && x <= max(rightBermParams.firstPoint.x(), rightBermParams.lastPoint.x()) && y >= min(rightBermParams.firstPoint.y(), rightBermParams.lastPoint.y()) && y <= max(rightBermParams.firstPoint.y(), rightBermParams.lastPoint.y()))) { //if not on short
					x = 0;
					y = 0;
				}
			}
			else { //lines are parallel
				continue;
			}
			if (x != 0 && y != 0)
				points.push_back(Point(x, y));
	}
	double minimumDistance = 9999999.0, minimumRightDistance = 9999999.0;
	double dist;
	for (auto _point : points) {
		dist = sqrt(pow(oppositePoint.x() - _point.x(), 2) + pow(oppositePoint.y() - _point.y(), 2));
		if (dist < minimumDistance) {
			minimumDistance = dist;
			point = _point;
		}
	}
	return point;
}

void Junction::connectRoads()
{
}

bool Junction::isPoint(QPoint point)
{
	return this->point == point;
}

bool Junction::isPoint(Point point)
{
	return this->point == point;
}
