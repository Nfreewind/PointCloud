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
	void onSaveVG();
	void onDelete();
	void onUndo();
	void onDetect();
	void onSegment();
	void onDownSample();
	void onPrintDetectedFaces();
	void onShowChanged();
	void onUseRandomFaceColor();
	void onUseSameFaceColor();
	void onRenderingModeChanged();
};

#endif // MAINWINDOW_H
