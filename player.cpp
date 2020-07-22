#include "player.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsLineItem>
#include <QString>
#include <QDebug>
#include <QTimer>
#include <QPen>

void Player::setup(bool r, bool m, QGraphicsLineItem* s, QGraphicsLineItem* g) {
	rightPlayer = r;
	machine = m;
	side = s;
	ground = g;
	qDebug() << rightPlayer << machine;
	QString pic = (rightPlayer) ? "./dataset/pikachu_right.png" : "./dataset/pikachu_left.png";
	setPixmap(QPixmap(pic).scaledToHeight(150));
	
	if (rightPlayer)
		netSide = new QGraphicsLineItem(605, 0, 605, 640);
	else
		netSide = new QGraphicsLineItem(595, 0, 595, 640);
	QPen pen(Qt::transparent);
	netSide->setPen(pen);
}
void Player::reset(void) {
	if (rightPlayer)
		setPos(850, 500);
	else
		setPos(200, 500);
	setVelocity(0, 0);
	canSmash = false;
	smashCounter = 0;
}
void Player::startRight(void) {
	velocity_x = moveSpeed;
}
void Player::startLeft(void) {
	velocity_x = -moveSpeed;
}
void Player::endMoveX(bool right) {
	if ((right && velocity_x > 0) || (!right && velocity_x < 0))
		velocity_x = 0;
}
void Player::endMoveX(void) {
	velocity_x = 0;
}
void Player::jump(void) {
	if (onGround) {
		velocity_y = jumpSpeed;
		onGround = false;
	}
}
void Player::smash(void) {
	if (!onGround)
		canSmash = true;
}
void Player::modifyX_right(void) {
	if (!((collidesWithItem(netSide) && velocity_x < 0) || (collidesWithItem(side) && velocity_x > 0)))
		setX(x() + velocity_x);
	if (canSmash) {
		smashCounter++;
		if (smashCounter >= 20) {
			smashCounter = 0;
			canSmash = false;
		}
	}
}
void Player::modifyX_left(void) {
	if (!((collidesWithItem(netSide) && velocity_x > 0) || (collidesWithItem(side) && velocity_x < 0))) {
		setX(x() + velocity_x);
	}
}
void Player::modifyY(void) {
	if (!onGround) {
		if (collidesWithItem(ground) && velocity_y > 0) {
			onGround = true;
			velocity_y = 0;
		}
		else {
			velocity_y = velocity_y + gravity;
			setY(y() + velocity_y);
		}
	}
}
void Player::setVelocity(qreal vx, qreal vy) {
	velocity_x = vx;
	velocity_y = vy;
}
QGraphicsLineItem* Player::getNetSide(void) {
	return netSide;
}