#include "TrafficLightsEditor.h"

TrafficLightsEditor::TrafficLightsEditor(QWidget *parent, Junction* junction)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->junction = junction;
	this->trafficLightsSettings = junction->getTrafficLightsSettings();
	strcpy(this->name, this->junction->getName());
	codec = QTextCodec::codecForName("Windows-1250");
	ui.label_skrzyzowanie->setText(codec->toUnicode(this->name)); 
	QStringList list;
	ui.tableWidget->setColumnCount(3);
	list << codec->toUnicode("Wêze³") << codec->toUnicode("Wêze³") << codec->toUnicode("Wêze³");
	ui.tableWidget->setHorizontalHeaderLabels(list);
}

TrafficLightsEditor::~TrafficLightsEditor()
{
}
