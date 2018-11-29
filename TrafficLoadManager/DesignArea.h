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
#include "Roads.h"
#include "OneWayOneLane.h"


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
	QTimer timer;
	int actualScale;
	QPoint lastPoint;
	QPoint firstPoint;
	bool constructing = false;
	QImage image;
	double x = 10;
	double y = 10;
	GLUquadric * object;

	//void drawLineTo(const QPoint &endPoint);
	void drawRoad();
	Point searchPoint(QPoint);
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
