#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <QGraphicsPixmapItem>

class Player : public QGraphicsPixmapItem {
public:
	void setup(bool, bool, QGraphicsLineItem*, QGraphicsLineItem*);
	void reset(void);
	void startRight(void);
	void startLeft(void);
	void endMoveX(bool);
	void endMoveX(void);
	void jump(void);
	void smash(void);
	void setVelocity(qreal, qreal);
	void modifyY(void);
	void modifyX_right(void);
	void modifyX_left(void);
	bool canSmash = false;
	int smashCounter = 0;
	QGraphicsLineItem* getNetSide(void);
private:
	qreal velocity_x;
	qreal velocity_y;
	bool rightPlayer;
	bool machine;
	const qreal jumpSpeed = -9;
	const qreal moveSpeed = 4;
	const qreal gravity = 0.16;

	bool onGround = false;
	QGraphicsLineItem* netSide;
	QGraphicsLineItem* side;
	QGraphicsLineItem* ground;
};

#endif