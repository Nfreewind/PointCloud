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
	connect(ui.actionDetect, SIGNAL(triggered()), this, SLOT(onDetect()));
	connect(ui.actionSegment, SIGNAL(triggered()), this, SLOT(onSegment()));
	connect(ui.actionPrintDetectedFaces, SIGNAL(triggered()), this, SLOT(onPrintDetectedFaces()));
	connect(ui.actionShowPoints, SIGNAL(triggered()), this, SLOT(onShowChanged()));
	connect(ui.actionShowFaces, SIGNAL(triggered()), this, SLOT(onShowChanged()));
	connect(ui.actionShowAll, SIGNAL(triggered()), this, SLOT(onShowAll()));
	connect(ui.actionShow1, SIGNAL(triggered()), this, SLOT(onShow1()));
	connect(ui.actionShow2, SIGNAL(triggered()), this, SLOT(onShow2()));
	connect(ui.actionShow3, SIGNAL(triggered()), this, SLOT(onShow3()));
	connect(ui.actionShow4, SIGNAL(triggered()), this, SLOT(onShow4()));
	connect(ui.actionShow5, SIGNAL(triggered()), this, SLOT(onShow5()));
	connect(ui.actionShow6, SIGNAL(triggered()), this, SLOT(onShow6()));
	connect(ui.actionShow7, SIGNAL(triggered()), this, SLOT(onShow7()));
	connect(ui.actionShow8, SIGNAL(triggered()), this, SLOT(onShow8()));
	connect(ui.actionShow9, SIGNAL(triggered()), this, SLOT(onShow9()));
	connect(ui.actionShow10, SIGNAL(triggered()), this, SLOT(onShow10()));
	connect(ui.actionShow11, SIGNAL(triggered()), this, SLOT(onShow11()));
	connect(ui.actionShow12, SIGNAL(triggered()), this, SLOT(onShow12()));
	connect(ui.actionRenderingBasic, SIGNAL(triggered()), this, SLOT(onRenderingModeChanged()));
	connect(ui.actionRenderingSSAO, SIGNAL(triggered()), this, SLOT(onRenderingModeChanged()));
	connect(ui.actionRenderingHatching, SIGNAL(triggered()), this, SLOT(onRenderingModeChanged()));

	// create tool bar for file menu
	ui.mainToolBar->addAction(ui.actionOpen);

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

void MainWindow::onShowAll() {
	glWidget->showFace(-1);
	glWidget->update();
}

void MainWindow::onShow1() {
	glWidget->showFace(1);
	glWidget->update();
}

void MainWindow::onShow2() {
	glWidget->showFace(2);
	glWidget->update();
}

void MainWindow::onShow3() {
	glWidget->showFace(3);
	glWidget->update();
}

void MainWindow::onShow4() {
	glWidget->showFace(4);
	glWidget->update();
}

void MainWindow::onShow5() {
	glWidget->showFace(5);
	glWidget->update();
}

void MainWindow::onShow6() {
	glWidget->showFace(6);
	glWidget->update();
}

void MainWindow::onShow7() {
	glWidget->showFace(7);
	glWidget->update();
}

void MainWindow::onShow8() {
	glWidget->showFace(8);
	glWidget->update();
}

void MainWindow::onShow9() {
	glWidget->showFace(9);
	glWidget->update();
}

void MainWindow::onShow10() {
	glWidget->showFace(10);
	glWidget->update();
}

void MainWindow::onShow11() {
	glWidget->showFace(11);
	glWidget->update();
}

void MainWindow::onShow12() {
	glWidget->showFace(12);
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