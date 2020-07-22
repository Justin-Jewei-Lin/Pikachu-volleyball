#ifndef _BALLSIMULATION_H_
#define _BALLSIMULATION_H_

#include <QGraphicsEllipseItem>

class BallSimulation : public QGraphicsEllipseItem {
public:
	void setup(QGraphicsLineItem*, QGraphicsLineItem*, QGraphicsLineItem*, QGraphicsRectItem*);
	void reset(bool);
	void setState(qreal, qreal, qreal, qreal);
	void modify(void);
	void collision(void);
	qreal getVy(void);
private:
	qreal velocity_x = 0;
	qreal velocity_y = 0;
	const qreal gravity = 0.16;

	bool canCollide_net = true;

	QGraphicsLineItem* upBound;
	QGraphicsLineItem* leftBound;
	QGraphicsLineItem* rightBound;
	QGraphicsRectItem* net;
	void reflectTransformation(qreal&, qreal&, qreal, qreal);
};

#endif