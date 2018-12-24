#include "SpawnSettingsEditor.h"

SpawnSettingsEditor::SpawnSettingsEditor(QWidget *parent, Junction* junction)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->junction = junction;
	strcpy(this->name, this->junction->getName());
	codec = QTextCodec::codecForName("Windows-1250");
	ui.label_Actual->setText(codec->toUnicode(this->name));
	ui.tableWidget->setColumnCount(1);
	QStringList list;
	list << codec->toUnicode("Wêze³");
	ui.tableWidget->setHorizontalHeaderLabels(list);
}

SpawnSettingsEditor::~SpawnSettingsEditor()
{
}

void SpawnSettingsEditor::setJunctionVector(std::vector<Junction*> vector)
{
	ui.comboBox_available->clear();
	QStringList junctionList;
	possibleJunctions = vector;
	for (auto possibleIt = possibleJunctions.begin(); possibleIt < possibleJunctions.end(); possibleIt++) {
		ui.comboBox_available->addItem(codec->toUnicode((*possibleIt)->getName()));
	}
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

void SpawnSettingsEditor::addToTableAction() {
	int idx = ui.comboBox_available->currentIndex();
	putJunctionInTable(possibleJunctions[idx]);
	setJunctionVector(possibleJunctions);
}

void SpawnSettingsEditor::acceptButton() {
	//change settings !!!!!!
	accept();
}

void SpawnSettingsEditor::rejectButton() {
	reject();
}
