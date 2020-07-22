#include "mainwindow.h"
#include "player.h"
#include <QLabel>
#include <QKeyEvent>
#include <QPainter>
#include <QFont>
#include <QSound>
#include <QPen>
#include <QString>
#include <QMainWindow>
#include <QImage>
#include <QDebug>
#include <QTimer>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QElapsedTimer>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	setStyleSheet("QWidget {background-image: url(./dataset/background.png) }");
	setFixedSize(1200, 700);
	startWindow = new StartWin;
	connect(startWindow, SIGNAL(changeWindow(bool, bool, bool)), this, SLOT(setup(bool, bool, bool)));
}
void MainWindow::keyPressEvent(QKeyEvent *event) {
	if (event->isAutoRepeat())
		event->ignore();
	else {
		switch(event->key()){
		case Qt::Key_Up:
			rightPlayer->jump();
			break;
		case Qt::Key_Left:
			rightPlayer->startLeft();
			break;
		case Qt::Key_Right:
			rightPlayer->startRight();
			break;
		case Qt::Key_Return:
			rightPlayer->smash();
			break;
		}
	}
}
void MainWindow::keyReleaseEvent(QKeyEvent* event) {
	if (event->isAutoRepeat())
		event->ignore();
	else {
		switch (event->key()){
		case Qt::Key_Left:
			rightPlayer->endMoveX(false);
			break;
		case Qt::Key_Right:
			rightPlayer->endMoveX(true);
			break;
		}
	}
}
void MainWindow::setup(bool s, bool m, bool d) {
	sound = s;	music = m;	demo = d;
	delete startWindow;
	this->show();
	qDebug() << "Sound: " << sound << "\nMusic: " << music << "\nDemo: " << demo;

	if (sound) {
		pikachu = new QSound("./dataset/pikachu_sound_short.wav");
		pikachu_long = new QSound("./dataset/pikachu_sound_long.wav");
	}
	if (music) {
		bgm = new QSound("./dataset/Pikachu_Volleyball_BGM.wav");
		bgm->setLoops(QSound::Infinite);
		bgm->play();
	}
	if (demo)
		setEnabled(false);
	else
		setEnabled(true);
	
	scene = new QGraphicsScene();
	scene->setSceneRect(0, 0, 1200, 700);
	
	view = new QGraphicsView();
	view->setScene(scene);
	view->setFixedSize(1200, 700);
	view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setCentralWidget(view);

	leftBoundary = new QGraphicsLineItem(0, 0, 0, 700);
	rightBoundary = new QGraphicsLineItem(1200, 0, 1200, 700);
	upBoundary = new QGraphicsLineItem(0, 0, 1200, 0);
	leftGround = new QGraphicsLineItem(0, 640, 600, 640);
	rightGround = new QGraphicsLineItem(600, 640, 1200, 640);
	net = new QGraphicsRectItem(595, 440, 10, 200);
	standardLine = new QGraphicsLineItem(0, 540, 1200, 540);
	rightS = new QGraphicsTextItem(QString("%1").arg(rightScore));
	leftS = new QGraphicsTextItem(QString("%1").arg(leftScore));
	
	leftS->setPos(150, 50);
	rightS->setPos(1000, 50);
	QFont font("Calibri", 50, QFont::Bold);
	leftS->setFont(font);
	rightS->setFont(font);
	leftS->setDefaultTextColor(Qt::darkCyan);
	rightS->setDefaultTextColor(Qt::darkCyan);

	rightPlayer = new Player;
	rightPlayer->setup(true, false, rightBoundary, rightGround);
	rightPlayer->reset();

	leftPlayer = new Player;
	leftPlayer->setup(false, true, leftBoundary, leftGround);
	leftPlayer->reset();

	ball = new Ball(upBoundary, leftBoundary, rightBoundary, net, rightPlayer, leftPlayer);
	ball->reset(true);
	ball->setVelocity(1, 0);

	simBall = new BallSimulation;
	simBall->setRect(0, 0, 100, 100);
	simBall->setup(upBoundary, leftBoundary, rightBoundary, net);
	simBall->reset(true);

	scene->addItem(leftBoundary);
	scene->addItem(rightBoundary);
	scene->addItem(upBoundary);
	scene->addItem(leftGround);
	scene->addItem(rightGround);
	scene->addItem(net);
	scene->addItem(rightPlayer);
	scene->addItem(rightPlayer->getNetSide());
	scene->addItem(leftPlayer);
	scene->addItem(leftPlayer->getNetSide());
	scene->addItem(ball);
	scene->addItem(standardLine);
	scene->addItem(simBall);
	scene->addItem(rightS);
	scene->addItem(leftS);

	leftBoundary->hide();
	rightBoundary->hide();
	upBoundary->hide();
	leftGround->hide();
	rightGround->hide();
	net->hide();
	standardLine->hide();
	simBall->hide();

	timer1 = new QTimer(this);
	timer2 = new QTimer(this);
	timer3 = new QTimer(this);
	timer4 = new QTimer(this);
	timer4->setTimerType(Qt::PreciseTimer);

	connect(timer1, SIGNAL(timeout()), this, SLOT(trigger_RP()));
	connect(timer1, SIGNAL(timeout()), this, SLOT(trigger_LP()));
	connect(timer1, SIGNAL(timeout()), this, SLOT(trigger_update()));
	connect(timer1, SIGNAL(timeout()), this, SLOT(trigger_ball1()));
	connect(timer2, SIGNAL(timeout()), this, SLOT(trigger_ball2()));
	connect(timer3, SIGNAL(timeout()), this, SLOT(trigger_simBall1()));
	connect(timer3, SIGNAL(timeout()), this, SLOT(trigger_simBall2()));
	connect(this, SIGNAL(simSignal()), this, SLOT(trigger_sim()));
	connect(timer4, SIGNAL(timeout()), this, SLOT(leftMachine()));
	if (demo)
		connect(timer4, SIGNAL(timeout()), this, SLOT(rightMachine()));

	timer1->start(refreshTime);
	timer2->start(refreshTime_ball);
	timer4->start(refreshTime_machine);

	emit simSignal();
	grabKeyboard();
	view->show();
	scene->update();
	return;
}
void MainWindow::startNewRound(bool rightwin) {
	smash = false;
	timer1->stop();
	timer2->stop();
	timer3->stop();
	timer4->stop();
	if (rightwin) {
		rightScore++;
		ball->reset(true);
		simBall->reset(true);
	}
	else {
		leftScore++;
		ball->reset(false);
		simBall->reset(false);
	}
	rightPlayer->reset();
	leftPlayer->reset();

	rightS->setHtml(QString("%1").arg(rightScore));
	leftS->setHtml(QString("%1").arg(leftScore));

	if (rightScore >= 5 && (rightScore - leftScore) >= 2) {
		if (!deuce) {
			text = new QGraphicsTextItem(QString("Right Side player won"));
			QFont font1("Calibri", 20, QFont::Bold);
			text->setFont(font1);
			scene->addItem(text);
		}
		else {
			text->setHtml(QString("Right Side player won"));
		}
		ball->setPos(550, 200);
		text->setPos(480, 80);
		text->update();
		return;
	}
	else if (leftScore >= 5 && (leftScore - rightScore) >= 2) {
		if (!deuce) {
			text = new QGraphicsTextItem(QString("Left Side player won"));
			QFont font1("Calibri", 20, QFont::Bold);
			text->setFont(font1);
			scene->addItem(text);
		}
		else {
			text->setHtml(QString("Left Side player won"));
		}
		ball->setPos(550, 200);
		text->setPos(480, 80);
		text->update();
		return;
	}
	else if (leftScore >= 5 || rightScore >= 5 || (leftScore == 4 && rightScore == 3) || (rightScore == 4 && leftScore == 3)) {
		if (deuce) {
			text = new QGraphicsTextItem(QString("DEUCE"));
			QFont font1("Calibri", 20, QFont::Bold);
			text->setFont(font1);
			text->setPos(557, 80);
			scene->addItem(text);
			deuce = true;
			text->update();
		}
	}
	rightPlayer->update();
	leftPlayer ->update();
	ball->update();
	simBall->update();
	rightS->update();
	leftS->update();

	QElapsedTimer t;
	t.start();
	while (t.elapsed() < 500);
	emit simSignal();
	timer1->start(refreshTime);
	timer2->start(refreshTime_ball);
	timer4->start(refreshTime_machine);
}
void MainWindow::trigger_sim(void) {
	simBall->setState(ball->x(), ball->y(), ball->getVx(), ball->getVy());
	timer3->start(refreshTime_simBall);
}
void MainWindow::trigger_RP(void) {
	rightPlayer->modifyX_right();
	rightPlayer->modifyY();
}
void MainWindow::trigger_LP(void) {
	leftPlayer->modifyX_left();
	leftPlayer->modifyY();
}
void MainWindow::trigger_update(void) {
	if (ball->collidesWithItem(leftGround)) {
		startNewRound(true);
	}
	else if (ball->collidesWithItem(rightGround)) {
		startNewRound(false);
	}
}
void MainWindow::trigger_ball1(void) {
	ball->collision();
}
void MainWindow::trigger_ball2(void) {
	if (ball->collidesWithItem(rightPlayer)) {
		emit simSignal();
		if (rightPlayer->canSmash) {
			timer2->setInterval(refreshTime_smash);
			smash = true;
			if (sound)
				pikachu_long->play();
		}
		else {
			timer2->setInterval(refreshTime_ball);
			smash = false;
			if (sound)
				pikachu->play();
		}
	}
	else if (ball->collidesWithItem(leftPlayer)) {
		emit simSignal();
		timer2->setInterval(refreshTime_ball);
		smash = false;
		if (sound)
			pikachu->play();
	}
	if (!smash)
		ball->modifySpeed();
	else {
		if (counter)
			ball->modifySpeed();
		counter = !counter;
	}
	ball->modify();
}
void MainWindow::trigger_simBall1(void) {
	simBall->collision();
}
void MainWindow::trigger_simBall2(void) {
	simBall->modify();
	if (simBall->getVy() > 0) {
		if (simBall->collidesWithItem(standardLine))
			timer3->stop();
	}
}
void MainWindow::leftMachine(void) {
	if (simBall->x() > 550) {
		if (leftPlayer->x() < 150)
			leftPlayer->startRight();
		else if (leftPlayer->x() > 250)
			leftPlayer->startLeft();
		else
			leftPlayer->endMoveX();
		return;
	}
	if (leftPlayer->x() < simBall->x() - 75)
		leftPlayer->startRight();
	else if (leftPlayer->x() > simBall->x() - 65)
		leftPlayer->startLeft();
	else
		leftPlayer->endMoveX();
	rightPlayer->update();
	leftPlayer->update();
	simBall->update();
	ball->update();
}
void MainWindow::rightMachine(void) {
	if (simBall->x() < 550) {
		if (rightPlayer->x() < 800)
			rightPlayer->startRight();
		else if (rightPlayer->x() > 900)
			rightPlayer->startLeft();
		else
			rightPlayer->endMoveX();
		return;
	}
	if (rightPlayer->x() < simBall->x() + 18)
		rightPlayer->startRight();
	else if (rightPlayer->x() > simBall->x() + 28)
		rightPlayer->startLeft();
	else
		rightPlayer->endMoveX();
}
