#include "DesignArea.h"

DesignArea::DesignArea(QWidget *parent) : QOpenGLWidget(parent)
{/*
	QSurfaceFormat format;
	format.setProfile(QSurfaceFormat::CompatibilityProfile);
	format.setVersion(2, 1);
	
	context = new QOpenGLContext;
	context->setFormat(format);
	context->create();
	context->makeCurrent(this);

	openGLFunctions = context->functions();*/
	//connect(&timer, SIGNAL(timeout()), this, SLOT(updateGL()));
	//timer.start(16);
}

DesignArea::~DesignArea()
{
}
/*
void DesignArea::drawLineTo(const QPoint & endPoint)
{
	QPainter painter(&image);
	painter.setPen(QPen(Qt::blue, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter.drawLine(_lastPoint, endPoint);

	int rad = (4 / 2) + 2;
	update(QRect(_lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
	_lastPoint = endPoint;
}*/

void DesignArea::initializeGL()
{
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	resizeGL(this->width(), this->height());
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void DesignArea::resizeGL(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, w, h);
	glLoadIdentity();

	qreal aspectRatio = qreal(w) / qreal(h);
	//glOrtho(-1 * aspectRatio, 1 * aspectRatio, -1 * aspectRatio, 1 * aspectRatio, 1, -1);
	gluOrtho2D(0, w, h, 0);
	//glLoadIdentity();
}

void DesignArea::paintGL()
{
	if (constructing) {
		drawRoad();
	}
	else if (repainting) {
		repaintScene();
	}
	else if (undo) {
		undoChanges();
	}
}

void DesignArea::resizeEvent(QResizeEvent * event)
{
	resizeGL(this->width(), this->height());
}

void DesignArea::paintEvent(QPaintEvent * event)
{
	QPainter painter;
	painter.begin(this);
	paintGL();
	painter.end();
}

void DesignArea::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton) {
		firstPoint = event->pos();
		_firstPoint = Point(firstPoint.x(), firstPoint.y());
		constructing = true;
	}
	else if (event->button() == Qt::RightButton) {
		undo = true;
		repaint();
	}
}
/*
void DesignArea::mouseMoveEvent(QMouseEvent * event)
{
	if (event->buttons() & Qt::LeftButton && constructing == true) {
		_lastPoint = event->pos();
		repaint();
	}
}*/

void DesignArea::mouseReleaseEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton && constructing == true) {
		lastPoint = event->pos();
		_lastPoint = Point(lastPoint.x(), lastPoint.y());
		if(lastPoint != firstPoint) 
			repaint();
	}
	else if (event->button() == Qt::RightButton) {
		repainting = true;
		repaint();
	}
}

void DesignArea::drawRoad()
{/*
	constructing = false;
	glLineWidth(3);
	float x, y;
	vectors parallel_segments = calc_vectors(_lastPoint, _firstPoint);

	glBegin(GL_LINES);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex2f(_lastPoint.x(), _lastPoint.y());
		glVertex2f(_firstPoint.x(), _firstPoint.y());
	glEnd();

	glBegin(GL_LINES);
		glColor3f(0.0f, 1.0f, 0.0f);
		x = round(_lastPoint.x() + parallel_segments.x1);
		y = round(_lastPoint.y() + parallel_segments.y1);
		glVertex2f(x, y);
		x = round(_firstPoint.x() + parallel_segments.x1);
		y = round(_firstPoint.y() + parallel_segments.y1);
		glVertex2f(x, y);
	glEnd();

	glBegin(GL_LINES);
		glColor3f(0.0f, 1.0f, 0.0f);
		x = round(_lastPoint.x() + parallel_segments.x2);
		y = round(_lastPoint.y() + parallel_segments.y2);
		glVertex2f(x, y);
		x = round(_firstPoint.x() + parallel_segments.x2);
		y = round(_firstPoint.y() + parallel_segments.y2);
		glVertex2f(x, y);
	glEnd();
	*/
	Point point;
	int currentRoadId = allRoads.size();
	Junction *startJunction = NULL, *endJunction = NULL;
	Road* startRoad = NULL, *endRoad = NULL;
	bool startJunctionExists = false, endJunctionExists = false;
	LineParams startBermParams = LineParams{ 0.0, 0.0, false }, endBermParams = LineParams{ 0.0,  0.0, false };
	std::vector<int> connectedRoads;

	// search all roads
	for (auto road : this->allRoads)
	{
		point = road->searchPoint(_lastPoint);
		if (point.x() != 0 && endRoad == NULL) {
			endRoad = road;
			_lastPoint = point;
			//endBermParams = road->getLineParams(road->returnCloserBerm(_firstPoint));
		}
		point = road->searchPoint(_firstPoint);
		if (point.x() != 0 && startRoad == NULL) {
			startRoad = road;
			_firstPoint = point;
			//startBermParams = road->getLineParams(road->returnCloserBerm(_lastPoint));
		}
	}
	if (startRoad == endRoad && startRoad != NULL)
		return;

	switch (currentObjectBrush)
	{
	case OneWayRoadWithOneLane: {
		OneWayOneLane *road = new OneWayOneLane(currentRoadId);
		if (startRoad != NULL) {
			for (auto junction : this->allJunctions)
				if (junction->isPoint(_firstPoint)) {
					startJunction = junction;
					startJunctionExists = true;
					junction->addRoad(road);
					break;
				}
			if (!startJunctionExists) {
				startJunction = new Junction(_firstPoint, startRoad, road);
				allJunctions.push_back(startJunction);
			}
			std::vector<int> roadIds = startJunction->getRoadIds();
			connectedRoads.insert(connectedRoads.begin(), roadIds.begin(), roadIds.end());
		}
		if (endRoad != NULL) {
			for (auto junction : this->allJunctions)
				if (junction->isPoint(_lastPoint)) {
					endJunction = junction;
					endJunctionExists = true;
					junction->addRoad(road);
					break;
				}
			if (!endJunctionExists) {
				endJunction = new Junction(_lastPoint, endRoad, road);
				allJunctions.push_back(endJunction);
			}
			std::vector<int> roadIds = endJunction->getRoadIds();
			connectedRoads.insert(connectedRoads.end(), roadIds.begin(), roadIds.end());
		}
		road->setRoad(_firstPoint, _lastPoint, endRoad != NULL, startJunction, endJunction);
		if (!checkIfCollidingWithOtherRoad(road, connectedRoads)) { //road can be added
			road->drawRoad();
			if (startRoad != NULL) startRoad->addJunction(_firstPoint, LANE, startJunction);
			if (endRoad != NULL) endRoad->addJunction(_lastPoint, LANE, endJunction);
			allRoads.push_back(road);
			std::vector<AppObject*> changes;
			//add changes
			allChanges.push_back(Change{ road, changeCounter });
			if (endJunction != NULL && endJunction->numberOfRoads() == 2) allChanges.push_back(Change{ endJunction, changeCounter });
			if (startJunction != NULL && startJunction->numberOfRoads() == 2) allChanges.push_back(Change{ startJunction, changeCounter });
			changeCounter++;
		}
		else {
			if (endRoad != NULL && endJunction->numberOfRoads() == 2) // only if two roads in junction - it is brand new one -> delete whole junction
				deleteJunction(endJunction);
			else if (endRoad != NULL && endJunction->numberOfRoads() > 2) // if more roads in junction -> delete only this road from junction
				endJunction->deleteRoad(road);
			if (startRoad != NULL && startJunction->numberOfRoads() == 2)
				deleteJunction(startJunction);
			else if (startRoad != NULL && startJunction->numberOfRoads() > 2)
				startJunction->deleteRoad(road);
		}
		constructing = false;
		break;
	}
	}
	makeConnections();
}

void DesignArea::repaintScene()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (auto _road : allRoads) {
		switch (_road->getRoadType()) {
		case OneWayRoadWithOneLane: {
			OneWayOneLane *road = (OneWayOneLane*)_road;
			road->drawRoad();
		}
		}
	}
	repainting = false;
}

void DesignArea::undoChanges()
{
	bool done = false;
	auto change = allChanges.end();
	while (change > allChanges.begin()) {
		change--;
		if ((*change).changeId == changeCounter - 1) {
			if ((*change).appObject->getObjectType() == ROAD)
				deleteRoad(dynamic_cast<Road*>((*change).appObject));
			if ((*change).appObject->getObjectType() == JUNCTION)
				deleteJunction(dynamic_cast<Junction*>((*change).appObject));
			done = true;
		}
		else {
			break;
		}
		allChanges.pop_back();
		change = allChanges.end();
	}
	if (done) changeCounter--;
	undo = false;
}

void DesignArea::addChanges(std::vector<AppObject*> addedObjects)
{
	for (auto objectIt = addedObjects.begin(); objectIt <= addedObjects.end(); objectIt++)
		allChanges.push_back(Change{ (*objectIt), changeCounter });
	changeCounter++;
}

void DesignArea::makeConnections()
{
	for (auto junctionIt = allJunctions.begin(); junctionIt < allJunctions.end(); junctionIt++)
		(*junctionIt)->makeConnections();
}

Point DesignArea::searchPoint(QPoint)
{
	return Point(1, 1);
}

bool DesignArea::checkIfCollidingWithOtherRoad(Road *actual, std::vector<int> actuallyConnectedRoads)
{
	bool isAlreadyConnected;
	bool lineCollides = false;
	for (auto other : allRoads) {
		isAlreadyConnected = false;
		for (auto roadId : actuallyConnectedRoads)
			if (roadId == other->id) {
				isAlreadyConnected = true;
				break;
			}
		if (isAlreadyConnected)
			continue;
		if (other->doLineCrosses(actual->getLineParams(LEFT_BERM)) || other->doLineCrosses(actual->getLineParams(RIGHT_BERM))) {
			lineCollides = true;
			break;
		}
	}
	return lineCollides;
}

void DesignArea::deleteJunction(Junction *deletedJunction)
{
	for(auto junctionIt = allJunctions.begin(); junctionIt <= allJunctions.end(); junctionIt++)
		if ((*junctionIt) == deletedJunction) {

			allJunctions.erase(junctionIt);
			break;
		}
}

void DesignArea::deleteRoad(Road *deletedRoad)
{
	for(auto roadIt = allRoads.begin(); roadIt <= allRoads.end(); roadIt++)
		if ((*roadIt)->id == deletedRoad->id) {
			deletedRoad->deleteFromJunctions();
			allRoads.erase(roadIt);
			break;
		}
}

/*
	vectors to calc two parallel segments
	yb^2 * (ya^2/xa^2) - yb - odl^2 = 0
	delta = b^2 - 4ac  -->  float delta = sqrt( 1 + 4 * odl^2 * (ya^2/xa^2) )
*/
vectors DesignArea::calc_vectors(QPoint A, QPoint B)
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
