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

private:
	char name[100];
	Ui::TrafficLightsEditor ui;
	QTextCodec *codec;
	Junction* junction;
	TrafficLightsSettings trafficLightsSettings;
};
