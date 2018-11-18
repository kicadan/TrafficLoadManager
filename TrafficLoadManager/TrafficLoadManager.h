#pragma once

#include <QtWidgets/QMainWindow>
#include <QList>
#include "ui_TrafficLoadManager.h"

class TrafficLoadManager : public QMainWindow
{
	Q_OBJECT

public:
	TrafficLoadManager(QWidget *parent = Q_NULLPTR);

private:
	Ui::TrafficLoadManagerClass ui;
	DesignArea *designArea;

protected:
	void resizeEvent(QResizeEvent *event) override;
};
