#include <QtWidgets/QApplication>
#include "TrafficLoadManager.h"
#include <qgraphicsscene.h>
#include <qgraphicsitem.h>
#include <qgraphicsview.h>
#include <qcolor.h>
#include <qpainter.h>

#define width 1920
#define height 1080

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	/*QGraphicsScene * scene = new QGraphicsScene();
	QGraphicsRectItem * item = new QGraphicsRectItem();
	item->setRect(0, 0, 100, 100);
	item->setRotation(10);
	item->setScale(5);
	scene->addItem(item);

	QGraphicsView * view = new QGraphicsView(scene);
	view->show();*/
	TrafficLoadManager w;
	w.setWindowTitle("TrafficLoadManager");
	w.resize(width, height);
	w.show();

	return a.exec();
}
