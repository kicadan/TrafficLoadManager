#include "DesignArea.h"

DesignArea::DesignArea(QWidget *parent) : QOpenGLWidget(parent)
{/*
	QSurfaceFormat format;
	format.setProfile(QSurfaceFormat::CompatibilityProfile);
	format.setVersion(2, 1);
	
	context = new QOpenGLContext;
	context->setFormat(format);
	context->create();
	context->makeCurrent(this);

	openGLFunctions = context->functions();*/
	//connect(&timer, SIGNAL(timeout()), this, SLOT(updateGL()));
	//timer.start(16);
}

DesignArea::~DesignArea()
{
}
/*
void DesignArea::drawLineTo(const QPoint & endPoint)
{
	QPainter painter(&image);
	painter.setPen(QPen(Qt::blue, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter.drawLine(lastPoint, endPoint);

	int rad = (4 / 2) + 2;
	update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
	lastPoint = endPoint;
}*/

void DesignArea::initializeGL()
{
	resizeGL(this->width(), this->height());
}

void DesignArea::resizeGL(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, w, h);
	glLoadIdentity();

	qreal aspectRatio = qreal(w) / qreal(h);
	//glOrtho(-1 * aspectRatio, 1 * aspectRatio, -1 * aspectRatio, 1 * aspectRatio, 1, -1);
	gluOrtho2D(0, w, h, 0);
	//glLoadIdentity();
}

void DesignArea::paintGL()
{
	if (constructing)
	{
		drawRoad();
	}
}

void DesignArea::resizeEvent(QResizeEvent * event)
{
	resizeGL(this->width(), this->height());
}

void DesignArea::paintEvent(QPaintEvent * event)
{
	QPainter painter;
	painter.begin(this);
	paintGL();
	painter.end();
}

void DesignArea::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton) {
		firstPoint = event->pos();
		constructing = true;
	}
}
/*
void DesignArea::mouseMoveEvent(QMouseEvent * event)
{
	if (event->buttons() & Qt::LeftButton && constructing == true) {
		lastPoint = event->pos();
		repaint();
	}
}*/

void DesignArea::mouseReleaseEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton && constructing == true) {
		lastPoint = event->pos();
		repaint();
	}
}

void DesignArea::drawLine()
{
	constructing = false;
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(lastPoint.x(), lastPoint.y());
	glVertex2f( firstPoint.x(), firstPoint.y());
	glEnd();
	/*glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f((float)lastPoint.x(), (float)lastPoint.y());
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2f((float)firstPoint.x(), (float)firstPoint.y());
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(600.0f, 600.0f);
	glEnd();*/
}

void DesignArea::drawRoad()
{/*
	constructing = false;
	glLineWidth(3);
	float x, y;
	vectors parallel_segments = calc_vectors(lastPoint, firstPoint);

	glBegin(GL_LINES);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex2f(lastPoint.x(), lastPoint.y());
		glVertex2f(firstPoint.x(), firstPoint.y());
	glEnd();

	glBegin(GL_LINES);
		glColor3f(0.0f, 1.0f, 0.0f);
		x = round(lastPoint.x() + parallel_segments.x1);
		y = round(lastPoint.y() + parallel_segments.y1);
		glVertex2f(x, y);
		x = round(firstPoint.x() + parallel_segments.x1);
		y = round(firstPoint.y() + parallel_segments.y1);
		glVertex2f(x, y);
	glEnd();

	glBegin(GL_LINES);
		glColor3f(0.0f, 1.0f, 0.0f);
		x = round(lastPoint.x() + parallel_segments.x2);
		y = round(lastPoint.y() + parallel_segments.y2);
		glVertex2f(x, y);
		x = round(firstPoint.x() + parallel_segments.x2);
		y = round(firstPoint.y() + parallel_segments.y2);
		glVertex2f(x, y);
	glEnd();
	*/
	switch (currentObjectBrush)
	{
	case OneWayOneLane: {
		Roads::OneWayOneLane road = Roads::OneWayOneLane();
		road.drawLane(firstPoint, lastPoint);
		constructing = false;
		appObjects.push_back(road);
	}
	}
}

/*
	vectors to calc two parallel segments
	yb^2 * (ya^2/xa^2) - yb - odl^2 = 0
	delta = b^2 - 4ac  -->  float delta = sqrt( 1 + 4 * odl^2 * (ya^2/xa^2) )
*/
vectors DesignArea::calc_vectors(QPoint A, QPoint B)
{
	//int xa = A.x() - B.x();
	//int ya = A.y() - B.y();
	//double xd = pow(odl, 2);
	//double delta = sqrt(1 + 4 * pow(odl, 2) * (pow(ya, 2) / pow(xa, 2) ));
	//double ya_sq = ya ^ 2;
	//return vectors { /*x1*/(1 - delta) / (2 * (pow(ya, 2) / pow(xa, 2))),
		///*y1*/ (-xa * (1 - delta) / (2 * (pow(ya, 2) / pow(xa, 2)))) / ya,
		///*x2*/ (1 + delta) / (2 * (pow(ya, 2) / pow(xa, 2))),
		///*y2*/ (-xa * (1 + delta) / (2 * (pow(ya, 2) / pow(xa, 2)))) / ya };
	double xa = A.x() - B.x();
	double ya = A.y() - B.y();
	double radius = sqrt(xa*xa + ya * ya);
	return vectors{ ya*distance / radius, -xa * distance / radius, -ya * distance / radius, xa * distance / radius };
}
