#include "TrafficLoadManager.h"

TrafficLoadManager::TrafficLoadManager(QWidget *parent)
	: QMainWindow(parent)
{
	designArea = ui.designArea;
	ui.setupUi(this);
}

void TrafficLoadManager::resizeEvent(QResizeEvent * event)
{
}

