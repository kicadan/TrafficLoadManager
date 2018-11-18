#include "Roads.h"

Roads::OneWayOneLane::OneWayOneLane()
{
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

void Roads::OneWayOneLane::drawLane(QPoint firstPoint, QPoint lastPoint)
{
	glLineWidth(3);
	float x, y, A, B;
	//equation of straight line
	if (!(firstPoint.x() == lastPoint.x()))
	{
		A = (lastPoint.y() - firstPoint.y()) / (float)(lastPoint.x() - firstPoint.x());
		B = firstPoint.y() - A * firstPoint.x();
	}
	else 
	{ 
		A = 0;
		B = firstPoint.y(); 
	}
	vectors parallel_segments = calc_vectors(lastPoint, firstPoint);
	//check if it is more horizontal or vertical lane
	float dx = ( lastPoint.x() - firstPoint.x() )> 0 ? (lastPoint.x() - firstPoint.x()) : -(lastPoint.x() - firstPoint.x());
	float dy = ( lastPoint.y() - firstPoint.y() )> 0 ? (lastPoint.y() - firstPoint.y()) : -(lastPoint.y() - firstPoint.y());
	bool horizontal;
	if (dx > dy)
		horizontal = true;
	else
		horizontal = false;

	//main lane
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(lastPoint.x(), lastPoint.y());
	glVertex2f(firstPoint.x(), firstPoint.y());
	glEnd();
	x = round(firstPoint.x());
	if (horizontal)
	{
		float x_last = round(lastPoint.x());
		if (lastPoint.x() > firstPoint.x())
			for (float _x = x; _x <= x_last; _x += distance) {
				addPoint(Point(_x, A*_x + B), Roads::LANE);
			}
		else
			for (float _x = x; _x >= x_last; _x -= distance) {
				addPoint(Point(_x, A*_x + B), Roads::LANE);
			}
	}
	else
	{
		y = round(firstPoint.y());
		float y_last = round(lastPoint.y());
		if(lastPoint.y() > firstPoint.y())
			for (float _y = y; _y <= y_last; _y += distance) {
				addPoint(Point( dx != 0 ? (_y - B)/A : x, _y), Roads::LANE);
			}
		else
			for (float _y = y; _y >= y_last; _y -= distance) {
				addPoint(Point( dx != 0 ? (_y - B) / A : x, _y), Roads::LANE);
			}
	}

	//berm left
	glBegin(GL_LINES);
	glColor3f(0.0f, 1.0f, 0.0f);
	x = round(firstPoint.x() + parallel_segments.x1);
	y = round(firstPoint.y() + parallel_segments.y1);
	glVertex2f(x, y);
	x = round(lastPoint.x() + parallel_segments.x1);
	y = round(lastPoint.y() + parallel_segments.y1);
	glVertex2f(x, y);
	glEnd();
	x = round(firstPoint.x() + parallel_segments.x1);
	if (horizontal)
	{
		float x_last = round(lastPoint.x() + parallel_segments.x1);
		if (lastPoint.x() > firstPoint.x())
			for (float _x = x; _x <= x_last; _x += distance) {
				addPoint(Point(_x, A*_x + B), Roads::LEFT_BERM);
			}
		else
			for (float _x = x; _x >= x_last; _x -= distance) {
				addPoint(Point(_x, A*_x + B), Roads::LEFT_BERM);
			}
	}
	else
	{
		y = round(firstPoint.y() + parallel_segments.y1);
		float y_last = round(lastPoint.y() + parallel_segments.y1);
		if (lastPoint.y() > firstPoint.y())
			for (float _y = y; _y <= y_last; _y += distance) {
				addPoint(Point( dx != 0 ? (_y - B) / A : x, _y), Roads::LEFT_BERM);
			}
		else
			for (float _y = y; _y >= y_last; _y -= distance) {
				addPoint(Point( dx != 0 ? (_y - B) / A : x, _y), Roads::LEFT_BERM);
			}
	}

	//berm right
	glBegin(GL_LINES);
	glColor3f(0.0f, 1.0f, 0.0f);
	x = round(lastPoint.x() + parallel_segments.x2);
	y = round(lastPoint.y() + parallel_segments.y2);
	glVertex2f(x, y);
	x = round(firstPoint.x() + parallel_segments.x2);
	y = round(firstPoint.y() + parallel_segments.y2);
	glVertex2f(x, y);
	glEnd();
	x = round(firstPoint.x() + parallel_segments.x2);
	if (horizontal)
	{
		float x_last = round(lastPoint.x() + parallel_segments.x2);
		if (lastPoint.x() > firstPoint.x())
		for (float _x = x; _x <= x_last; _x += distance) {
			addPoint(Point(_x, A*_x + B), Roads::RIGHT_BERM);
		}
		else
			for (float _x = x; _x >= x_last; _x -= distance) {
				addPoint(Point(_x, A*_x + B), Roads::RIGHT_BERM);
			}
	}
	else
	{
		y = round(firstPoint.y() + parallel_segments.y2);
		float y_last = round(lastPoint.y() + parallel_segments.y2);
		if (lastPoint.y() > firstPoint.y())
			for (float _y = y; _y <= y_last; _y += distance) {
				addPoint(Point( dx != 0 ? (_y - B) / A : x, _y), Roads::RIGHT_BERM);
			}
		else
			for (float _y = y; _y >= y_last; _y -= distance) {
				addPoint(Point( dx != 0 ? (_y - B) / A : x, _y), Roads::RIGHT_BERM);
			}
	}
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