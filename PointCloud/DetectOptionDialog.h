#ifndef DETECTOPTIONDIALOG_H
#define DETECTOPTIONDIALOG_H

#include <QDialog>
#include "ui_DetectOptionDialog.h"

class DetectOptionDialog : public QDialog {
	Q_OBJECT

private:
	Ui::DetectOptionDialog ui;

public:
	DetectOptionDialog(QWidget *parent = 0);
	~DetectOptionDialog();

	double getProbability();
	double getMinPoints();
	double getEpsilon();
	double getClusterEpsilon();
	double getNormalThreshold();

public slots:
	void onDefault();
	void onOK();
	void onCancel();
};

#endif // DETECTOPTIONDIALOG_H
