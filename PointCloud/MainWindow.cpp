#include "MainWindow.h"
#include <QFileDialog>
#include "DetectOptionDialog.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	ui.setupUi(this);

	// group for show modes
	QActionGroup* groupShow = new QActionGroup(this);
	groupShow->addAction(ui.actionShowAll);
	groupShow->addAction(ui.actionShow1);
	groupShow->addAction(ui.actionShow2);
	groupShow->addAction(ui.actionShow3);
	groupShow->addAction(ui.actionShow4);
	groupShow->addAction(ui.actionShow5);
	groupShow->addAction(ui.actionShow6);
	groupShow->addAction(ui.actionShow7);
	groupShow->addAction(ui.actionShow8);
	groupShow->addAction(ui.actionShow9);
	groupShow->addAction(ui.actionShow10);
	groupShow->addAction(ui.actionShow11);
	groupShow->addAction(ui.actionShow12);

	// group for rendering modes
	QActionGroup* groupRendering = new QActionGroup(this);
	groupRendering->addAction(ui.actionRenderingBasic);
	groupRendering->addAction(ui.actionRenderingSSAO);
	groupRendering->addAction(ui.actionRenderingHatching);

	ui.actionShowPoints->setChecked(true);

	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(onOpen()));
	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui.actionDelete, SIGNAL(triggered()), this, SLOT(onDelete()));
	connect(ui.actionUndo, SIGNAL(triggered()), this, SLOT(onUndo()));
	connect(ui.actionDetect, SIGNAL(triggered()), this, SLOT(onDetect()));
	connect(ui.actionSegment, SIGNAL(triggered()), this, SLOT(onSegment()));
	connect(ui.actionPrintDetectedFaces, SIGNAL(triggered()), this, SLOT(onPrintDetectedFaces()));
	connect(ui.actionShowPoints, SIGNAL(triggered()), this, SLOT(onShowChanged()));
	connect(ui.actionShowFaces, SIGNAL(triggered()), this, SLOT(onShowChanged()));
	connect(ui.actionRenderingBasic, SIGNAL(triggered()), this, SLOT(onRenderingModeChanged()));
	connect(ui.actionRenderingSSAO, SIGNAL(triggered()), this, SLOT(onRenderingModeChanged()));
	connect(ui.actionRenderingHatching, SIGNAL(triggered()), this, SLOT(onRenderingModeChanged()));

	// create tool bar for file menu
	ui.mainToolBar->addAction(ui.actionOpen);
	ui.mainToolBar->addSeparator();

	// create tool bar for edit menu
	ui.mainToolBar->addAction(ui.actionDelete);
	ui.mainToolBar->addAction(ui.actionUndo);

	// setup the GL widget
	glWidget = new GLWidget3D(this);
	setCentralWidget(glWidget);
}

MainWindow::~MainWindow() {
}

void MainWindow::onOpen() {
	QString filename = QFileDialog::getOpenFileName(this, tr("Load voxel data..."), "", tr("Image files (*.png *.jpg *.bmp)"));
	if (filename.isEmpty()) return;

	setWindowTitle("Point Cloud - " + filename);
	glWidget->loadVoxelData(filename);
	glWidget->update();
}

void MainWindow::onDelete() {
	glWidget->deleteFace();
}

void MainWindow::onUndo() {
	glWidget->undo();
}

void MainWindow::onDetect() {
	DetectOptionDialog dlg;
	if (dlg.exec()) {
		glWidget->detect(dlg.getProbability(), dlg.getMinPoints(), dlg.getEpsilon(), dlg.getClusterEpsilon(), dlg.getNormalThreshold());
		glWidget->update();
	}
}

void MainWindow::onSegment() {
	glWidget->segment();
	glWidget->update();
}

void MainWindow::onPrintDetectedFaces() {
	glWidget->printDetectedFaces();
}

void MainWindow::onShowChanged() {
	glWidget->show_points = ui.actionShowPoints->isChecked();
	glWidget->show_faces = ui.actionShowFaces->isChecked();
	glWidget->update3DGeometry();
	glWidget->update();
}

void MainWindow::onRenderingModeChanged() {
	if (ui.actionRenderingBasic->isChecked()) {
		glWidget->renderManager.renderingMode = RenderManager::RENDERING_MODE_BASIC;
	}
	else if (ui.actionRenderingSSAO->isChecked()) {
		glWidget->renderManager.renderingMode = RenderManager::RENDERING_MODE_SSAO;
	}
	else if (ui.actionRenderingHatching->isChecked()) {
		glWidget->renderManager.renderingMode = RenderManager::RENDERING_MODE_HATCHING;
	}
	glWidget->update();
}