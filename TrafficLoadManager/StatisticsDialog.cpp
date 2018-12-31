#include "StatisticsDialog.h"

StatisticsDialog::StatisticsDialog(QWidget *parent, std::vector<JunctionStatistics> junctionStatistics)
	: QDialog(parent)
{
	ui.setupUi(this);
	codec = QTextCodec::codecForName("Windows-1250");
	this->junctionStatistics = junctionStatistics;
	QStringList list;
	list << codec->toUnicode("Nazwa skrzy¿owania") << codec->toUnicode("Powinno byæ wyemitowanych") << codec->toUnicode("Pojazdy wyemitowane") << codec->toUnicode("Kierunek") << codec->toUnicode("Przejecha³o samochodów");
	ui.tableWidget->setHorizontalHeaderLabels(list);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

StatisticsDialog::~StatisticsDialog()
{
}

void StatisticsDialog::fillStatistics()
{
	char temporaryForNumbers[20];
	for (auto statsIt = junctionStatistics.begin(); statsIt < junctionStatistics.end(); statsIt++) {
		ui.tableWidget->insertRow(ui.tableWidget->rowCount());
		int counter = ui.tableWidget->rowCount() - 1;
		ui.tableWidget->setItem(counter, 0, new QTableWidgetItem(codec->toUnicode((*statsIt).junctionName)));
		sprintf(temporaryForNumbers, "%d", (*statsIt).shouldBeEmitted);
		ui.tableWidget->setItem(counter, 1, new QTableWidgetItem(codec->toUnicode(temporaryForNumbers)));
		sprintf(temporaryForNumbers, "%d", (*statsIt).emitted);
		ui.tableWidget->setItem(counter, 2, new QTableWidgetItem(codec->toUnicode(temporaryForNumbers)));
		sprintf(temporaryForNumbers, "%d", (*statsIt).overallPassed);
		ui.tableWidget->setItem(counter, 4, new QTableWidgetItem(codec->toUnicode(temporaryForNumbers)));
		for (auto directionsIt = (*statsIt).lightsStats.begin(); directionsIt < (*statsIt).lightsStats.end(); directionsIt++) {
			ui.tableWidget->insertRow(ui.tableWidget->rowCount());
			counter = ui.tableWidget->rowCount() - 1;
			ui.tableWidget->setItem(counter, 3, new QTableWidgetItem(codec->toUnicode((*directionsIt).lightsName)));
			sprintf(temporaryForNumbers, "%d", (*directionsIt).carsPassed);
			ui.tableWidget->setItem(counter, 4, new QTableWidgetItem(codec->toUnicode(temporaryForNumbers)));
		}
	}
}
