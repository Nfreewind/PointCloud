#include "DetectOptionDialog.h"

DetectOptionDialog::DetectOptionDialog(QWidget *parent) : QDialog(parent) {
	ui.setupUi(this);

	ui.lineEditProbability->setText("0.05");
	ui.lineEditMinPoints->setText("1");
	ui.lineEditEpsilon->setText("1");
	ui.lineEditClusterEpsilon->setText("1");
	ui.lineEditNormalThreshold->setText("0.9");

	connect(ui.pushButtonDefault, SIGNAL(clicked()), this, SLOT(onDefault()));
	connect(ui.pushButtonOK, SIGNAL(clicked()), this, SLOT(onOK()));
	connect(ui.pushButtonCancel, SIGNAL(clicked()), this, SLOT(onCancel()));
}

DetectOptionDialog::~DetectOptionDialog() {
}

double DetectOptionDialog::getProbability() {
	return ui.lineEditProbability->text().toDouble();
}

double DetectOptionDialog::getMinPoints() {
	return ui.lineEditMinPoints->text().toInt();
}

double DetectOptionDialog::getEpsilon() {
	return ui.lineEditEpsilon->text().toDouble();
}

double DetectOptionDialog::getClusterEpsilon() {
	return ui.lineEditClusterEpsilon->text().toDouble();
}

double DetectOptionDialog::getNormalThreshold() {
	return ui.lineEditNormalThreshold->text().toDouble();
}

void DetectOptionDialog::onDefault() {
	ui.lineEditProbability->setText("0.05");
	ui.lineEditMinPoints->setText("1");
	ui.lineEditEpsilon->setText("1");
	ui.lineEditClusterEpsilon->setText("1");
	ui.lineEditNormalThreshold->setText("0.9");
}

void DetectOptionDialog::onOK() {
	accept();
}

void DetectOptionDialog::onCancel() {
	reject();
}
