#include "AppObject.h"



AppObject::AppObject(ObjectType objectType):_objectType(objectType)
{
}

AppObject::AppObject()
{
}


AppObject::~AppObject()
{
}

ObjectType AppObject::getObjectType()
{
	return _objectType;
}

double min(double first, double second) {
	return first < second ? first : second;
}

double max(double first, double second) {
	return first > second ? first : second;
}
