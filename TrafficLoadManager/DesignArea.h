#pragma once

#include <QWidget>
#include <QOpenGLWindow>
#include <QOpenGLWidget>
#include <QSurfaceFormat>
#include <QTimer>
#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QtOpenGL>
#include <QPoint>
#include <gl\GLU.h>
#include <algorithm>
#include "Roads.h"
#include "OneWayOneLane.h"

struct Change {
	AppObject *appObject;
	int changeId;
};

class DesignArea : public QOpenGLWidget
{
	Q_OBJECT

public:
	explicit DesignArea(QWidget *parent = 0);
	~DesignArea();

private:
	QOpenGLFunctions *openGLFunctions;
	QOpenGLContext *context;
	RoadType currentObjectBrush = OneWayRoadWithOneLane;
	std::vector<Road*> allRoads;
	std::vector<Junction*> allJunctions;
	std::vector<Change> allChanges;
	int changeCounter = 0;
	QTimer timer;
	int actualScale;
	QPoint lastPoint;
	QPoint firstPoint;
	Point _lastPoint;
	Point _firstPoint;
	bool constructing = false;
	bool repainting = false;
	bool undo = false;
	QImage image;
	double x = 10;
	double y = 10;
	GLUquadric * object;

	//void drawLineTo(const QPoint &endPoint);
	void drawRoad();
	void repaintScene();
	void undoChanges();
	void addChanges(std::vector<AppObject*>);
	void makeConnections();
	Point searchPoint(QPoint);
	bool checkIfCollidingWithOtherRoad(Road*, std::vector<int>);
	void deleteJunction(Junction*);
	void deleteRoad(Road*);
	vectors calc_vectors(QPoint, QPoint);

protected:
	
	void mousePressEvent(QMouseEvent *event) override;
	//void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void resizeEvent(QResizeEvent *event) override;
	void paintEvent(QPaintEvent *event) override;
	virtual void initializeGL();
	virtual void resizeGL(int w, int h);
	virtual void paintGL();
};
