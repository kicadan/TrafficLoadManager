#pragma once
#include <vector>
#include "Point.h"
#include <string>
#include <QPoint>

#define distance 10

struct vectors {
	double x1;
	double y1;
	double x2;
	double y2;
};

namespace Roads
{

	enum LaneType
	{
		LEFT_BERM = 0,
		RIGHT_BERM = 1,
		LANE = 2,
		LEFT_LANE = 3,
		RIGHT_LANE = 4
	};

	class Road :
		public AppObject
	{
	public:
		virtual void addPoint(Point, Roads::LaneType) = 0;
		virtual void drawLane(QPoint, QPoint) = 0;
	private:
		virtual vectors calc_vectors(QPoint, QPoint) = 0;
	};

	class OneWayOneLane :
		public Road
	{
	public:
		OneWayOneLane();
		~OneWayOneLane();

		void addPoint(Point, Roads::LaneType);
		void drawLane(QPoint, QPoint);

	private:
		std::vector<Point> bermL, bermR, lane;

		vectors calc_vectors(QPoint, QPoint);
	};

}