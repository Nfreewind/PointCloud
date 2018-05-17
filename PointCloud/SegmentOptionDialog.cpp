#include "SegmentOptionDialog.h"

SegmentOptionDialog::SegmentOptionDialog(QWidget *parent) : QDialog(parent) {
	ui.setupUi(this);

	ui.lineEditDilationScale->setText("1.3");
	ui.lineEditMinSupportingPointsRatio->setText("3.0");

	connect(ui.pushButtonOK, SIGNAL(clicked()), this, SLOT(onOK()));
	connect(ui.pushButtonCancel, SIGNAL(clicked()), this, SLOT(onCancel()));
}

SegmentOptionDialog::~SegmentOptionDialog() {
}

double SegmentOptionDialog::getDilationScale() {
	return ui.lineEditDilationScale->text().toDouble();
}

double SegmentOptionDialog::getMinSupportingPointsRatio() {
	return ui.lineEditMinSupportingPointsRatio->text().toDouble();
}

void SegmentOptionDialog::onOK() {
	accept();
}

void SegmentOptionDialog::onCancel() {
	reject();
}