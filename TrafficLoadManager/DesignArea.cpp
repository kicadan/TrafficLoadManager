#include "DesignArea.h"
#include "SpawnSettingsEditor.h"

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
	else if (action == "actionDroga_dwukierunkowa_z_jednym_pasem_ruchu") {
		dispatchAction(DRAW_TWO_WAY_ONE_LANE);
	}
	else if (action == "actionPunkt_odradzania_pojazd_w") {
		dispatchAction(DRAW_SPAWN_POINT);
	}
	else if (action == "actionPunkt_odradzania_pojazd_w_2") {
		dispatchAction(EDIT_SPAWN_POINT);
	}
	else if (action == "actionPo_czenia_na_skrzy_owaniu") {
		dispatchAction(MAKE_CONNECTION);
	}
	else if (action == "action_sygnalizacja") {
		dispatchAction(SET_TRAFFIC_LIGHTS);
	}
	else if (action == "action_sygnalizacja_edytuj") {
		dispatchAction(EDIT_TRAFFIC_LIGHTS);
	}
}

void DesignArea::dispatchAction(Action menuAction)
{
	editing = false;
	constructing = false;
	repainting = false;
	if (!simulationInProgress) {
		switch (menuAction) {
		case UNDO: {
			undoChanges();
			validateConnections();
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
		case DRAW_SPAWN_POINT: {
			currentObjectBrush = CarSpawn;
			break;
		}
		case EDIT_SPAWN_POINT: {
			currentObjectBrush = CarSpawn;
			editing = true;
			break;
		}
		case MAKE_CONNECTION: {
			currentObjectBrush = JunctionConnection;
			break;
		}
		case SET_TRAFFIC_LIGHTS: {
			currentObjectBrush = TrafficLights;
			break;
		}
		case EDIT_TRAFFIC_LIGHTS: {
			currentObjectBrush = TrafficLights;
			editing = true;
			break;
		}
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
	else if (simulationInProgress) {
		processSimulation();
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
	case OneWayRoadWithOneLane: case OneWayRoadWithTwoLanes: case TwoWayRoadWithOneLane: {
		drawRoad(); 
		break;
	}
	case CarSpawn: {
		if (!editing)
			addCarSpawn();
		else
			editCarSpawn();
		break;
	}
	case JunctionConnection: {
		makeConnection();
		break;
	}
	case TrafficLights: {
		if (!editing)
			setTrafficLights();
		else
			editTrafficLights();
		break;
	}
	}
	repaintScene();
	updateWays();
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
		startPointJunction->updateLightsSettings();
		startPointJunction->drawJunction();
	}
	constructing = false;
}

void DesignArea::updateWays()
{
	allWays.clear();
	Way theWay;
	std::vector<Junction*> destinations;
	for (auto junctionIt = allJunctions.begin(); junctionIt < allJunctions.end(); junctionIt++) {
		destinations = (*junctionIt)->getCarSpawnSettings().destinations;
		for (auto destinationIt = destinations.begin(); destinationIt < destinations.end(); destinationIt++) {
			findWay(**junctionIt, **destinationIt);
		}
		(*junctionIt)->clearWays();
		for (auto wayIt = allWays.begin(); wayIt < allWays.end(); wayIt++) {
			if ((*wayIt).from.getId() == (*junctionIt)->getId())
				(*junctionIt)->addWay(*wayIt);
		}
	}
}

//deprecated
void DesignArea::renewConnectionsForSpawnPoints()
{
	for (auto junctionIt = allJunctions.begin(); junctionIt < allJunctions.end(); junctionIt++) {
		if ((*junctionIt)->isCarSpawn() && (*junctionIt)->getNumberOfRoads() == 1 && (*junctionIt)->getConnectionsFrom(-1).size() == 0) {
			(*junctionIt)->makeConnectionsForCarSpawn();
		}
	}
}

void DesignArea::setTrafficLights()
{
	constructing = false; 
	Road* startRoad = NULL;
	Junction* junction = NULL;
	Point point(0, 0);
	// search all roads
	for (auto road : this->allRoads)
	{
		point = road->searchPoint(firstPoint);
		if (point.x() != 0 && startRoad == NULL) {
			startRoad = road;
			_firstPoint = QPointF(point.x(), point.y());
			firstPoint = point;
			break;
		}
	}
	for (auto junctionIt = allJunctions.begin(); junctionIt < allJunctions.end(); junctionIt++) {
		if ((*junctionIt)->isPoint(point)) {
			if (!(*junctionIt)->hasTrafficLights()) {
				junction = *junctionIt;
				junction->setTrafficLights();
				junction->drawJunction();
				allChanges.push_back(Change{ junction, changeCounter, SET_TRAFFIC_LIGHTS });
				changeCounter++;
			}
			break;
		}
	}
	
}

void DesignArea::editTrafficLights()
{
	constructing = false;
	Road* startRoad = NULL;
	Junction* junction = NULL;
	Point point(0, 0);
	//search all roads to find point
	for (auto road : this->allRoads)
	{
		point = road->searchPoint(firstPoint);
		if (point.x() != 0 && startRoad == NULL) {
			startRoad = road;
			_firstPoint = QPointF(point.x(), point.y());
			firstPoint = point;
			break;
		}
	}
	if (startRoad != NULL) {
		junction = (Junction*)startRoad->searchForClosestJunction(point, startRoad->getRoadType() == OneWayRoadWithOneLane || startRoad->getRoadType() == TwoWayRoadWithOneLane ? LANE : LEFT_LANE);
		//open dialog
		if (junction != NULL && junction->hasTrafficLights()) {
			TrafficLightsEditor trafficLightsEditor(this, junction);
			trafficLightsEditor.setLightsEditorFields();
			trafficLightsEditor.setModal(true);
			trafficLightsEditor.exec();
		}
	}
}

void DesignArea::addCarSpawn()
{
	Road* startRoad = NULL;
	Junction* junction = NULL;
	Point point(0, 0);
	// search all roads
	for (auto road : this->allRoads)
	{
		point = road->searchPoint(firstPoint);
		if (point.x() != 0 && startRoad == NULL) {
			startRoad = road;
			_firstPoint = QPointF(point.x(), point.y());
			firstPoint = point;
			break;
		}
	}
	for (auto junctionIt = allJunctions.begin(); junctionIt < allJunctions.end(); junctionIt++) {
		if ((*junctionIt)->isPoint(point)) {
			junction = *junctionIt;
			if (!(*junctionIt)->isCarSpawn()) {
				junction->setAsCarSpawn();
				junction->makeConnectionsForCarSpawn();
				allChanges.push_back(Change{ junction, changeCounter, DRAW_SPAWN_POINT });
				changeCounter++;
				junction->drawJunction();
			}
			break;
		}
	}
	if (junction == NULL && startRoad != NULL) {
		junction = new Junction(firstPoint, startRoad, allJunctions.size());
		startRoad->addJunction(firstPoint, junction);
		junction->updateOtherJunctionsOnMainRoad();
		junction->setAsCarSpawn();
		junction->makeConnectionsForCarSpawn();
		allJunctions.push_back(junction);
		allChanges.push_back(Change{ junction, changeCounter, DRAW_SPAWN_POINT });
		changeCounter++;
		junction->drawJunction();
	}
	constructing = false;
}

void DesignArea::editCarSpawn()
{
	constructing = false;
	Road* startRoad = NULL;
	Junction* junction = NULL;
	Point point(0, 0);
	//search all roads to find point
	for (auto road : this->allRoads)
	{
		point = road->searchPoint(firstPoint);
		if (point.x() != 0 && startRoad == NULL) {
			startRoad = road;
			_firstPoint = QPointF(point.x(), point.y());
			firstPoint = point;
			break;
		}
	}
	if (startRoad != NULL) {
		junction = (Junction*)startRoad->searchForClosestJunction(point, startRoad->getRoadType() == OneWayRoadWithOneLane || startRoad->getRoadType() == TwoWayRoadWithOneLane ? LANE : LEFT_LANE);
		//open dialog
		if (junction != NULL && junction->isCarSpawn()) {
			SpawnSettingsEditor spawnEditor(this, junction);
			spawnEditor.setSpawnEditorFields();
			spawnEditor.setJunctionComboBox(allJunctions);
			spawnEditor.setModal(true);
			spawnEditor.exec();
		}
	}
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
				startJunction->updateLightsSettings();
				startJunction->updateOtherJunctionsOnMainRoad();
				startJunction->drawJunction();
			}
			if (endRoad != NULL) {
				endRoad->addJunction(lastPoint, endJunction); 
				endJunction->addRoad(road);
				endJunction->updateLightsSettings();
				endJunction->updateOtherJunctionsOnMainRoad();
				endJunction->drawJunction();
			}
			allRoads.push_back(road);
			std::vector<AppObject*> changes;
			//add changes
			allChanges.push_back(Change{ road, changeCounter, DRAW_ONE_WAY_ONE_LANE });
			if (endJunction != NULL && endJunction->getNumberOfRoads() == 2) allChanges.push_back(Change{ endJunction, changeCounter, DRAW_ONE_WAY_ONE_LANE });
			if (startJunction != NULL && startJunction->getNumberOfRoads() == 2) allChanges.push_back(Change{ startJunction, changeCounter, DRAW_ONE_WAY_ONE_LANE });
			changeCounter++;
		}
		else {
			if (endRoad != NULL && endJunction->getNumberOfRoads() == 1) // only if one road in junction - it is brand new one -> delete whole junction
				deleteJunction(endJunction);
			else if (endRoad != NULL && endJunction->getNumberOfRoads() > 1) // if more roads in junction -> delete only this road from junction
				endJunction->deleteRoad(road);
			if (startRoad != NULL && startJunction->getNumberOfRoads() == 1)
				deleteJunction(startJunction);
			else if (startRoad != NULL && startJunction->getNumberOfRoads() > 1)
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
				startJunction->updateLightsSettings();
				startJunction->updateOtherJunctionsOnMainRoad();
				startJunction->drawJunction();
			}
			if (endRoad != NULL) {
				endRoad->addJunction(lastPoint, endJunction);
				endJunction->addRoad(road);
				endJunction->updateLightsSettings();
				endJunction->updateOtherJunctionsOnMainRoad();
				endJunction->drawJunction();
			}
			allRoads.push_back(road);
			std::vector<AppObject*> changes;
			//add changes
			allChanges.push_back(Change{ road, changeCounter, DRAW_ONE_WAY_TWO_LANES });
			if (endJunction != NULL && endJunction->getNumberOfRoads() == 2) allChanges.push_back(Change{ endJunction, changeCounter, DRAW_ONE_WAY_TWO_LANES });
			if (startJunction != NULL && startJunction->getNumberOfRoads() == 2) allChanges.push_back(Change{ startJunction, changeCounter, DRAW_ONE_WAY_TWO_LANES });
			changeCounter++;
		}
		else {
			if (endRoad != NULL && endJunction->getNumberOfRoads() == 1) // only if two road in junction - it is brand new one -> delete whole junction
				deleteJunction(endJunction);
			else if (endRoad != NULL && endJunction->getNumberOfRoads() > 1) // if more roads in junction -> delete only this road from junction
				endJunction->deleteRoad(road);
			if (startRoad != NULL && startJunction->getNumberOfRoads() == 1)
				deleteJunction(startJunction);
			else if (startRoad != NULL && startJunction->getNumberOfRoads() > 1)
				startJunction->deleteRoad(road);
		}
		break;
	}
	case TwoWayRoadWithOneLane: {
		TwoWayOneLane *road = new TwoWayOneLane(currentRoadId);
		road->setRoad(_firstPoint, _lastPoint, endRoad != NULL, startJunction, endJunction);
		if (!checkIfCollidingWithOtherRoad(road, connectedRoads)) { //road can be added
			road->drawRoad();
			if (startRoad != NULL) {
				startRoad->addJunction(firstPoint, startJunction);
				startJunction->addRoad(road);
				startJunction->updateLightsSettings();
				startJunction->updateOtherJunctionsOnMainRoad();
				startJunction->drawJunction();
			}
			if (endRoad != NULL) {
				endRoad->addJunction(lastPoint, endJunction);
				endJunction->addRoad(road);
				endJunction->updateLightsSettings();
				endJunction->updateOtherJunctionsOnMainRoad();
				endJunction->drawJunction();
			}
			allRoads.push_back(road);
			std::vector<AppObject*> changes;
			//add changes
			allChanges.push_back(Change{ road, changeCounter, DRAW_TWO_WAY_ONE_LANE });
			if (endJunction != NULL && endJunction->getNumberOfRoads() == 2) allChanges.push_back(Change{ endJunction, changeCounter, DRAW_TWO_WAY_ONE_LANE });
			if (startJunction != NULL && startJunction->getNumberOfRoads() == 2) allChanges.push_back(Change{ startJunction, changeCounter, DRAW_TWO_WAY_ONE_LANE });
			changeCounter++;
		}
		else {
			if (endRoad != NULL && endJunction->getNumberOfRoads() == 1) // only if one road in junction - it is brand new one -> delete whole junction
				deleteJunction(endJunction);
			else if (endRoad != NULL && endJunction->getNumberOfRoads() > 1) // if more roads in junction -> delete only this road from junction
				endJunction->deleteRoad(road);
			if (startRoad != NULL && startJunction->getNumberOfRoads() == 1)
				deleteJunction(startJunction);
			else if (startRoad != NULL && startJunction->getNumberOfRoads() > 1)
				startJunction->deleteRoad(road);
		}
		break;
	}
	}
	constructing = false;
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
			break;
		}
		case OneWayRoadWithTwoLanes: {
			OneWayTwoLanes *road = (OneWayTwoLanes*)_road;
			road->drawRoad();
			break;
		}
		case TwoWayRoadWithOneLane: {
			TwoWayOneLane *road = (TwoWayOneLane*)_road;
			road->drawRoad();
			break;
		}
		}
	}
	for (auto _junction : allJunctions) {
		_junction->drawJunction();
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
			if ((*change).appObject->getObjectType() == JUNCTION) {
				Junction* junction = dynamic_cast<Junction*>((*change).appObject);
				if ((*change).action != DRAW_SPAWN_POINT && (*change).action != SET_TRAFFIC_LIGHTS)
					deleteJunction(junction);
				else if ((*change).action == DRAW_SPAWN_POINT){
					if (junction->getNumberOfRoads() == 1)
						deleteJunction(junction);
					else
						junction->notCarSpawn();
				}
				else {
					junction->notTrafficLights();
				}

			}
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

void DesignArea::validateConnections()
{
	for (auto junctionIt = allJunctions.begin(); junctionIt < allJunctions.end(); junctionIt++) {
		(*junctionIt)->validateConnections();
		(*junctionIt)->updateLightsSettings();
	}
}

//deprecated
void DesignArea::addChanges(std::vector<AppObject*> addedObjects)
{
	for (auto objectIt = addedObjects.begin(); objectIt <= addedObjects.end(); objectIt++)
		allChanges.push_back(Change{ (*objectIt), changeCounter});
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
	auto junctionIt = allJunctions.begin();
	while( junctionIt < allJunctions.end()) {
		if ((*junctionIt)->getId() == deletedJunction->getId()) {
			(*junctionIt)->forgetAboutMe();
			(*junctionIt)->updateOtherJunctionsOnMainRoad();
			allJunctions.erase(junctionIt);
			junctionIt = allJunctions.begin();
			continue;
		}
		(*junctionIt)->forgetJunction(deletedJunction);
		junctionIt++;
	}
}

void DesignArea::deleteRoad(Road *deletedRoad)
{
	for(auto roadIt = allRoads.begin(); roadIt < allRoads.end(); roadIt++)
		if ((*roadIt)->id == deletedRoad->id) {
			deletedRoad->deleteFromJunctions();
			allRoads.erase(roadIt);
			break;
		}
}

void DesignArea::deleteVehicle(Vehicle *deleted)
{
	for (auto vehicleIt = allVehicles.begin(); vehicleIt < allVehicles.end(); vehicleIt++) {
		if ((*vehicleIt) == deleted) {
			allVehicles.erase(vehicleIt);
			break;
		}
	}
}

void DesignArea::copyAllJunctions(std::vector<Junction> &vectorTo)
{
	for (auto junctionIt = allJunctions.begin(); junctionIt < allJunctions.end(); junctionIt++)
		vectorTo.push_back(*(*junctionIt));
}

void DesignArea::fulfillNodeTable(std::vector<Node> &wayTable, Junction startJunction)
{
	/*for (auto junctionIt = allJunctions.begin(); junctionIt < allJunctions.end(); junctionIt++)
		if ((*junctionIt)->getId() == startJunction.getId()) {
			wayTable.push_back(Node{ startJunction, 0 });
		}
		else
			wayTable.push_back(Node{ *(*junctionIt), 999999, Junction(-1) });*/
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

void deleteNode(std::vector<Node> &deleteFrom, Junction deleted) {
	for (auto nodeIt = deleteFrom.begin(); nodeIt < deleteFrom.end(); nodeIt++) {
		if ((*nodeIt).junction.getId() == deleted.getId()) {
			deleteFrom.erase(nodeIt);
			break;
		}
	}
}

void DesignArea::resetNodeTable(Junction startJunction)
{
	allNodes.clear();
	std::vector<Connection> initialConnections;
	for (auto junctionIt = allJunctions.begin(); junctionIt < allJunctions.end(); junctionIt++) {
		if ((*junctionIt)->getId() == startJunction.getId()) {
			Node addedNode;
			addedNode.junction = startJunction;
			initialConnections = startJunction.getConnectionsFrom(-1);
			for (auto connIt = initialConnections.begin(); connIt < initialConnections.end(); connIt++) {
				Previous initialPreviousConnection;
				Connection connection;
				//it's only needed { previous.connection.nextRoad and previous.cost } for all start node's previousConnections (node's previous.connection.nextRoad = junction's connection.previousRoad)
				connection.nextRoad = (*connIt).previousRoad;
				initialPreviousConnection.connection = connection;
				initialPreviousConnection.cost = 0;
				addedNode.previousConnections.push_back(initialPreviousConnection);
			}
			allNodes.push_back(addedNode);
		}
		else {
			allNodes.push_back(Node{ *(*junctionIt) });
		}
	}
}

void DesignArea::updateNode(Junction actual, Junction next/*updated node*/, Connection connection) {
	int cost = 0;
	//take cost from Previous vector, where previous (nextRoad) == actual_connection (previousRoad)
	for (auto nodeIt = allNodes.begin(); nodeIt < allNodes.end(); nodeIt++) {
		if ((*nodeIt).junction.getId() == actual.getId()) {
			for (auto previousIt = (*nodeIt).previousConnections.begin(); previousIt < (*nodeIt).previousConnections.end(); previousIt++) {
				if ((*previousIt).connection.nextRoad->id == connection.previousRoad->id) {
					cost = (*previousIt).cost;
				}
			}
			break;
		}
	}
	Previous previous;
	previous.connection = connection;
	previous.junction = actual;
	previous.cost = connection.distanceToNextJunction + cost;
	for (auto nodeIt = allNodes.begin(); nodeIt < allNodes.end(); nodeIt++) {
		if ((*nodeIt).junction.getId() == next.getId()) {
			(*nodeIt).previousConnections.push_back(previous);
			break;
		}
	}
}

void DesignArea::findWay(Junction startJunction, Junction endJunction) {
	Way theWay;
	resetNodeTable(startJunction);
	std::vector<Junction> passedNodes;
	std::vector<Connection> connections;
	connections = startJunction.getConnectionsFrom(-1);
	//deleteNode(allNodes, startJunction);
	passedNodes.push_back(startJunction);
	for (auto connIt = connections.begin(); connIt < connections.end(); connIt++) {
		updateNode(startJunction, *(*connIt).nextJunction, *connIt);
		recursiveNodeFollow(passedNodes, *connIt, endJunction.getId());
	}
	collectWay(startJunction, endJunction);
}

void DesignArea::processVehiclesMove()
{
	std::vector<Vehicle*> alreadyProcessedVehicles;
	for (auto roadIt = allRoads.begin(); roadIt < allRoads.end(); roadIt++) {
		(*roadIt)->processAllVehicles(alreadyProcessedVehicles);
	}
}

void DesignArea::emitCar(Way theWay) //called only when Vehicle can be emitted cause there is a space for it
{
	Automobile *newAutomobile;
	Connection firstStep;
	if (theWay.length > 0) {
		firstStep = theWay.steps[theWay.steps.size() - 1];
		newAutomobile = new Automobile(theWay);
		firstStep.nextRoad->reservePoint(firstStep.nextLaneType, newAutomobile, firstStep.nextPoint);
	}
}

void DesignArea::processSimulation()
{
	repaintScene();
	processVehiclesMove();
	for (auto junctionIt = allJunctions.begin(); junctionIt < allJunctions.end(); junctionIt++) {
		(*junctionIt)->updateLights();
		emitCar((*junctionIt)->updateCarSpawn());
	}
}

void DesignArea::recursiveNodeFollow(std::vector<Junction> passedNodes, Connection previousConnection, int endJunctionId) {
	std::vector<Connection> connections;
	Junction actual = *previousConnection.nextJunction;
	//dont go further if u were in this node in this recursion before (it shouldn't be closer cause recursion still adds more cost in deeper steps)
	for (auto junctionIt = passedNodes.begin(); junctionIt < passedNodes.end(); junctionIt++)
		if ((*junctionIt).getId() == actual.getId())
			return;
	//if it's finish - end recursion
	if (actual.getId() == endJunctionId)
		return;
	passedNodes.push_back(actual);
	connections = actual.getConnectionsFrom(-1); //next->getConnectionsFrom(-1)
	//deleteNode(passedNodes, actual);
	for (auto connIt = connections.begin(); connIt < connections.end(); connIt++) {
		if ((*connIt).previousRoad->id == previousConnection.nextRoad->id && (*connIt).previousLaneType == previousConnection.nextLaneType) {
			updateNode(actual, *(*connIt).nextJunction, *connIt);
			recursiveNodeFollow(passedNodes, *connIt, endJunctionId);
		}
	}
}

//collect way steps from all nodes, following from back to front (last node -> first node); do it recursive, it will let to be more than 1 collected way
void DesignArea::collectWay(Junction startJunction, Junction endJunction)
{
	int deepWatchDog = 0;
	int length;
	Node actualNode;
	for (auto nodeIt = allNodes.begin(); nodeIt < allNodes.end(); nodeIt++) {
		if ((*nodeIt).junction.getId() == endJunction.getId()) {
			actualNode = *nodeIt;
			break;
		}
	}
	for (auto connectionsIt = actualNode.previousConnections.begin(); connectionsIt < actualNode.previousConnections.end(); connectionsIt++) {
		Way way;
		way.from = startJunction;
		way.to = endJunction;
		way.length += (*connectionsIt).connection.distanceToNextJunction;
		way.steps.push_back((*connectionsIt).connection);
		collectWayRecursive((*connectionsIt).junction, (*connectionsIt).connection, startJunction.getId(), deepWatchDog, way);
		allWays.push_back(way);
	}
}

void DesignArea::collectWayRecursive(Junction actualJunction, Connection actualConnection, int firstJunctionId, int deepWatchDog, Way &way)
{
	if (deepWatchDog > allNodes.size())
		return;
	deepWatchDog++;
	Node actualNode;
	for (auto nodeIt = allNodes.begin(); nodeIt < allNodes.end(); nodeIt++) {
		if ((*nodeIt).junction.getId() == actualJunction.getId()) {
			actualNode = (*nodeIt);
			break;
		}
	}
	if (actualNode.junction.getId() == firstJunctionId)
		return;
	for (auto nodeConnectionIt = actualNode.previousConnections.begin(); nodeConnectionIt < actualNode.previousConnections.end(); nodeConnectionIt++) {
		if ((*nodeConnectionIt).connection.nextRoad->id == actualConnection.previousRoad->id && (*nodeConnectionIt).connection.nextLaneType == actualConnection.previousLaneType) { //teraz wszystkie mo¿liwe drogi (szykuje siê implementacja zmiany pasa)
			way.length += (*nodeConnectionIt).connection.distanceToNextJunction;
			way.steps.push_back((*nodeConnectionIt).connection);
			collectWayRecursive((*nodeConnectionIt).junction, (*nodeConnectionIt).connection, firstJunctionId, deepWatchDog, way);
		}
	}
}

void DesignArea::continueSimulation()
{
	repaint();
}

void DesignArea::startSimulation()
{
	if (!simulationInProgress) {
		simulationInProgress = true; //block other actions
		timer = new QTimer(this);
		connect(timer, SIGNAL(timeout()), this, SLOT(continueSimulation()));
		timer->start(1000);
		for (auto junctionIt = allJunctions.begin(); junctionIt < allJunctions.end(); junctionIt++) {
			(*junctionIt)->initTrafficLights();
		}
	}
}

void DesignArea::stopSimulation()
{
	if (simulationInProgress) {
		timer->stop();
		simulationInProgress = false;
		//collect and show statistics
	}
}

//struct Way {
//	Junction from;
//	Junction to;
//	std::vector<Connection> steps;
//	int length;
//};

//struct Previous {
//	int cost = 999999;
//	Connection connection;
//	Junction junction;
//};
//
//struct Node {
//	Junction junction;
//	std::vector<Previous> previousConnections;
//};
