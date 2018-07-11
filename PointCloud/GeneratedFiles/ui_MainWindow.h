/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindowClass
{
public:
    QAction *actionOpen;
    QAction *actionExit;
    QAction *actionDetect;
    QAction *actionShowPoints;
    QAction *actionShowFaces;
    QAction *actionRenderingBasic;
    QAction *actionRenderingSSAO;
    QAction *actionRenderingHatching;
    QAction *actionSegment;
    QAction *actionShow1;
    QAction *actionShow2;
    QAction *actionShow3;
    QAction *actionShow4;
    QAction *actionShow5;
    QAction *actionShow6;
    QAction *actionShow7;
    QAction *actionShow8;
    QAction *actionShow9;
    QAction *actionShowAll;
    QAction *actionShow10;
    QAction *actionShow11;
    QAction *actionShow12;
    QAction *actionPrintDetectedFaces;
    QAction *actionDelete;
    QAction *actionUndo;
    QAction *actionUseRandomFaceColor;
    QAction *actionUseSameFaceColor;
    QAction *actionRegularization;
    QAction *actionSaveVG;
    QAction *actionDownSample;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuTool;
    QMenu *menuView;
    QMenu *menuRendering;
    QMenu *menuEdit;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName(QStringLiteral("MainWindowClass"));
        MainWindowClass->resize(781, 807);
        actionOpen = new QAction(MainWindowClass);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        QIcon icon;
        icon.addFile(QStringLiteral("Resources/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon);
        actionExit = new QAction(MainWindowClass);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionDetect = new QAction(MainWindowClass);
        actionDetect->setObjectName(QStringLiteral("actionDetect"));
        actionShowPoints = new QAction(MainWindowClass);
        actionShowPoints->setObjectName(QStringLiteral("actionShowPoints"));
        actionShowPoints->setCheckable(true);
        actionShowFaces = new QAction(MainWindowClass);
        actionShowFaces->setObjectName(QStringLiteral("actionShowFaces"));
        actionShowFaces->setCheckable(true);
        actionRenderingBasic = new QAction(MainWindowClass);
        actionRenderingBasic->setObjectName(QStringLiteral("actionRenderingBasic"));
        actionRenderingBasic->setCheckable(true);
        actionRenderingBasic->setChecked(true);
        actionRenderingSSAO = new QAction(MainWindowClass);
        actionRenderingSSAO->setObjectName(QStringLiteral("actionRenderingSSAO"));
        actionRenderingSSAO->setCheckable(true);
        actionRenderingHatching = new QAction(MainWindowClass);
        actionRenderingHatching->setObjectName(QStringLiteral("actionRenderingHatching"));
        actionRenderingHatching->setCheckable(true);
        actionSegment = new QAction(MainWindowClass);
        actionSegment->setObjectName(QStringLiteral("actionSegment"));
        actionShow1 = new QAction(MainWindowClass);
        actionShow1->setObjectName(QStringLiteral("actionShow1"));
        actionShow1->setCheckable(true);
        actionShow2 = new QAction(MainWindowClass);
        actionShow2->setObjectName(QStringLiteral("actionShow2"));
        actionShow2->setCheckable(true);
        actionShow3 = new QAction(MainWindowClass);
        actionShow3->setObjectName(QStringLiteral("actionShow3"));
        actionShow3->setCheckable(true);
        actionShow4 = new QAction(MainWindowClass);
        actionShow4->setObjectName(QStringLiteral("actionShow4"));
        actionShow4->setCheckable(true);
        actionShow5 = new QAction(MainWindowClass);
        actionShow5->setObjectName(QStringLiteral("actionShow5"));
        actionShow5->setCheckable(true);
        actionShow6 = new QAction(MainWindowClass);
        actionShow6->setObjectName(QStringLiteral("actionShow6"));
        actionShow6->setCheckable(true);
        actionShow7 = new QAction(MainWindowClass);
        actionShow7->setObjectName(QStringLiteral("actionShow7"));
        actionShow7->setCheckable(true);
        actionShow8 = new QAction(MainWindowClass);
        actionShow8->setObjectName(QStringLiteral("actionShow8"));
        actionShow8->setCheckable(true);
        actionShow9 = new QAction(MainWindowClass);
        actionShow9->setObjectName(QStringLiteral("actionShow9"));
        actionShow9->setCheckable(true);
        actionShowAll = new QAction(MainWindowClass);
        actionShowAll->setObjectName(QStringLiteral("actionShowAll"));
        actionShowAll->setCheckable(true);
        actionShow10 = new QAction(MainWindowClass);
        actionShow10->setObjectName(QStringLiteral("actionShow10"));
        actionShow10->setCheckable(true);
        actionShow11 = new QAction(MainWindowClass);
        actionShow11->setObjectName(QStringLiteral("actionShow11"));
        actionShow11->setCheckable(true);
        actionShow12 = new QAction(MainWindowClass);
        actionShow12->setObjectName(QStringLiteral("actionShow12"));
        actionShow12->setCheckable(true);
        actionPrintDetectedFaces = new QAction(MainWindowClass);
        actionPrintDetectedFaces->setObjectName(QStringLiteral("actionPrintDetectedFaces"));
        actionDelete = new QAction(MainWindowClass);
        actionDelete->setObjectName(QStringLiteral("actionDelete"));
        QIcon icon1;
        icon1.addFile(QStringLiteral("Resources/delete.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDelete->setIcon(icon1);
        actionUndo = new QAction(MainWindowClass);
        actionUndo->setObjectName(QStringLiteral("actionUndo"));
        QIcon icon2;
        icon2.addFile(QStringLiteral("Resources/undo.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionUndo->setIcon(icon2);
        actionUseRandomFaceColor = new QAction(MainWindowClass);
        actionUseRandomFaceColor->setObjectName(QStringLiteral("actionUseRandomFaceColor"));
        actionUseRandomFaceColor->setCheckable(true);
        actionUseSameFaceColor = new QAction(MainWindowClass);
        actionUseSameFaceColor->setObjectName(QStringLiteral("actionUseSameFaceColor"));
        actionUseSameFaceColor->setCheckable(true);
        actionRegularization = new QAction(MainWindowClass);
        actionRegularization->setObjectName(QStringLiteral("actionRegularization"));
        actionSaveVG = new QAction(MainWindowClass);
        actionSaveVG->setObjectName(QStringLiteral("actionSaveVG"));
        QIcon icon3;
        icon3.addFile(QStringLiteral("Resources/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSaveVG->setIcon(icon3);
        actionDownSample = new QAction(MainWindowClass);
        actionDownSample->setObjectName(QStringLiteral("actionDownSample"));
        centralWidget = new QWidget(MainWindowClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        MainWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindowClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 781, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuTool = new QMenu(menuBar);
        menuTool->setObjectName(QStringLiteral("menuTool"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QStringLiteral("menuView"));
        menuRendering = new QMenu(menuBar);
        menuRendering->setObjectName(QStringLiteral("menuRendering"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        MainWindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindowClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindowClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindowClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuTool->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuRendering->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSaveVG);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuTool->addAction(actionDetect);
        menuTool->addAction(actionSegment);
        menuTool->addSeparator();
        menuTool->addAction(actionDownSample);
        menuTool->addAction(actionPrintDetectedFaces);
        menuView->addAction(actionShowPoints);
        menuView->addAction(actionShowFaces);
        menuView->addSeparator();
        menuView->addAction(actionUseRandomFaceColor);
        menuView->addAction(actionUseSameFaceColor);
        menuRendering->addAction(actionRenderingBasic);
        menuRendering->addAction(actionRenderingSSAO);
        menuRendering->addAction(actionRenderingHatching);
        menuEdit->addAction(actionDelete);
        menuEdit->addAction(actionUndo);

        retranslateUi(MainWindowClass);

        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QApplication::translate("MainWindowClass", "Point Cloud", Q_NULLPTR));
        actionOpen->setText(QApplication::translate("MainWindowClass", "Open", Q_NULLPTR));
        actionOpen->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+O", Q_NULLPTR));
        actionExit->setText(QApplication::translate("MainWindowClass", "Exit", Q_NULLPTR));
        actionDetect->setText(QApplication::translate("MainWindowClass", "Detect", Q_NULLPTR));
        actionShowPoints->setText(QApplication::translate("MainWindowClass", "Show Points", Q_NULLPTR));
        actionShowFaces->setText(QApplication::translate("MainWindowClass", "Show Faces", Q_NULLPTR));
        actionRenderingBasic->setText(QApplication::translate("MainWindowClass", "Basic", Q_NULLPTR));
        actionRenderingSSAO->setText(QApplication::translate("MainWindowClass", "SSAO", Q_NULLPTR));
        actionRenderingHatching->setText(QApplication::translate("MainWindowClass", "Hatching", Q_NULLPTR));
        actionSegment->setText(QApplication::translate("MainWindowClass", "Segment", Q_NULLPTR));
        actionShow1->setText(QApplication::translate("MainWindowClass", "Show 1", Q_NULLPTR));
        actionShow2->setText(QApplication::translate("MainWindowClass", "Show 2", Q_NULLPTR));
        actionShow3->setText(QApplication::translate("MainWindowClass", "Show 3", Q_NULLPTR));
        actionShow4->setText(QApplication::translate("MainWindowClass", "Show 4", Q_NULLPTR));
        actionShow5->setText(QApplication::translate("MainWindowClass", "Show 5", Q_NULLPTR));
        actionShow6->setText(QApplication::translate("MainWindowClass", "Show 6", Q_NULLPTR));
        actionShow7->setText(QApplication::translate("MainWindowClass", "Show 7", Q_NULLPTR));
        actionShow8->setText(QApplication::translate("MainWindowClass", "Show 8", Q_NULLPTR));
        actionShow9->setText(QApplication::translate("MainWindowClass", "Show 9", Q_NULLPTR));
        actionShowAll->setText(QApplication::translate("MainWindowClass", "Show All", Q_NULLPTR));
        actionShow10->setText(QApplication::translate("MainWindowClass", "Show 10", Q_NULLPTR));
        actionShow11->setText(QApplication::translate("MainWindowClass", "Show 11", Q_NULLPTR));
        actionShow12->setText(QApplication::translate("MainWindowClass", "Show 12", Q_NULLPTR));
        actionPrintDetectedFaces->setText(QApplication::translate("MainWindowClass", "Print Detected Faces", Q_NULLPTR));
        actionDelete->setText(QApplication::translate("MainWindowClass", "Delete", Q_NULLPTR));
        actionDelete->setShortcut(QApplication::translate("MainWindowClass", "Del", Q_NULLPTR));
        actionUndo->setText(QApplication::translate("MainWindowClass", "Undo", Q_NULLPTR));
        actionUndo->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+Z", Q_NULLPTR));
        actionUseRandomFaceColor->setText(QApplication::translate("MainWindowClass", "Use Random Color for Faces", Q_NULLPTR));
        actionUseSameFaceColor->setText(QApplication::translate("MainWindowClass", "Use Same Color for Faces", Q_NULLPTR));
        actionRegularization->setText(QApplication::translate("MainWindowClass", "Regularization", Q_NULLPTR));
        actionSaveVG->setText(QApplication::translate("MainWindowClass", "Save VG", Q_NULLPTR));
        actionSaveVG->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+S", Q_NULLPTR));
        actionDownSample->setText(QApplication::translate("MainWindowClass", "Downsample (50%)", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("MainWindowClass", "File", Q_NULLPTR));
        menuTool->setTitle(QApplication::translate("MainWindowClass", "Tool", Q_NULLPTR));
        menuView->setTitle(QApplication::translate("MainWindowClass", "View", Q_NULLPTR));
        menuRendering->setTitle(QApplication::translate("MainWindowClass", "Rendering", Q_NULLPTR));
        menuEdit->setTitle(QApplication::translate("MainWindowClass", "Edit", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
