#include "MainWindow.h"
#include <QFileDialog>
#include "DetectOptionDialog.h"
#include "SegmentOptionDialog.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	ui.setupUi(this);

	// group for face color
	QActionGroup* groupFaceColor = new QActionGroup(this);
	groupFaceColor->addAction(ui.actionUseRandomFaceColor);
	groupFaceColor->addAction(ui.actionUseSameFaceColor);

	ui.actionUseRandomFaceColor->setChecked(true);

	// group for rendering modes
	QActionGroup* groupRendering = new QActionGroup(this);
	groupRendering->addAction(ui.actionRenderingBasic);
	groupRendering->addAction(ui.actionRenderingSSAO);
	groupRendering->addAction(ui.actionRenderingHatching);

	ui.actionShowPoints->setChecked(true);

	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(onOpen()));
	connect(ui.actionSaveVG, SIGNAL(triggered()), this, SLOT(onSaveVG()));
	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui.actionDelete, SIGNAL(triggered()), this, SLOT(onDelete()));
	connect(ui.actionUndo, SIGNAL(triggered()), this, SLOT(onUndo()));
	connect(ui.actionDetect, SIGNAL(triggered()), this, SLOT(onDetect()));
	connect(ui.actionSegment, SIGNAL(triggered()), this, SLOT(onSegment()));
	connect(ui.actionDownSample, SIGNAL(triggered()), this, SLOT(onDownSample()));
	connect(ui.actionPrintDetectedFaces, SIGNAL(triggered()), this, SLOT(onPrintDetectedFaces()));
	connect(ui.actionShowPoints, SIGNAL(triggered()), this, SLOT(onShowChanged()));
	connect(ui.actionShowFaces, SIGNAL(triggered()), this, SLOT(onShowChanged()));
	connect(ui.actionUseRandomFaceColor, SIGNAL(triggered()), this, SLOT(onUseRandomFaceColor()));
	connect(ui.actionUseSameFaceColor, SIGNAL(triggered()), this, SLOT(onUseSameFaceColor()));
	connect(ui.actionRenderingBasic, SIGNAL(triggered()), this, SLOT(onRenderingModeChanged()));
	connect(ui.actionRenderingSSAO, SIGNAL(triggered()), this, SLOT(onRenderingModeChanged()));
	connect(ui.actionRenderingHatching, SIGNAL(triggered()), this, SLOT(onRenderingModeChanged()));

	// create tool bar for file menu
	ui.mainToolBar->addAction(ui.actionOpen);
	ui.mainToolBar->addAction(ui.actionSaveVG);
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
	int num_points = glWidget->loadVoxelData(filename);
	statusBar()->showMessage(QLocale(QLocale::English).toString(num_points) +" points were loaded");
	glWidget->update();
}

void MainWindow::onSaveVG() {
	QString filename = QFileDialog::getSaveFileName(this, tr("Save VG data..."), "", tr("VG files (*.vg)"));
	if (filename.isEmpty()) return;

	glWidget->saveVG(filename);
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
	SegmentOptionDialog dlg;
	if (dlg.exec()) {
		glWidget->segment(dlg.getDilationScale(), dlg.getMinSupportingPointsRatio());
		glWidget->update();
	}
}

void MainWindow::onDownSample() {
	int num_points = glWidget->downSample(0.5);
	statusBar()->showMessage(QLocale(QLocale::English).toString(num_points) + " points were loaded");
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

void MainWindow::onUseRandomFaceColor() {
	glWidget->face_coloring = GLWidget3D::FACE_RANDOM_COLOR;
	glWidget->update3DGeometry();
	glWidget->update();
}

void MainWindow::onUseSameFaceColor() {
	glWidget->face_coloring = GLWidget3D::FACE_SAME_COLOR;
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