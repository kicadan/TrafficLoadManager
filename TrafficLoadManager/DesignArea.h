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
#include "SpawnSettingsEditor.h"
#include "Roads.h"
#include "OneWayOneLane.h"
#include "OneWayTwoLanes.h"
#include "TwoWayOneLane.h"

enum Action {
	LOAD_FROM_FILE = 0,
	WRITE_TO_FILE = 1,
	UNDO = 2,
	DRAW_ONE_WAY_ONE_LANE = 3,
	DRAW_ONE_WAY_TWO_LANES = 4,
	DRAW_TWO_WAY_ONE_LANE = 5,
	DRAW_SPAWN_POINT = 6,
	EDIT_SPAWN_POINT = 7,
	MAKE_CONNECTION = 8
};

struct Change {
	AppObject *appObject;
	int changeId;
	Action action;
};

struct Way {
	Junction from;
	Junction to;
	std::vector<Connection> steps;
	int length;
};

struct Node {
	Junction junction;
	int cost = 999999;
	Junction previousJunction;
	std::vector<Connection> connections;
};

class DesignArea : public QOpenGLWidget
{
	Q_OBJECT

public:
	explicit DesignArea(QWidget *parent = 0);
	~DesignArea();

	void dispatchAction(Action);
	
private:
	QOpenGLFunctions *openGLFunctions;
	QOpenGLContext *context;
	ElementType currentObjectBrush = OneWayRoadWithOneLane;
	std::vector<Road*> allRoads;
	std::vector<Junction*> allJunctions;
	std::vector<Change> allChanges;
	std::vector<Way> allWays;
	int changeCounter = 0;
	QTimer timer;
	int actualScale;
	Point lastPoint;
	Point firstPoint;
	QPointF _lastPoint;
	QPointF _firstPoint;
	bool constructing = false;
	bool editing = false;
	bool repainting = false;
	QImage image;
	double x = 10;
	double y = 10;
	GLUquadric * object;

	//void drawLineTo(const QPoint &endPoint);
	void drawElement();
	void drawRoad();
	void makeConnection();
	void renewConnectionsForSpawnPoints(); //deprecated
	void addCarSpawn();
	void editCarSpawn();
	void repaintScene();
	void undoChanges();
	void validateConnections();
	void addChanges(std::vector<AppObject*>); //deprecated
	Point searchPoint(QPoint);
	bool checkIfCollidingWithOtherRoad(Road*, std::vector<int>);
	void deleteJunction(Junction*);
	void deleteRoad(Road*);
	void copyAllJunctions(std::vector<Junction>&);
	void fulfillNodeTable(std::vector<Node>&, Junction);
	void transferJunction(std::vector<Junction> &from, std::vector<Junction> &to, Junction junction);
	void updateConnectionIfCloser(Junction, std::vector<Node>&, Connection);
	Junction getClosestJunction(std::vector<Node> allNodes, std::vector<Junction> Qset);
	vectors calc_vectors(QPoint, QPoint);
	Way makeWayFromNodes(std::vector<Node>, Junction, Junction);
	Way findWay(Junction, Junction);

protected:
	
	void mousePressEvent(QMouseEvent *event) override;
	//void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void resizeEvent(QResizeEvent *event) override;
	void paintEvent(QPaintEvent *event) override;
	virtual void initializeGL();
	virtual void resizeGL(int w, int h);
	virtual void paintGL();


public slots:
	void handleAction();

};
