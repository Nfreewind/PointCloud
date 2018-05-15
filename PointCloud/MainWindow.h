#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "GLWidget3D.h"

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	Ui::MainWindowClass ui;
	GLWidget3D* glWidget;

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

public slots:
	void onOpen();
	void onDetect();
	void onSegment();
	void onPrintDetectedFaces();
	void onShowChanged();
	void onShowAll();
	void onShow1();
	void onShow2();
	void onShow3();
	void onShow4();
	void onShow5();
	void onShow6();
	void onShow7();
	void onShow8();
	void onShow9();
	void onShow10();
	void onShow11();
	void onShow12();
	void onRenderingModeChanged();
};

#endif // MAINWINDOW_H
