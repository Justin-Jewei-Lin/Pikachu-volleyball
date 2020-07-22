#include "startWin.h"
#include <QCheckBox>
#include <QPainter>
#include <QMainWindow>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QDebug>

StartWin::StartWin(QWidget* parent) : QMainWindow(parent) {
	setStyleSheet("QWidget {background-image: url(./dataset/startwin.png) }");
	setFixedSize(1200, 700);
	soundCBox = new QCheckBox(this);
	soundCBox->move(732, 532);
	soundCBox->resize(15, 15);
	soundCBox->setChecked(true);
	connect(soundCBox, SIGNAL(clicked(bool)), this, SLOT(setSound(bool)));

	musicCBox = new QCheckBox(this);
	musicCBox->move(893, 532);
	musicCBox->resize(15, 15);
	musicCBox->setChecked(true);
	connect(musicCBox, SIGNAL(clicked(bool)), this, SLOT(setMusic(bool)));

	demoCBox = new QCheckBox(this);
	demoCBox->move(732, 576);
	demoCBox->resize(15, 15);
	demoCBox->setChecked(false);
	connect(demoCBox, SIGNAL(clicked(bool)), this, SLOT(setDemo(bool)));
	show();
	setMouseTracking(true);
	update();
}
StartWin::~StartWin(void) {
	disconnect(soundCBox, SIGNAL(clicked(bool)), this, SLOT(setSound(bool)));
	disconnect(musicCBox, SIGNAL(clicked(bool)), this, SLOT(setMusic(bool)));
	disconnect(demoCBox, SIGNAL(clicked(bool)), this, SLOT(setDemo(bool)));
	delete soundCBox;
	delete musicCBox;
	delete demoCBox;
}
void StartWin::setSound(bool checked) {
	if (checked)
		sound = true;
	else
		sound = false;
}
void StartWin::setMusic(bool checked) {
	if (checked)
		music = true;
	else
		music = false;
}
void StartWin::setDemo(bool checked) {
	if (checked)
		demo = true;
	else
		demo = false;
}
void StartWin::mouseReleaseEvent(QMouseEvent* event) {
	if (event->x() >= 727 && event->x() <= 935 && event->y() >= 466 && event->y() <= 516) {
		setMouseTracking(false);
		qDebug() << "Pressed";
		emit changeWindow(sound, music, demo);
	}
}
void StartWin::keyPressEvent(QKeyEvent* event) {
	if (event->key() == Qt::Key_Return) {
		setMouseTracking(false);
		qDebug() << "Pressed";
		emit changeWindow(sound, music, demo);
	}
}