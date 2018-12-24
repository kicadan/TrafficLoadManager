#pragma once

#include <QOpenGLFunctions>
#include <QtOpenGL>
#include <gl\GLU.h>
#include <string>

enum ObjectType {
	ROAD = 0,
	JUNCTION = 1,
	APP_POINT = 2
};

class AppObject
{
public:
	AppObject(ObjectType);
	AppObject();
	~AppObject();
	virtual ObjectType getObjectType() = 0;
	//add one virtual method and change this to non virtual (delete implementation in child classes) !!!!VIRTUAL METHOD NEEDED!!!

protected:
	ObjectType _objectType;
};

double min(double, double);
double max(double, double);

