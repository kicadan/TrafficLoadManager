#pragma once

#include <QDialog>
#include "ui_StatisticsDialog.h"
#include "Junction.h"

class StatisticsDialog : public QDialog
{
	Q_OBJECT

public:
	StatisticsDialog(QWidget *parent, std::vector<JunctionStatistics>);
	~StatisticsDialog();

	void fillStatistics();

private:
	Ui::StatisticsDialog ui;

	QTextCodec *codec;
	std::vector<JunctionStatistics> junctionStatistics;
};
