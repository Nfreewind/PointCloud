/********************************************************************************
** Form generated from reading UI file 'DetectOptionDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DETECTOPTIONDIALOG_H
#define UI_DETECTOPTIONDIALOG_H

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

class Ui_DetectOptionDialog
{
public:
    QLabel *label;
    QLineEdit *lineEditProbability;
    QLabel *label_2;
    QLineEdit *lineEditMinPoints;
    QLabel *label_3;
    QLineEdit *lineEditEpsilon;
    QLabel *label_4;
    QLineEdit *lineEditClusterEpsilon;
    QLabel *label_5;
    QLineEdit *lineEditNormalThreshold;
    QPushButton *pushButtonOK;
    QPushButton *pushButtonCancel;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QPushButton *pushButtonDefault;

    void setupUi(QDialog *DetectOptionDialog)
    {
        if (DetectOptionDialog->objectName().isEmpty())
            DetectOptionDialog->setObjectName(QStringLiteral("DetectOptionDialog"));
        DetectOptionDialog->resize(321, 202);
        label = new QLabel(DetectOptionDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 50, 71, 16));
        lineEditProbability = new QLineEdit(DetectOptionDialog);
        lineEditProbability->setObjectName(QStringLiteral("lineEditProbability"));
        lineEditProbability->setGeometry(QRect(140, 50, 61, 20));
        label_2 = new QLabel(DetectOptionDialog);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 70, 71, 16));
        lineEditMinPoints = new QLineEdit(DetectOptionDialog);
        lineEditMinPoints->setObjectName(QStringLiteral("lineEditMinPoints"));
        lineEditMinPoints->setGeometry(QRect(140, 70, 61, 20));
        label_3 = new QLabel(DetectOptionDialog);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(20, 90, 71, 16));
        lineEditEpsilon = new QLineEdit(DetectOptionDialog);
        lineEditEpsilon->setObjectName(QStringLiteral("lineEditEpsilon"));
        lineEditEpsilon->setGeometry(QRect(140, 90, 61, 20));
        label_4 = new QLabel(DetectOptionDialog);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(20, 110, 91, 16));
        lineEditClusterEpsilon = new QLineEdit(DetectOptionDialog);
        lineEditClusterEpsilon->setObjectName(QStringLiteral("lineEditClusterEpsilon"));
        lineEditClusterEpsilon->setGeometry(QRect(140, 110, 61, 20));
        label_5 = new QLabel(DetectOptionDialog);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(20, 130, 91, 16));
        lineEditNormalThreshold = new QLineEdit(DetectOptionDialog);
        lineEditNormalThreshold->setObjectName(QStringLiteral("lineEditNormalThreshold"));
        lineEditNormalThreshold->setGeometry(QRect(140, 130, 61, 20));
        pushButtonOK = new QPushButton(DetectOptionDialog);
        pushButtonOK->setObjectName(QStringLiteral("pushButtonOK"));
        pushButtonOK->setGeometry(QRect(100, 160, 91, 31));
        pushButtonCancel = new QPushButton(DetectOptionDialog);
        pushButtonCancel->setObjectName(QStringLiteral("pushButtonCancel"));
        pushButtonCancel->setGeometry(QRect(210, 160, 91, 31));
        label_7 = new QLabel(DetectOptionDialog);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(210, 70, 101, 16));
        label_8 = new QLabel(DetectOptionDialog);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(210, 90, 101, 16));
        label_9 = new QLabel(DetectOptionDialog);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(210, 110, 101, 16));
        pushButtonDefault = new QPushButton(DetectOptionDialog);
        pushButtonDefault->setObjectName(QStringLiteral("pushButtonDefault"));
        pushButtonDefault->setGeometry(QRect(20, 10, 101, 31));

        retranslateUi(DetectOptionDialog);

        QMetaObject::connectSlotsByName(DetectOptionDialog);
    } // setupUi

    void retranslateUi(QDialog *DetectOptionDialog)
    {
        DetectOptionDialog->setWindowTitle(QApplication::translate("DetectOptionDialog", "DetectOptionDialog", 0));
        label->setText(QApplication::translate("DetectOptionDialog", "Probability:", 0));
        label_2->setText(QApplication::translate("DetectOptionDialog", "Min points:", 0));
        label_3->setText(QApplication::translate("DetectOptionDialog", "Epsilon:", 0));
        label_4->setText(QApplication::translate("DetectOptionDialog", "Cluster epsilon:", 0));
        label_5->setText(QApplication::translate("DetectOptionDialog", "Normal threshold:", 0));
        pushButtonOK->setText(QApplication::translate("DetectOptionDialog", "OK", 0));
        pushButtonCancel->setText(QApplication::translate("DetectOptionDialog", "Cancel", 0));
        label_7->setText(QApplication::translate("DetectOptionDialog", "% of total points", 0));
        label_8->setText(QApplication::translate("DetectOptionDialog", "% of bbox diagonal", 0));
        label_9->setText(QApplication::translate("DetectOptionDialog", "% of bbox diagonal", 0));
        pushButtonDefault->setText(QApplication::translate("DetectOptionDialog", "Default Values", 0));
    } // retranslateUi

};

namespace Ui {
    class DetectOptionDialog: public Ui_DetectOptionDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DETECTOPTIONDIALOG_H
