#include "Roads.h"

Point Roads::Road::searchPoint(QPoint point)
{
	for (auto &existingPoint : this->mid) {
		if ((point.x() > floor(existingPoint.x() - distance / 2) && point.x() < ceil(existingPoint.x() + distance / 2)) && (point.y() > floor(existingPoint.y() - distance / 2) && point.y() < ceil(existingPoint.y() + distance / 2)))
			return Point(existingPoint);
	}
	return Point(0, 0);
}

void Roads::Road::addOtherRoad(int roadNumber)
{
	this->otherRoads.push_back(roadNumber);
}

LineParams Roads::Road::getBermParams(LaneType laneType)
{
	return laneType == LEFT_BERM ? bermLParams : bermRParams;
}

 Roads::LaneType Roads::Road::returnCloserBerm(QPoint point)
{
	float distanceFromLeftBerm = 0.0;
	float distanceFromRightBerm = 0.0;
	distanceFromLeftBerm = fabs(-bermLParams.a*point.x() + point.y() - bermLParams.b) / (float)sqrt(pow(bermLParams.a, 2) + 1);
	distanceFromRightBerm = fabs(-bermRParams.a*point.x() + point.y() - bermRParams.b) / (float)sqrt(pow(bermRParams.a, 2) + 1);
	return distanceFromLeftBerm > distanceFromRightBerm ? Roads::RIGHT_BERM : Roads::LEFT_BERM;
}

Roads::OneWayOneLane::OneWayOneLane(int _id)
{
	this->Road::id = _id;
}

Roads::OneWayOneLane::~OneWayOneLane()
{
}

void Roads::OneWayOneLane::addPoint(Point point, LaneType pointType)
{
	switch (pointType) {
	case LANE: { lane.push_back(point); break; }
	case LEFT_BERM: { bermL.push_back(point); break; }
	case RIGHT_BERM: bermR.push_back(point);
	}
}

bool Roads::OneWayOneLane::drawRoad(QPoint firstPoint, QPoint lastPoint, bool endingToOtherRoad, LineParams startBermParams, LineParams endBermParams)//, int beginRoad, int endRoad)
{
	glLineWidth(3);

	this->parallel_segments = calc_vectors(lastPoint, firstPoint);
	float x, y, A, B, bermLeftA, bermLeftB, bermRightA, bermRightB;
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

	if ( (A == startBermParams.a && upright == startBermParams.upright) || (A == endBermParams.a && upright == endBermParams.upright) )
	{//dont draw if parallel to any connected line
		return false;
	}

	//check if it is more horizontal or vertical lane
	float dx = ( lastPoint.x() - firstPoint.x() )> 0 ? (lastPoint.x() - firstPoint.x()) : -(lastPoint.x() - firstPoint.x());
	float dy = ( lastPoint.y() - firstPoint.y() )> 0 ? (lastPoint.y() - firstPoint.y()) : -(lastPoint.y() - firstPoint.y());
	if (dx > dy)
		horizontal = true;
	else
		horizontal = false;
	//create vectors of the roads
	float xm = firstPoint.x();
	float xl = firstPoint.x() + parallel_segments.xl;
	float xr = firstPoint.x() + parallel_segments.xr;
	float _x = xm;
	float _y = 0.0;
	if (horizontal)
	{
		// y = A * x + B
		float x_last = lastPoint.x();
		if (lastPoint.x() > firstPoint.x())
			for (_x = xm; _x <= x_last; _x += distance, xl += distance, xr += distance) {
				addPoint(Point(_x, A*_x + B), Roads::LANE);
				addPoint(Point(xl, A*xl + B), Roads::LEFT_BERM);
				addPoint(Point(xr, A*xr + B), Roads::RIGHT_BERM);
			}
		else
			for (_x = xm; _x >= x_last; _x -= distance, xl -= distance, xr -= distance) {
				addPoint(Point(_x, A*_x + B), Roads::LANE);
				addPoint(Point(xl, A*xl + B), Roads::LEFT_BERM);
				addPoint(Point(xr, A*xr + B), Roads::RIGHT_BERM);
			}
	}
	else
	{
		// x = (y - B) / A if dx != 0 else x = point.x
		float ym = firstPoint.y();
		float yl = firstPoint.y() + parallel_segments.yl;
		float yr = firstPoint.y() + parallel_segments.yr;
		float y_last = lastPoint.y();
		if (lastPoint.y() > firstPoint.y())
			for (_y = ym; _y <= y_last; _y += distance, yl += distance, yr += distance) {
				addPoint(Point(dx != 0 ? (_y - B) / A : xm, _y), Roads::LANE);
				addPoint(Point(dx != 0 ? (yl - B) / A : xl, yl), Roads::LEFT_BERM);
				addPoint(Point(dx != 0 ? (yr - B) / A : xr, yr), Roads::RIGHT_BERM);
			}
		else
			for (_y = ym; _y >= y_last; _y -= distance, yl -= distance, yr -= distance) {
				addPoint(Point(dx != 0 ? (_y - B) / A : x, _y), Roads::LANE);
				addPoint(Point(dx != 0 ? (yl - B) / A : xl, yl), Roads::LEFT_BERM);
				addPoint(Point(dx != 0 ? (yr - B) / A : xr, yr), Roads::RIGHT_BERM);
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

	Point _firstBermLeftPoint(0.0, 0.0), _firstBermRightPoint(0.0, 0.0), _lastBermLeftPoint(0.0, 0.0), _lastBermRightPoint(0.0, 0.0);
	if (startBermParams.a != 0 || startBermParams.b != 0)
	{
		float x = midLineParams.upright ? -midLineParams.b : (startBermParams.b - this->bermLParams.b) / (this->bermLParams.a - startBermParams.a); //dzielenie przez 0
		_firstBermLeftPoint = Point(x, startBermParams.a*x + startBermParams.b);
		x = midLineParams.upright ? -midLineParams.b : (startBermParams.b - this->bermRParams.b) / (this->bermRParams.a - startBermParams.a); //dzielenie przez 0
		_firstBermRightPoint = Point(x, startBermParams.a*x + startBermParams.b);

		//erase part of existing berm which would cut the road
		glBegin(GL_LINES);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex2f(_firstBermLeftPoint.x(), _firstBermLeftPoint.y());
		glVertex2f(_firstBermRightPoint.x(), _firstBermRightPoint.y());
		glEnd();
	}

	if (endBermParams.a != 0 || endBermParams.b != 0)
	{
		float x = midLineParams.upright ? -midLineParams.b : (endBermParams.b - this->bermLParams.b) / (this->bermLParams.a - endBermParams.a); //dzielenie przez 0
		_lastBermLeftPoint = Point(x, endBermParams.a*x + endBermParams.b);
		x = midLineParams.upright ? -midLineParams.b : (endBermParams.b - this->bermRParams.b) / (this->bermRParams.a - endBermParams.a); //dzielenie przez 0
		_lastBermRightPoint = Point(x, endBermParams.a*x + endBermParams.b);
		
		//erase part of existing berm which would cut the road
		glBegin(GL_LINES);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex2f(_lastBermLeftPoint.x(), _lastBermLeftPoint.y());
		glVertex2f(_lastBermRightPoint.x(), _lastBermRightPoint.y());
		glEnd();
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
	return true;
}

vectors Roads::OneWayOneLane::calc_vectors(QPoint A, QPoint B)
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
