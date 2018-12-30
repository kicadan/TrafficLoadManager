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
#include "TrafficLightsEditor.h"
#include "Automobile.h"

enum Action {
	LOAD_FROM_FILE = 0,
	WRITE_TO_FILE = 1,
	UNDO = 2,
	DRAW_ONE_WAY_ONE_LANE = 3,
	DRAW_ONE_WAY_TWO_LANES = 4,
	DRAW_TWO_WAY_ONE_LANE = 5,
	DRAW_SPAWN_POINT = 6,
	EDIT_SPAWN_POINT = 7,
	MAKE_CONNECTION = 8,
	SET_TRAFFIC_LIGHTS = 9,
	EDIT_TRAFFIC_LIGHTS = 10
};

struct Change {
	AppObject *appObject;
	int changeId;
	Action action;
};

struct Previous {
	int cost = 999999;
	Connection connection;
	Junction junction;
};

struct Node {
	Junction junction;
	std::vector<Previous> previousConnections;
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
	std::vector<Vehicle*> allVehicles;
	std::vector<Change> allChanges;
	std::vector<Node> allNodes;
	std::vector<Way> allWays;
	int changeCounter = 0;
	QTimer* timer;
	int actualScale;
	Point lastPoint;
	Point firstPoint;
	QPointF _lastPoint;
	QPointF _firstPoint;
	bool constructing = false;
	bool editing = false;
	bool repainting = false;
	bool simulationInProgress = false;
	QImage image;
	double x = 10;
	double y = 10;
	GLUquadric * object;

	//void drawLineTo(const QPoint &endPoint);
	void resetNodeTable(Junction);
	void updateNode(Junction, Junction, Connection);
	void recursiveNodeFollow(std::vector<Junction>, Connection, int);
	void collectWay(Junction, Junction);
	void collectWayRecursive(Junction, Connection, int, int, Way&);
	void drawElement();
	void drawRoad();
	void makeConnection();
	void updateWays();
	void renewConnectionsForSpawnPoints(); //deprecated
	void setTrafficLights();
	void editTrafficLights();
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
	void deleteVehicle(Vehicle*);
	void copyAllJunctions(std::vector<Junction>&);
	void fulfillNodeTable(std::vector<Node>&, Junction);
	void transferJunction(std::vector<Junction> &from, std::vector<Junction> &to, Junction junction);
	vectors calc_vectors(QPoint, QPoint);
	void findWay(Junction, Junction);
	void processVehiclesMove();
	void emitCar(Way);
	void processSimulation();

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
	void continueSimulation();
	void startSimulation();
	void stopSimulation();

};
