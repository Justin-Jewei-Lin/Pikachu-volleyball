#include "ballSimulation.h"
#include <QGraphicsEllipseItem>
#include <QPen>
#include <QtMath>

void BallSimulation::setup(QGraphicsLineItem* u, QGraphicsLineItem* l, QGraphicsLineItem* r, QGraphicsRectItem* n) {
	upBound = u;
	leftBound = l;
	rightBound = r;
	net = n;
}
void BallSimulation::reset(bool right) {
	if (right)
		setPos(850, 0);
	else
		setPos(250, 0);
	velocity_x = 0;
	velocity_y = 0;
	canCollide_net = true;
}
void BallSimulation::setState(qreal x, qreal y, qreal vx, qreal vy) {
	setPos(x, y);
	velocity_x = vx;
	velocity_y = vy;
}
void BallSimulation::modify(void) {
	velocity_y = velocity_y + gravity;
	setX(x() + velocity_x);
	setY(y() + velocity_y);
}
void BallSimulation::collision(void) {
	if (collidesWithItem(net)) {
		if (canCollide_net) {
			if (y() >= 390)
				velocity_x = -velocity_x;
			else
				reflectTransformation(velocity_x, velocity_y, (x() - 550), (y() - 390));
		}
		canCollide_net = false;
	}
	else
		canCollide_net = true;
	if (velocity_x < 0) {
		if (collidesWithItem(leftBound))
				velocity_x = -velocity_x;
	}
	else if (velocity_x > 0) {
		if (collidesWithItem(rightBound))
			velocity_x = -velocity_x;
	}
	if (collidesWithItem(upBound))
		velocity_y = -velocity_y;
}
void BallSimulation::reflectTransformation(qreal& vx, qreal& vy, qreal x, qreal y) {
	qreal ux = x / qSqrt(qPow(x, 2) + qPow(y, 2));
	qreal uy = y / qSqrt(qPow(x, 2) + qPow(y, 2));
	qreal ox = vx, oy = vy;
	vx = -(2 * qPow(ux, 2) - 1) * ox - 2 * ux * uy * oy;
	vy = -2 * ux * uy * ox - (2 * qPow(uy, 2) - 1) * oy;
	return;
}
qreal BallSimulation::getVy(void) {
	return velocity_y;
}