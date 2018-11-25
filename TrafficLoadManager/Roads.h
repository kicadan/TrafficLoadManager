#pragma once
#include <vector>
#include "Point.h"
#include <string>
#include <QPoint>

#define distance 10

struct vectors {
	double xl;
	double yl;
	double xr;
	double yr;
};

struct LineParams {
	double a;
	double b;
	bool upright;
	Point firstPoint;
	Point lastPoint;
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
		int id;

		virtual void addPoint(Point, Roads::LaneType) = 0;
		virtual bool drawRoad(QPoint, QPoint, bool, LineParams startBermParams = LineParams{ 0.0, 0.0, false }, LineParams endBermParams = LineParams{ 0.0, 0.0, false }) = 0;//, int, int) = 0;
		Point searchPoint(QPoint);
		void addOtherRoad(int);
		LineParams getBermParams(LaneType);
		LaneType returnCloserBerm(QPoint); //returns berm basing on distance calculated on opposite point to connecting to this road (if firstPoint connecting, lastPoint is the calculation base)
	protected:
		std::vector<Point> mid;
		std::vector<int> otherRoads;
		vectors parallel_segments;
		LineParams midLineParams, bermLParams, bermRParams;
		bool horizontal;
	private:
		virtual vectors calc_vectors(QPoint, QPoint) = 0;
	};

	class OneWayOneLane :
		public Road
	{
	public:
		OneWayOneLane(int);
		~OneWayOneLane();

		void addPoint(Point, Roads::LaneType);
		bool drawRoad(QPoint, QPoint, bool, LineParams startBermParams = LineParams{ 0.0, 0.0, false }, LineParams endBermParams = LineParams{ 0.0, 0.0, false }); // , int, int);

	private:
		std::vector<Point> bermL, bermR, lane;

		vectors calc_vectors(QPoint, QPoint);
	};
}