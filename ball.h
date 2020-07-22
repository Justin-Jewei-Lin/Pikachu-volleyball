#ifndef _BALL_H_
#define _BALL_H_

#include "player.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>

class Ball : public QGraphicsPixmapItem {
public:
	Ball(QGraphicsLineItem*, QGraphicsLineItem*, QGraphicsLineItem*, QGraphicsRectItem*, Player*, Player*);
	void reset(bool);
	void setVelocity(qreal, qreal);
	void modify(void);
	void modifySpeed(void);
	void collision(void);
	qreal getVx(void);
	qreal getVy(void);
private:
	qreal velocity_x;
	qreal velocity_y;
	const qreal gravity = 0.16;

	bool canCollide_LP = true;
	bool canCollide_RP = true;
	bool canCollide_net = true;

	QGraphicsLineItem* upBound;
	QGraphicsLineItem* leftBound;
	QGraphicsLineItem* rightBound;
	QGraphicsRectItem* net;
	Player *rightPlayer;
	Player *leftPlayer;

	void reflectTransformation(qreal&, qreal&, qreal, qreal);
};

#endif