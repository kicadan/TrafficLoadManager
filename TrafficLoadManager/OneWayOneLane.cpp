#include "OneWayOneLane.h"

OneWayOneLane::OneWayOneLane(int _id)
{
	this->Road::id = _id;
	this->Road::roadType = OneWayRoadWithOneLane;
}

OneWayOneLane::~OneWayOneLane()
{
}

void OneWayOneLane::addPoint(Point point, LaneType pointType)
{
	switch (pointType) {
	case LANE: { lane.push_back(point); break; }
	case LEFT_BERM: { bermL.push_back(point); break; }
	case RIGHT_BERM: bermR.push_back(point);
	}
}

void OneWayOneLane::drawRoad(QPoint firstPoint, QPoint lastPoint, bool endingToOtherRoad, Junction* startJunction, Junction* endJunction)//std::vector<Road*> startRoads, std::vector<Road*> endRoads)//LineParams startBermParams, LineParams endBermParams)//, int beginRoad, int endRoad)
{
	glLineWidth(3);

	this->parallel_segments = calc_vectors(lastPoint, firstPoint);
	double x, y, A, B, bermLeftA, bermLeftB, bermRightA, bermRightB;
	bool upright = false;
	//equation of straight line
	if (!(firstPoint.x() == lastPoint.x()))
	{
		A = (lastPoint.y() - firstPoint.y()) / (double)(lastPoint.x() - firstPoint.x());
		B = firstPoint.y() - A * firstPoint.x();
		bermLeftA = A;
		bermLeftB = firstPoint.y() + parallel_segments.yl - A * (firstPoint.x() + parallel_segments.xl);
		bermRightA = A;
		bermRightB = (firstPoint.y() + parallel_segments.yr) - A * (firstPoint.x() + parallel_segments.xr);
	}
	else
	{
		A = 1;
		B = -firstPoint.x();
		upright = true;
	}
	this->Road::midLineParams.a = A;
	this->Road::midLineParams.b = B;
	this->Road::midLineParams.upright = upright;

	this->Road::bermLParams.a = bermLeftA;
	this->Road::bermLParams.b = bermLeftB;
	this->Road::bermLParams.upright = upright;
	this->Road::bermRParams.a = bermRightA;
	this->Road::bermRParams.b = bermRightB;
	this->Road::bermRParams.upright = upright;


	//if ( (A == startBermParams.a && upright == startBermParams.upright) || (A == endBermParams.a && upright == endBermParams.upright) )
	//{//dont draw if parallel to any connected line
	//	return false;
	//}

	//check if it is more horizontal or vertical lane
	double dx = (lastPoint.x() - firstPoint.x())> 0 ? (lastPoint.x() - firstPoint.x()) : -(lastPoint.x() - firstPoint.x());
	double dy = (lastPoint.y() - firstPoint.y())> 0 ? (lastPoint.y() - firstPoint.y()) : -(lastPoint.y() - firstPoint.y());
	if (dx > dy)
		horizontal = true;
	else
		horizontal = false;
	//create vectors of the roads
	double xm = firstPoint.x();
	double xl = firstPoint.x() + parallel_segments.xl;
	double xr = firstPoint.x() + parallel_segments.xr;
	double _x = xm;
	double _y = 0.0;
	if (horizontal)
	{
		// y = A * x + B
		double x_last = lastPoint.x();
		if (lastPoint.x() > firstPoint.x())
			for (_x = xm; _x <= x_last; _x += distance, xl += distance, xr += distance) {
				addPoint(Point(_x, A*_x + B), LANE);
				addPoint(Point(xl, A*xl + B), LEFT_BERM);
				addPoint(Point(xr, A*xr + B), RIGHT_BERM);
			}
		else
			for (_x = xm; _x >= x_last; _x -= distance, xl -= distance, xr -= distance) {
				addPoint(Point(_x, A*_x + B), LANE);
				addPoint(Point(xl, A*xl + B), LEFT_BERM);
				addPoint(Point(xr, A*xr + B), RIGHT_BERM);
			}
	}
	else
	{
		// x = (y - B) / A if dx != 0 else x = point.x
		double ym = firstPoint.y();
		double yl = firstPoint.y() + parallel_segments.yl;
		double yr = firstPoint.y() + parallel_segments.yr;
		double y_last = lastPoint.y();
		if (lastPoint.y() > firstPoint.y())
			for (_y = ym; _y <= y_last; _y += distance, yl += distance, yr += distance) {
				addPoint(Point(dx != 0 ? (_y - B) / A : xm, _y), LANE);
				addPoint(Point(dx != 0 ? (yl - B) / A : xl, yl), LEFT_BERM);
				addPoint(Point(dx != 0 ? (yr - B) / A : xr, yr), RIGHT_BERM);
			}
		else
			for (_y = ym; _y >= y_last; _y -= distance, yl -= distance, yr -= distance) {
				addPoint(Point(dx != 0 ? (_y - B) / A : xm, _y), LANE);
				addPoint(Point(dx != 0 ? (yl - B) / A : xl, yl), LEFT_BERM);
				addPoint(Point(dx != 0 ? (yr - B) / A : xr, yr), RIGHT_BERM);
			}
	}
	//assing real values FROM VECTOR to lastPoint if road is not connecting to the other at its end
	if (!endingToOtherRoad) {
		if (_y == 0)
		{
			if (lastPoint.x() > firstPoint.x())
				_x -= distance;
			else
				_x += distance;
			_y = A * _x + B;
		}
		else
		{
			if (lastPoint.y() > firstPoint.y())
				_y -= distance;
			else
				_y += distance;
			_x = dx != 0 ? (_y - B) / A : xm;
		}
		lastPoint = QPoint(_x, _y);
	}

	Point _firstBermLeftPoint(0, 0), _firstBermRightPoint(0, 0), _lastBermLeftPoint(0, 0), _lastBermRightPoint(0, 0);
	if (startJunction != NULL) {
		_firstBermLeftPoint = startJunction->returnCrossPointsForBerm(bermLParams, LEFT_BERM, lastPoint);
		_firstBermRightPoint = startJunction->returnCrossPointsForBerm(bermRParams, RIGHT_BERM, lastPoint);
	}
	if (endJunction != NULL) {
		_lastBermLeftPoint = endJunction->returnCrossPointsForBerm(bermLParams, LEFT_BERM, firstPoint);
		_lastBermRightPoint = endJunction->returnCrossPointsForBerm(bermRParams, RIGHT_BERM, firstPoint);
	}
	//main lane
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);
	midLineParams.firstPoint = Point(firstPoint.x(), firstPoint.y());
	midLineParams.lastPoint = Point(lastPoint.x(), lastPoint.y());
	glVertex2f(lastPoint.x(), lastPoint.y());
	glVertex2f(firstPoint.x(), firstPoint.y());
	glEnd();

	//berm left
	glBegin(GL_LINES);
	glColor3f(0.0f, 1.0f, 0.0f);
	x = _firstBermLeftPoint.x() == 0 ? firstPoint.x() + parallel_segments.xl : _firstBermLeftPoint.x();
	y = _firstBermLeftPoint.y() == 0 ? firstPoint.y() + parallel_segments.yl : _firstBermLeftPoint.y();
	bermLParams.firstPoint = Point(x, y);
	glVertex2f(x, y);
	x = _lastBermLeftPoint.x() == 0 ? lastPoint.x() + parallel_segments.xl : _lastBermLeftPoint.x();
	y = _lastBermLeftPoint.y() == 0 ? lastPoint.y() + parallel_segments.yl : _lastBermLeftPoint.y();
	bermLParams.lastPoint = Point(x, y);
	glVertex2f(x, y);
	glEnd();

	//berm right
	glBegin(GL_LINES);
	glColor3f(0.0f, 1.0f, 0.0f);
	x = _lastBermRightPoint.x() == 0 ? lastPoint.x() + parallel_segments.xr : _lastBermRightPoint.x();
	y = _lastBermRightPoint.y() == 0 ? lastPoint.y() + parallel_segments.yr : _lastBermRightPoint.y();
	bermRParams.lastPoint = Point(x, y);
	glVertex2f(x, y);
	x = _firstBermRightPoint.x() == 0 ? firstPoint.x() + parallel_segments.xr : _firstBermRightPoint.x();
	y = _firstBermRightPoint.y() == 0 ? firstPoint.y() + parallel_segments.yr : _firstBermRightPoint.y();
	bermRParams.firstPoint = Point(x, y);
	glVertex2f(x, y);
	glEnd();
	//assign lane to core of the road
	this->Road::mid.assign(this->lane.begin(), this->lane.end());
}

vectors OneWayOneLane::calc_vectors(QPoint A, QPoint B)
{
	//int xa = A.x() - B.x();
	//int ya = A.y() - B.y();
	//double xd = pow(odl, 2);
	//double delta = sqrt(1 + 4 * pow(odl, 2) * (pow(ya, 2) / pow(xa, 2) ));
	//double ya_sq = ya ^ 2;
	//return vectors { /*x1*/(1 - delta) / (2 * (pow(ya, 2) / pow(xa, 2))),
	///*y1*/ (-xa * (1 - delta) / (2 * (pow(ya, 2) / pow(xa, 2)))) / ya,
	///*x2*/ (1 + delta) / (2 * (pow(ya, 2) / pow(xa, 2))),
	///*y2*/ (-xa * (1 + delta) / (2 * (pow(ya, 2) / pow(xa, 2)))) / ya };
	double xa = A.x() - B.x();
	double ya = A.y() - B.y();
	double radius = sqrt(xa*xa + ya * ya);
	return vectors{ ya*distance / radius, -xa * distance / radius, -ya * distance / radius, xa * distance / radius };
}