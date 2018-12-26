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

	void setJunctionComboBox(std::vector<Junction*>);
	void setSpawnEditorFields();

private:
	char name[100];
	Ui::SpawnSettingsEditor ui;
	CarSpawnSettings spawnSettings;
	Junction* junction;
	std::vector<Junction*> possibleJunctions;
	std::vector<Junction*> tableJunctions;
	QTextCodec *codec;

	std::vector<Junction*> differentialBetweenTwoJunctionVectors(std::vector<Junction*>, std::vector<Junction*>);
	void putJunctionInTable(Junction*);
	void deleteJunctionFromTable(int);

public slots:
	void addToTableAction();
	void deleteFromTableAction(int, int);
	void acceptButton();
	void rejectButton();
};
