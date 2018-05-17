#ifndef SEGMENTOPTIONDIALOG_H
#define SEGMENTOPTIONDIALOG_H

#include <QDialog>
#include "ui_SegmentOptionDialog.h"

class SegmentOptionDialog : public QDialog {
	Q_OBJECT

private:
	Ui::SegmentOptionDialog ui;

public:
	SegmentOptionDialog(QWidget *parent = 0);
	~SegmentOptionDialog();

	double getDilationScale();
	double getMinSupportingPointsRatio();

public slots:
	void onOK();
	void onCancel();
};

#endif // SEGMENTOPTIONDIALOG_H
