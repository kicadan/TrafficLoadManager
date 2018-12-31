#pragma once

#include <QDialog>
#include "ui_TrafficLightsEditor.h"
#include "Junction.h"

class TrafficLightsEditor : public QDialog
{
	Q_OBJECT

public:
	TrafficLightsEditor(QWidget *parent, Junction*);
	~TrafficLightsEditor();

	void setLightsComboBox();
	void setLightsEditorFields();

private:
	char name[100];
	Ui::TrafficLightsEditor ui;
	QTextCodec *codec;
	Junction* junction;
	TrafficLightsSettings trafficLightsSettings;
	std::vector<Lights> lightsInTable;
	std::vector<Lights> lightsInCombo;
	std::vector<Lights> lights;
	int totalGreenLight = 0;

	std::vector<Lights> differentialBetweenTwoLightsVectors(std::vector<Lights>, std::vector<Lights>);
	void putLightsInTable(Lights);
	void updateLightsInTables(Lights);
	void deleteLightsFromTable(int);
	void showFocusedLights(int);

public slots:
	void addToTableAction();
	void deleteFromTableAction(int, int);
	void resetSettings();
	void acceptButton();
	void rejectButton();
	void comboBoxIndexChanged(int);
};

//enum Light {
//	RED = 0,
//	GREEN = 1
//};
//
//struct LightSequence {
//	Light light;
//	int time; //seconds
//};
//
//struct Lights {
//	LightSequence redLight{ RED, 0 };
//	LightSequence greenLight{ GREEN, 0 };
//	Point lightsPoint;
//	QLineF berm;
//	bool haveConnection = false;
//	short direction; //on which side of the road it is
//	int roadId;
//	int lightsId;
//};
//
//struct TrafficLightsSettings {
//	std::vector<Lights> lights;
//	std::vector<Lights> lightsSequence;
//	bool upToDate = false;
//};
