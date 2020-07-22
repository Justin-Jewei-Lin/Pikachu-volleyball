#ifndef _STARTWIN_H_
#define _STARTWIN_H_

#include <QCheckBox>
#include <QMainWindow>

class StartWin : public QMainWindow {
	Q_OBJECT
public:
	StartWin(QWidget* parent = 0);
	~StartWin(void);
public slots:
	void setSound(bool);
	void setMusic(bool);
	void setDemo(bool);
signals:
	void changeWindow(bool, bool, bool);
private:
	void keyPressEvent(QKeyEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	QCheckBox* soundCBox;
	QCheckBox* musicCBox;
	QCheckBox* demoCBox;
	bool sound = true;
	bool music = true;
	bool demo = false;
};
#endif