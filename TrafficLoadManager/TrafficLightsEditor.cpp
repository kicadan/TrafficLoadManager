#include "TrafficLightsEditor.h"

TrafficLightsEditor::TrafficLightsEditor(QWidget *parent, Junction* junction)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->junction = junction;
	this->trafficLightsSettings = junction->getTrafficLightsSettings();
	strcpy(this->name, this->junction->getName());
	lightsInTable = trafficLightsSettings.lightsSequence;
	lightsInCombo = differentialBetweenTwoLightsVectors(trafficLightsSettings.lights, trafficLightsSettings.lightsSequence);
	lights = trafficLightsSettings.lights;
	for (auto lightsIt = lightsInTable.begin(); lightsIt < lightsInTable.end(); lightsIt++) {
		totalGreenLight += (*lightsIt).greenLight.time;
	}
	codec = QTextCodec::codecForName("Windows-1250");
	ui.label_skrzyzowanie->setText(codec->toUnicode(this->name)); 
	QStringList list;
	ui.tableWidget->setColumnCount(2);
	list << codec->toUnicode("Kierunek") << codec->toUnicode("Czas zielonego œwiat³a [s]");
	ui.tableWidget->setHorizontalHeaderLabels(list);
}

TrafficLightsEditor::~TrafficLightsEditor()
{
}

void TrafficLightsEditor::setLightsComboBox()
{
	ui.comboBox_kierunki->clear();
	for (auto possibleIt = lightsInCombo.begin(); possibleIt < lightsInCombo.end(); possibleIt++) {
		ui.comboBox_kierunki->addItem(codec->toUnicode((*possibleIt).lightsName));
	}
}

void TrafficLightsEditor::setLightsEditorFields()
{
	setLightsComboBox();
	for (auto lightsIt = lightsInTable.begin(); lightsIt < lightsInTable.end(); lightsIt++) {
		char greenTime[5];
		sprintf(greenTime, "%d", (*lightsIt).greenLight.time);
		ui.tableWidget->insertRow(ui.tableWidget->rowCount());
		int counter = ui.tableWidget->rowCount() - 1;
		ui.tableWidget->setItem(counter, 0, new QTableWidgetItem(codec->toUnicode((*lightsIt).lightsName)));
		ui.tableWidget->setItem(counter, 1, new QTableWidgetItem(codec->toUnicode(greenTime)));
	}
}

std::vector<Lights> TrafficLightsEditor::differentialBetweenTwoLightsVectors(std::vector<Lights> minuend, std::vector<Lights> subtrahend)
{
	std::vector<Lights> differential = minuend;
	//not need to sort since it already sorted
	if (subtrahend.size() > 0) {
		//std::set_difference(minuend.begin(), minuend.end(), subtrahend.begin(), subtrahend.end(), std::inserter(differential, differential.begin())); very unstable
		for (auto subtrahendIt = subtrahend.begin(); subtrahendIt < subtrahend.end(); subtrahendIt++) {
			for (auto minuendIt = differential.begin(); minuendIt < differential.end(); minuendIt++)
				if ((*subtrahendIt).lightsId == (*minuendIt).lightsId) {
					differential.erase(minuendIt);
					break;
				}
		}
	}
	return differential;
}

void TrafficLightsEditor::putLightsInTable(Lights addedLights)
{
	for (auto possibleIt = lightsInCombo.begin(); possibleIt < lightsInCombo.end(); possibleIt++) {
		if ((*possibleIt).lightsId == addedLights.lightsId) {
			lightsInCombo.erase(possibleIt);
			break;
		}
	}
	lightsInTable.push_back(addedLights);
	char greenTime[5];
	sprintf(greenTime, "%d", addedLights.greenLight.time);
	ui.tableWidget->insertRow(ui.tableWidget->rowCount());
	int counter = ui.tableWidget->rowCount() - 1;
	ui.tableWidget->setItem(counter, 0, new QTableWidgetItem(codec->toUnicode(addedLights.lightsName)));
	ui.tableWidget->setItem(counter, 1, new QTableWidgetItem(codec->toUnicode(greenTime)));
}

void TrafficLightsEditor::updateLightsInTables(Lights updatedLights)
{
	for (auto lightsIt = lightsInCombo.begin(); lightsIt < lightsInCombo.end(); lightsIt++) {
		if ((*lightsIt).lightsId == updatedLights.lightsId) {
			(*lightsIt) = updatedLights;
			break;
		}
	}
	for (auto lightsIt = lightsInTable.begin(); lightsIt < lightsInTable.end(); lightsIt++) {
		if ((*lightsIt).lightsId == updatedLights.lightsId) {
			(*lightsIt) = updatedLights;
			break;
		}
	}
}

void TrafficLightsEditor::deleteLightsFromTable(int idx)
{
	Lights deleted = lightsInTable[idx];
	totalGreenLight -= deleted.greenLight.time;
	lightsInTable.erase(lightsInTable.begin() + idx);
	lightsInCombo.push_back(deleted);
	setLightsComboBox();
}

void TrafficLightsEditor::addToTableAction() {
	int idx = ui.comboBox_kierunki->currentIndex();
	if (lightsInCombo.size() > 0) {
		Lights addedLights = lightsInCombo[idx];
		addedLights.greenLight = LightSequence{ GREEN, ui.spinBox_green->value() };
		totalGreenLight += ui.spinBox_green->value();
		updateLightsInTables(addedLights);
		putLightsInTable(addedLights);
		setLightsComboBox();
	}
}

void TrafficLightsEditor::deleteFromTableAction(int row, int)
{
	deleteLightsFromTable(row);
	ui.tableWidget->removeRow(row);
}

void TrafficLightsEditor::resetSettings()
{
	int rowCount = ui.tableWidget->rowCount();
	for (int i = rowCount - 1; i >= 0; i--) {
		deleteLightsFromTable(i);
		ui.tableWidget->removeRow(i);
	}
	lightsInCombo = lights;
	setLightsComboBox();
}

void TrafficLightsEditor::acceptButton() {
	//change settings !!!!!!
	if (lights.size() == lightsInTable.size()) {
		trafficLightsSettings.upToDate = true;
		for (auto lightsIt = lightsInTable.begin(); lightsIt < lightsInTable.end(); lightsIt++) {
			(*lightsIt).redLight = LightSequence{ RED, totalGreenLight - (*lightsIt).greenLight.time };
		}
	}
	else
		trafficLightsSettings.upToDate = false;
	//update lights with lights from table
	for (auto lightsIt = lights.begin(); lightsIt < lights.end(); lightsIt++) {
		for (auto tableLightsIt = lightsInTable.begin(); tableLightsIt < lightsInTable.end(); tableLightsIt++) {
			if ((*tableLightsIt).lightsId == (*lightsIt).lightsId)
				(*lightsIt) = (*tableLightsIt);
		}
	}
	trafficLightsSettings.lights = lights;
	trafficLightsSettings.lightsSequence = lightsInTable;
	junction->setTrafficLightsSettings(trafficLightsSettings);
	accept();
}

void TrafficLightsEditor::rejectButton() {
	reject();
}