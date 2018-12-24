#pragma once

#include <QDialog>
#include <qtextcodec.h>
#include <string>
#include "ui_SpawnSettingsEditor.h"
#include "Junction.h"

class SpawnSettingsEditor : public QDialog
{
	Q_OBJECT

public:
	SpawnSettingsEditor(QWidget *parent, Junction*);
	~SpawnSettingsEditor();

	void setJunctionVector(std::vector<Junction*>);

private:
	char name[100];
	Ui::SpawnSettingsEditor ui;
	CarSpawnSettings spawnSettings;
	Junction* junction;
	std::vector<Junction*> possibleJunctions;
	std::vector<Junction*> tableJunctions;
	QTextCodec *codec;

	void putJunctionInTable(Junction*);

public slots:
	void addToTableAction();
	void acceptButton();
	void rejectButton();
};
