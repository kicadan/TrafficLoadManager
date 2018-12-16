#include "Roads.h"

Road::Road(ElementType roadType):AppObject(ROAD), _roadType(roadType)
{
}

Point Road::searchPoint(Point point)
{
	for (auto &existingPoint : this->mid) {
		if ((point.x() > floor(existingPoint.x() - distance / 2) && point.x() < ceil(existingPoint.x() + distance / 2)) && (point.y() > floor(existingPoint.y() - distance / 2) && point.y() < ceil(existingPoint.y() + distance / 2)))
			return Point(existingPoint);
	}
	return Point(0, 0);
}

void Road::addOtherRoad(int roadNumber)
{
	this->otherRoads.push_back(roadNumber);
}

QLineF Road::getLineParams(LaneType laneType)
{
	switch (laneType) {
	case LEFT_BERM: return leftBerm;
	case RIGHT_BERM: return rightBerm;
	case LANE: return coreLine;
	}
}

 LaneType Road::getCloserBerm(Point point)
{
	double distanceFromLeftBerm = 0.0;
	double distanceFromRightBerm = 0.0;

	LaneType returnedLane;
	QPointF pointF(point.x(), point.y());
	QLineF perpendicularLine;
	perpendicularLine.setP1(pointF);
	perpendicularLine.setAngle(leftBerm.angle() + 90.0);
	QPointF intersectLeft, intersectRight;
	leftBerm.intersect(perpendicularLine, &intersectLeft);
	rightBerm.intersect(perpendicularLine, &intersectRight);
	qreal distancy = intersectLeft.x() - point.x();
	distanceFromLeftBerm = qSqrt(qPow(intersectLeft.x() - point.x(), 2) + qPow(intersectLeft.y() - point.y(), 2));
	distanceFromRightBerm = qSqrt(qPow(intersectRight.x() - point.x(), 2) + qPow(intersectRight.y() - point.y(), 2));
	return distanceFromLeftBerm > distanceFromRightBerm ? RIGHT_BERM : LEFT_BERM;
}

 ElementType Road::getRoadType()
 {
	 return _roadType;
 }

 ObjectType Road::getObjectType()
 {
	 return _objectType;
 }

 bool Road::doLineCrosses(QLineF theLine)
 {
	bool crossing = false;
	QPointF crossPoint;
	QLineF::IntersectType intersectType;
	intersectType = theLine.intersect(rightBerm, &crossPoint);
	if (intersectType == QLineF::BoundedIntersection)
		crossing = true;
	intersectType = theLine.intersect(leftBerm, &crossPoint);
	if (intersectType == QLineF::BoundedIntersection)
		crossing = true;
	intersectType = theLine.intersect(coreLine, &crossPoint);
	if (intersectType == QLineF::BoundedIntersection)
		crossing = true;
	return crossing;
 }

 Point Road::getFurtherPoint(Point point)
 {
	 Point oppositePoint;
	 if (point == coreLineParams.firstPoint)
		 oppositePoint = mid[mid.size()-1];
	 else
		 oppositePoint = mid[0];
	 return oppositePoint;
 }
