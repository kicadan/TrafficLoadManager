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

void DesignArea::handleAction() {
	auto action = qobject_cast<QAction*>(QObject::sender())->objectName();

	if (action == "actionCofnij") {
		dispatchAction(UNDO);
	}
	else if (action == "actionDroga_jednokierunkowa_z_jednym_pasem") {
		dispatchAction(DRAW_ONE_WAY_ONE_LANE);
	}
	else if (action == "actionDroga_jednokierunkowa_z_dwoma_pasami_ruchu") {
		dispatchAction(DRAW_ONE_WAY_TWO_LANES);
	}
	else if (action == "actionPo_czenia_na_skrzy_owaniu") {
		dispatchAction(MAKE_CONNECTION);
	}
}

void DesignArea::dispatchAction(Menu menuAction)
{
	switch (menuAction) {
	case UNDO: {
		undoChanges();
		repainting = true;
		repaint();
		break;
	}
	case DRAW_ONE_WAY_ONE_LANE: {
		currentObjectBrush = OneWayRoadWithOneLane;
		break;
	}
	case DRAW_ONE_WAY_TWO_LANES: {
		currentObjectBrush = OneWayRoadWithTwoLanes;
		break;
	}
	case DRAW_TWO_WAY_ONE_LANE: {
		currentObjectBrush = TwoWayRoadWithOneLane;
		break;
	}
	case DRAW_TWO_WAY_TWO_LANES: {
		currentObjectBrush = TwoWayRoadWithTwoLanes;
		break;
	}
	case MAKE_CONNECTION: {
		currentObjectBrush = JunctionConnection;
		break;
	}
	}
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
		drawElement();
	}
	else if (repainting) {
		repaintScene();
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
		_firstPoint = event->pos();
		firstPoint = Point(_firstPoint.x(), _firstPoint.y());
		constructing = true;
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
		_lastPoint = event->pos();
		lastPoint = Point(_lastPoint.x(), _lastPoint.y());
		if(!(lastPoint == firstPoint)) 
			repaint();
	}
	else if (event->button() == Qt::RightButton) {
		repainting = true;
		repaint();
	}
}

void DesignArea::drawElement()
{
	switch (currentObjectBrush) {
	case OneWayRoadWithOneLane: case OneWayRoadWithTwoLanes: case TwoWayRoadWithOneLane: case TwoWayRoadWithTwoLanes: {
		drawRoad(); 
		break;
	}
	case JunctionConnection: {
		makeConnection();
		break;
	}
	}
}

void DesignArea::makeConnection()
{
	Point startPoint, endPoint;
	Junction *startPointJunction = NULL, *endPointJunction = NULL;
	LaneType startLaneType = NOTHING, endLaneType = NOTHING;
	Road* startRoad = NULL, *endRoad = NULL;
	for (auto road : this->allRoads)
	{
		if (endLaneType == NOTHING) 
			endLaneType = road->searchPointForLaneType(lastPoint);
		if (endLaneType != NOTHING && endRoad == NULL) {
			endRoad = road;
			endPointJunction = (Junction*)road->searchForClosestJunction(lastPoint, endLaneType);
		}
		if(startLaneType == NOTHING) 
			startLaneType = road->searchPointForLaneType(firstPoint);
		if (startLaneType != NOTHING && startRoad == NULL) {
			startRoad = road;
			startPointJunction = (Junction*)road->searchForClosestJunction(firstPoint, startLaneType);
		}
		if (startPointJunction != NULL && startPointJunction == endPointJunction) break;
	}
	if (startPointJunction != NULL && startPointJunction == endPointJunction && endRoad != NULL && startRoad != NULL && endLaneType != NOTHING && startLaneType != NOTHING) {
		startPointJunction->connectRoads(startRoad, startLaneType, endRoad, endLaneType);
	}
	constructing = false;
}

void DesignArea::drawRoad()
{
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
		point = road->searchPoint(lastPoint);
		if (point.x() != 0 && endRoad == NULL) {
			endRoad = road;
			_lastPoint = QPointF(point.x(), point.y());
			lastPoint = point;
		}
		point = road->searchPoint(firstPoint);
		if (point.x() != 0 && startRoad == NULL) {
			startRoad = road;
			_firstPoint = QPointF(point.x(), point.y());
			firstPoint = point;
		}
	}
	if (startRoad == endRoad && startRoad != NULL)
		return;

	if (startRoad != NULL) {
		for (auto junction : this->allJunctions)
			if (junction->isPoint(firstPoint)) {
				startJunction = junction;
				startJunctionExists = true;
				break;
			}
		if (!startJunctionExists) {
			startJunction = new Junction(firstPoint, startRoad, allJunctions.size());
			allJunctions.push_back(startJunction);
		}
		std::vector<int> roadIds = startJunction->getRoadIds();
		connectedRoads.insert(connectedRoads.begin(), roadIds.begin(), roadIds.end());
	}
	if (endRoad != NULL) {
		for (auto junction : this->allJunctions)
			if (junction->isPoint(lastPoint)) {
				endJunction = junction;
				endJunctionExists = true;
				break;
			}
		if (!endJunctionExists) {
			endJunction = new Junction(lastPoint, endRoad, allJunctions.size());
			allJunctions.push_back(endJunction);
		}
		std::vector<int> roadIds = endJunction->getRoadIds();
		connectedRoads.insert(connectedRoads.end(), roadIds.begin(), roadIds.end());
	}

	switch (currentObjectBrush)
	{
	case OneWayRoadWithOneLane: {
		OneWayOneLane *road = new OneWayOneLane(currentRoadId);
		road->setRoad(_firstPoint, _lastPoint, endRoad != NULL, startJunction, endJunction);
		if (!checkIfCollidingWithOtherRoad(road, connectedRoads)) { //road can be added
			road->drawRoad();
			if (startRoad != NULL) {
				startRoad->addJunction(firstPoint, startJunction); 
				startJunction->addRoad(road);
			}
			if (endRoad != NULL) {
				endRoad->addJunction(lastPoint, endJunction); 
				endJunction->addRoad(road);
			}
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
		break;
	}
	case OneWayRoadWithTwoLanes: {
		OneWayTwoLanes *road = new OneWayTwoLanes(currentRoadId);
		road->setRoad(_firstPoint, _lastPoint, endRoad != NULL, startJunction, endJunction);
		if (!checkIfCollidingWithOtherRoad(road, connectedRoads)) { //road can be added
			road->drawRoad();
			if (startRoad != NULL) {
				startRoad->addJunction(firstPoint, startJunction);
				startJunction->addRoad(road);
			}
			if (endRoad != NULL) {
				endRoad->addJunction(lastPoint, endJunction);
				endJunction->addRoad(road);
			}
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
		break;
	}
	}
	constructing = false;
	if (allJunctions.size() == 5) allWays.push_back(findWay(*allJunctions[0], *allJunctions[4]));
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
		case OneWayRoadWithTwoLanes: {
			OneWayTwoLanes *road = (OneWayTwoLanes*)_road;
			road->drawRoad();
		}
		}
	}
	for (auto _junction : allJunctions) {
		_junction->drawConnections();
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
}

void DesignArea::addChanges(std::vector<AppObject*> addedObjects)
{
	for (auto objectIt = addedObjects.begin(); objectIt <= addedObjects.end(); objectIt++)
		allChanges.push_back(Change{ (*objectIt), changeCounter });
	changeCounter++;
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
			(*junctionIt)->forgetAboutMe();
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

void DesignArea::copyAllJunctions(std::vector<Junction> &vectorTo)
{
	for (auto junctionIt = allJunctions.begin(); junctionIt < allJunctions.end(); junctionIt++)
		vectorTo.push_back(*(*junctionIt));
}

void DesignArea::fulfillNodeTable(std::vector<Node> &wayTable, Junction startJunction)
{
	for (auto junctionIt = allJunctions.begin(); junctionIt < allJunctions.end(); junctionIt++)
		if ((*junctionIt)->getId() == startJunction.getId()) {
			wayTable.push_back(Node{ startJunction, 0 });
		}
		else
			wayTable.push_back(Node{ *(*junctionIt), 999999, Junction(-1) });
}

void DesignArea::transferJunction(std::vector<Junction> &from, std::vector<Junction> &to, Junction junction)
{
	for (auto junctionIt = from.begin(); junctionIt < from.end(); junctionIt++) {
		if ( (*junctionIt).getId() == junction.getId()) {
			to.push_back(*junctionIt);
			from.erase(junctionIt);
			break;
		}
	}
}

void DesignArea::updateConnectionIfCloser(Junction startJunction, std::vector<Node> &allNodes, Connection connection)
{
	int previousCost;
	for (auto nodesIt = allNodes.begin(); nodesIt < allNodes.end(); nodesIt++) {
		if ((*nodesIt).junction.getId() == startJunction.getId()) {
			previousCost = (*nodesIt).cost;
			break;
		}
	}
	//update connection if closer, or add to existing if same length and same previous road id
	for (auto nodesIt = allNodes.begin(); nodesIt < allNodes.end(); nodesIt++) {
		if ((*nodesIt).junction.getId() == connection.nextJunction->getId()) {
			if ((*nodesIt).cost > previousCost + connection.distanceToNextJunction) {
				(*nodesIt).connections.clear();
				(*nodesIt).connections.push_back(connection);
				(*nodesIt).previousJunction = startJunction;
				(*nodesIt).cost = previousCost + connection.distanceToNextJunction;
			}
			else if ((*nodesIt).cost == previousCost + connection.distanceToNextJunction && (*nodesIt).connections[0].previousRoad == connection.previousRoad) {
				(*nodesIt).connections.push_back(connection);
			}
			break;
		}
	}
}

Junction DesignArea::getClosestJunction(std::vector<Node> allNodes, std::vector<Junction> Qset)
{
	bool found = false;
	Junction foundJunction;
	std::sort(allNodes.begin(), allNodes.end(), [](const Node first, const Node second) { return first.cost < second.cost; });
	auto nodesIt = allNodes.begin();
	while (!found && nodesIt < allNodes.end()) {
		for (auto qsetIt = Qset.begin(); qsetIt < Qset.end(); qsetIt++) {
			if ((*nodesIt).junction.getId() == (*qsetIt).getId()) {
				found = true;
				foundJunction = *qsetIt;
				break;
			}
		}
		nodesIt++;
	}
	return foundJunction;
}

/*
	vectors to calc two parallel segments
	yb^2 * (ya^2/xa^2) - yb - odl^2 = 0
	delta = b^2 - 4ac  -->  float delta = sqrt( 1 + 4 * odl^2 * (ya^2/xa^2) )
*/
vectors DesignArea::calc_vectors(QPoint A, QPoint B)
{
	double xa = A.x() - B.x();
	double ya = A.y() - B.y();
	double radius = sqrt(xa*xa + ya * ya);
	return vectors{ ya*distance / radius, -xa * distance / radius, -ya * distance / radius, xa * distance / radius };
}

Way DesignArea::makeWayFromNodes(std::vector<Node> allNodes, Junction startJunction, Junction endJunction)
{
	int connectionNumber;
	int length = 0;
	int infinityLoopBreaker = 0;
	bool success = true;
	Way theWay;
	//could be needed if car couldnt change lane during moving straight the road; it makes car moving straight without changing lanes
	LaneType expectedLaneType;
	std::vector<Connection> connections;
	Junction previousJunction = endJunction;
	while (previousJunction.getId() != startJunction.getId() && success) {
		infinityLoopBreaker++;
		if (infinityLoopBreaker == allJunctions.size() + 1) { //one more iteration than all nodes
			success = false;
			break;
		}
		connectionNumber = -1;
		for (auto nodeIt = allNodes.begin(); nodeIt < allNodes.end(); nodeIt++) {
			if ((*nodeIt).junction.getId() == previousJunction.getId()) {
				connections = (*nodeIt).connections;
				previousJunction = (*nodeIt).previousJunction;
				break;
			}
			else if (nodeIt == allNodes.end() - 1) { //all nodes checked and nothing found
				success = false;
				break;
			}
		}
		if (!success || previousJunction.getId() == -1) { //if iteration before performed without success OR previousJunction doesn't exist
			success = false;
			break;
		}
		if(connections.size() > 1 && connections[0].nextJunction->getId() == endJunction.getId())
			//take most right lane to allow to get out of car
			for (auto connectionIt = connections.begin(); connectionIt < connections.end(); connectionIt++) {
				if ((*connectionIt).nextLaneType == RIGHT_LANE || (*connectionIt).nextLaneType == RIGHT_BACK_LANE)
					connectionNumber = connectionIt - connections.begin();
			}
		//if connection not matched and exists any connection
		if (connectionNumber == -1 && connections.size() > 0) {
			connectionNumber = qrand() % connections.size();
		}
		if (connectionNumber != -1) {
			theWay.steps.push_back(connections[connectionNumber]);
			length += connections[connectionNumber].distanceToNextJunction;
		}
		//car can change the lane so expectedLaneType is not needed during next iterations
		expectedLaneType = connections[connectionNumber].previousLaneType;
	}
	if (success)
		theWay.length = length;
	else
		theWay.length = -1; //no connection
	theWay.from = startJunction;
	theWay.to = endJunction;
	return theWay;
}

Way DesignArea::findWay(Junction startJunction, Junction endJunction)
{
	std::vector<Node> allNodes;
	fulfillNodeTable(allNodes, startJunction);
	std::vector<Junction> Qset, Sset;
	copyAllJunctions(Qset);
	std::vector<Connection> connections;
	Junction actualJunction;
	while (Qset.size() != 0) {
		actualJunction = getClosestJunction(allNodes, Qset);
		connections = actualJunction.getConnectionsFrom(-1); //all connections from this junction
		for (auto connectionIt = connections.begin(); connectionIt < connections.end(); connectionIt++) {
			updateConnectionIfCloser(actualJunction, allNodes, *connectionIt);
		}
		transferJunction(Qset, Sset, actualJunction);
	}
	//now take way from table
	return makeWayFromNodes(allNodes, startJunction, endJunction);
}