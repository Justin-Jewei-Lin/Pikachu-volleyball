#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
int main(int argc, char *argv[]){
	#ifdef QT_DEBUG
		qDebug() << "Running a debug build";
	#else
		qDebug() << "Running a release build";
	#endif
	QApplication a(argc, argv);
	MainWindow w;
	return a.exec();
}