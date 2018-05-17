/********************************************************************************
** Form generated from reading UI file 'SegmentOptionDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEGMENTOPTIONDIALOG_H
#define UI_SEGMENTOPTIONDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_SegmentOptionDialog
{
public:
    QLabel *label_2;
    QLineEdit *lineEditMinSupportingPointsRatio;
    QPushButton *pushButtonOK;
    QLabel *label_7;
    QLabel *label;
    QLineEdit *lineEditDilationScale;
    QPushButton *pushButtonCancel;

    void setupUi(QDialog *SegmentOptionDialog)
    {
        if (SegmentOptionDialog->objectName().isEmpty())
            SegmentOptionDialog->setObjectName(QStringLiteral("SegmentOptionDialog"));
        SegmentOptionDialog->resize(291, 105);
        label_2 = new QLabel(SegmentOptionDialog);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 30, 111, 16));
        lineEditMinSupportingPointsRatio = new QLineEdit(SegmentOptionDialog);
        lineEditMinSupportingPointsRatio->setObjectName(QStringLiteral("lineEditMinSupportingPointsRatio"));
        lineEditMinSupportingPointsRatio->setGeometry(QRect(140, 30, 61, 20));
        pushButtonOK = new QPushButton(SegmentOptionDialog);
        pushButtonOK->setObjectName(QStringLiteral("pushButtonOK"));
        pushButtonOK->setGeometry(QRect(80, 60, 91, 31));
        label_7 = new QLabel(SegmentOptionDialog);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(210, 30, 101, 16));
        label = new QLabel(SegmentOptionDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 10, 71, 16));
        lineEditDilationScale = new QLineEdit(SegmentOptionDialog);
        lineEditDilationScale->setObjectName(QStringLiteral("lineEditDilationScale"));
        lineEditDilationScale->setGeometry(QRect(140, 10, 61, 20));
        pushButtonCancel = new QPushButton(SegmentOptionDialog);
        pushButtonCancel->setObjectName(QStringLiteral("pushButtonCancel"));
        pushButtonCancel->setGeometry(QRect(190, 60, 91, 31));
        QWidget::setTabOrder(lineEditDilationScale, lineEditMinSupportingPointsRatio);
        QWidget::setTabOrder(lineEditMinSupportingPointsRatio, pushButtonOK);
        QWidget::setTabOrder(pushButtonOK, pushButtonCancel);

        retranslateUi(SegmentOptionDialog);

        QMetaObject::connectSlotsByName(SegmentOptionDialog);
    } // setupUi

    void retranslateUi(QDialog *SegmentOptionDialog)
    {
        SegmentOptionDialog->setWindowTitle(QApplication::translate("SegmentOptionDialog", "SegmentOptionDialog", 0));
        label_2->setText(QApplication::translate("SegmentOptionDialog", "Min supporting ponts:", 0));
        pushButtonOK->setText(QApplication::translate("SegmentOptionDialog", "OK", 0));
        label_7->setText(QApplication::translate("SegmentOptionDialog", "(ratio to area)", 0));
        label->setText(QApplication::translate("SegmentOptionDialog", "Dilation scale:", 0));
        pushButtonCancel->setText(QApplication::translate("SegmentOptionDialog", "Cancel", 0));
    } // retranslateUi

};

namespace Ui {
    class SegmentOptionDialog: public Ui_SegmentOptionDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEGMENTOPTIONDIALOG_H
