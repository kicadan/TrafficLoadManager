#include "SpawnSettingsEditor.h"

SpawnSettingsEditor::SpawnSettingsEditor(QWidget *parent, Junction* junction)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->junction = junction;
	strcpy(this->name, this->junction->getName());
	codec = QTextCodec::codecForName("Windows-1250");
	ui.lineEdit_Actual->setText(codec->toUnicode(this->name));
	QStringList list;
	ui.tableWidget->setColumnCount(1);
	list << codec->toUnicode("Wêze³");
	ui.tableWidget->setHorizontalHeaderLabels(list);
}

SpawnSettingsEditor::~SpawnSettingsEditor()
{
}

void SpawnSettingsEditor::setJunctionComboBox(std::vector<Junction*> vector)
{
	ui.comboBox_available->clear();
	QStringList junctionList;
	std::vector<Junction*> realPossibleJunctions = differentialBetweenTwoJunctionVectors(vector, this->spawnSettings.destinations);
	realPossibleJunctions.erase(std::remove(realPossibleJunctions.begin(), realPossibleJunctions.end(), this->junction), realPossibleJunctions.end());
	possibleJunctions = realPossibleJunctions;
	for (auto possibleIt = possibleJunctions.begin(); possibleIt < possibleJunctions.end(); possibleIt++) {
		ui.comboBox_available->addItem(codec->toUnicode((*possibleIt)->getName()));
	}
}

std::vector<Junction*> SpawnSettingsEditor::differentialBetweenTwoJunctionVectors(std::vector<Junction*> minuend, std::vector<Junction*> subtrahend)
{
	std::vector<Junction*> differential = minuend;
	//not need to sort since it already sorted
	if (subtrahend.size() > 0) {
		//std::set_difference(minuend.begin(), minuend.end(), subtrahend.begin(), subtrahend.end(), std::inserter(differential, differential.begin())); very unstable
		for (auto subtrahendIt = subtrahend.begin(); subtrahendIt < subtrahend.end(); subtrahendIt++) {
			for (auto minuendIt = differential.begin(); minuendIt < differential.end(); minuendIt++)
				if ((*subtrahendIt)->getId() == (*minuendIt)->getId()) {
					differential.erase(minuendIt);
					break;
				}
		}
	}
	return differential;
}

void SpawnSettingsEditor::putJunctionInTable(Junction *junction)
{
	for (auto possibleIt = possibleJunctions.begin(); possibleIt < possibleJunctions.end(); possibleIt++) {
		if ((*possibleIt)->getId() == junction->getId()) {
			possibleJunctions.erase(possibleIt);
			break;
		}
	}
	tableJunctions.push_back(junction);
	ui.tableWidget->insertRow(ui.tableWidget->rowCount());
	int counter = ui.tableWidget->rowCount() - 1;
	ui.tableWidget->setItem(counter, 0, new QTableWidgetItem(codec->toUnicode(junction->getName())));
}

void SpawnSettingsEditor::deleteJunctionFromTable(int idx)
{
	Junction* deleted = tableJunctions[idx];
	tableJunctions.erase(tableJunctions.begin() + idx);
	spawnSettings.destinations = tableJunctions;
	possibleJunctions.push_back(deleted);
	setJunctionComboBox(possibleJunctions);
}

void SpawnSettingsEditor::setSpawnEditorFields()
{
	spawnSettings = this->junction->getCarSpawnSettings();
	tableJunctions = spawnSettings.destinations;
	ui.spinBox_cars->setValue(spawnSettings.carsPerMinute);
	ui.spinBox_smart_drivers->setValue(spawnSettings.smartDrivers);
	for (auto junctionIt = tableJunctions.begin(); junctionIt < tableJunctions.end(); junctionIt++) {
		ui.tableWidget->insertRow(ui.tableWidget->rowCount());
		int counter = ui.tableWidget->rowCount() - 1;
		ui.tableWidget->setItem(counter, 0, new QTableWidgetItem(codec->toUnicode((*junctionIt)->getName())));
	}
}

void SpawnSettingsEditor::addToTableAction() {
	int idx = ui.comboBox_available->currentIndex();
	if (possibleJunctions.size() > 0) {
		putJunctionInTable(possibleJunctions[idx]);
		setJunctionComboBox(possibleJunctions);
	}
}

void SpawnSettingsEditor::deleteFromTableAction(int row, int)
{
	deleteJunctionFromTable(row);
	ui.tableWidget->removeRow(row);
}

void SpawnSettingsEditor::acceptButton() {
	//change settings !!!!!!
	strcpy(name, codec->fromUnicode(ui.lineEdit_Actual->text()));
	junction->setName(this->name);
	this->spawnSettings = CarSpawnSettings{ ui.spinBox_cars->value(), ui.spinBox_smart_drivers->value(), tableJunctions };
	junction->editCarSpawn(this->spawnSettings);
	accept();
}

void SpawnSettingsEditor::rejectButton() {
	reject();
}
