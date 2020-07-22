#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include "startWin.h"
#include "player.h"
#include "ball.h"
#include "ballSimulation.h"
#include <QMainWindow>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QSound>

class MainWindow : public QMainWindow {
	Q_OBJECT
public:
	MainWindow(QWidget *parent = 0);
public slots:
	void setup(bool, bool, bool);
	void trigger_update(void);
	void trigger_RP(void);
	void trigger_LP(void);
	void trigger_ball1(void);
	void trigger_ball2(void);
	void trigger_simBall1(void);
	void trigger_simBall2(void);
	void trigger_sim(void);
	void leftMachine(void);
	void rightMachine(void);
protected:
	void keyPressEvent(QKeyEvent* event);
	void keyReleaseEvent(QKeyEvent* event);
	bool sound;
	bool music;
	bool demo;
private:
	StartWin* startWindow;
	Player* rightPlayer;
	Player* leftPlayer;
	Ball* ball;
	BallSimulation* simBall;
	QGraphicsLineItem* leftBoundary;
	QGraphicsLineItem* rightBoundary;
	QGraphicsLineItem* upBoundary;
	QGraphicsLineItem* leftGround;
	QGraphicsLineItem* rightGround;
	QGraphicsRectItem* net;
	QGraphicsLineItem* standardLine;
	QGraphicsTextItem* rightS;
	QGraphicsTextItem* leftS;
	QGraphicsTextItem* text = NULL;

	QSound* bgm;
	QSound* pikachu;
	QSound* pikachu_long;
	bool deuce = false;
	bool smash = false;
	bool counter = true;

	QTimer* timer1; // For player
	QTimer* timer2; // For ball
	QTimer* timer3; // For simulation ball
	QTimer* timer4; // For machine
	int rightScore = 0;
	int leftScore = 0;
	const qreal refreshTime = 10;
	const qreal refreshTime_ball = 10;
	const qreal refreshTime_simBall = 1;
	const qreal refreshTime_machine = 30;
	const qreal refreshTime_smash = 5;
	QGraphicsScene* scene;
	QGraphicsView* view;
	void startNewRound(bool);
signals:
	simSignal(void);
};
#endif
