#include "ball.h"
#include "player.h"
#include <QtMath>

Ball::Ball(QGraphicsLineItem* up, QGraphicsLineItem* left, QGraphicsLineItem* right, QGraphicsRectItem* n, Player* rp, Player* lp) 
	:upBound(up), leftBound(left), rightBound(right), net(n), rightPlayer(rp), leftPlayer(lp) {
	setPixmap(QPixmap("./dataset/pokeball.png").scaledToHeight(100));
}
void Ball::reset(bool right) {
	if (right)
		setPos(850, 0);
	else
		setPos(250, 0);
	canCollide_LP = true;
	canCollide_RP = true;
	canCollide_net = true;
	setVelocity(0, 0);
}
void Ball::setVelocity(qreal vx, qreal vy) {
	velocity_x = vx;
	velocity_y = vy;
}
void Ball::modify(void) {
	setX(x() + velocity_x);
	setY(y() + velocity_y);
}
void Ball::modifySpeed(void) {
	velocity_y = velocity_y + gravity;
}
void Ball::collision(void) {
	if (collidesWithItem(rightPlayer)) {
		if (canCollide_RP)
			reflectTransformation(velocity_x, velocity_y, (rightPlayer->x() - x() + 5), (rightPlayer->y() - y() + 24));
		canCollide_RP = false;
	}
	else
		canCollide_RP = true;
	if (collidesWithItem(leftPlayer)) {
		if (canCollide_LP)
			reflectTransformation(velocity_x, velocity_y, (leftPlayer->x() - x() + 40), (leftPlayer->y() - y() + 32));
		canCollide_LP = false;
	}
	else
		canCollide_LP = true;
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
	if (collidesWithItem(leftBound) && velocity_x < 0)
		velocity_x = -velocity_x;
	else if (collidesWithItem(rightBound) && velocity_x > 0)
		velocity_x = -velocity_x;
	if (collidesWithItem(upBound))
		velocity_y = -velocity_y;
}
void Ball::reflectTransformation(qreal &vx, qreal &vy, qreal x, qreal y) {
	qreal ux = x / qSqrt(qPow(x, 2) + qPow(y, 2));
	qreal uy = y / qSqrt(qPow(x, 2) + qPow(y, 2));
	qreal ox = vx, oy = vy;
	vx = (-(2 * qPow(ux, 2) - 1) * ox - 2 * ux * uy * oy) * 1.011;
	vy = (-2 * ux * uy * ox - (2 * qPow(uy, 2) - 1) * oy) * 1.011;
	return;
}
qreal Ball::getVx(void) {
	return velocity_x;
}
qreal Ball::getVy(void) {
	return velocity_y;
}