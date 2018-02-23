/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
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
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionSave_Contour;
    QAction *actionSave_Mask;
    QAction *actionExit;
    QAction *actionCost_Graph;
    QAction *actionMinimum_Path;
    QAction *actionSet_Seed;
    QAction *actionDelete_Latest_Seed;
    QAction *actionOpen_File;
    QAction *actionPixel_Node;
    QWidget *centralWidget;
    QLabel *imgviewer_label;
    QPushButton *zoomIn;
    QPushButton *zoomOut;
    QLabel *local_mousepos_label;
    QLabel *seedpos_label;
    QLabel *global_mousepos_label;
    QLabel *maskviewer_label;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QMenu *menuImage_Scissor;
    QMenu *menuTool;
    QMenu *menuAction;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(998, 811);
        actionSave_Contour = new QAction(MainWindow);
        actionSave_Contour->setObjectName(QStringLiteral("actionSave_Contour"));
        actionSave_Mask = new QAction(MainWindow);
        actionSave_Mask->setObjectName(QStringLiteral("actionSave_Mask"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionCost_Graph = new QAction(MainWindow);
        actionCost_Graph->setObjectName(QStringLiteral("actionCost_Graph"));
        actionMinimum_Path = new QAction(MainWindow);
        actionMinimum_Path->setObjectName(QStringLiteral("actionMinimum_Path"));
        actionSet_Seed = new QAction(MainWindow);
        actionSet_Seed->setObjectName(QStringLiteral("actionSet_Seed"));
        actionDelete_Latest_Seed = new QAction(MainWindow);
        actionDelete_Latest_Seed->setObjectName(QStringLiteral("actionDelete_Latest_Seed"));
        actionOpen_File = new QAction(MainWindow);
        actionOpen_File->setObjectName(QStringLiteral("actionOpen_File"));
        actionPixel_Node = new QAction(MainWindow);
        actionPixel_Node->setObjectName(QStringLiteral("actionPixel_Node"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        imgviewer_label = new QLabel(centralWidget);
        imgviewer_label->setObjectName(QStringLiteral("imgviewer_label"));
        imgviewer_label->setGeometry(QRect(30, 20, 441, 541));
        QFont font;
        font.setPointSize(24);
        font.setItalic(false);
        imgviewer_label->setFont(font);
        imgviewer_label->setTextFormat(Qt::AutoText);
        imgviewer_label->setAlignment(Qt::AlignCenter);
        zoomIn = new QPushButton(centralWidget);
        zoomIn->setObjectName(QStringLiteral("zoomIn"));
        zoomIn->setGeometry(QRect(700, 570, 151, 27));
        zoomOut = new QPushButton(centralWidget);
        zoomOut->setObjectName(QStringLiteral("zoomOut"));
        zoomOut->setGeometry(QRect(700, 610, 151, 27));
        local_mousepos_label = new QLabel(centralWidget);
        local_mousepos_label->setObjectName(QStringLiteral("local_mousepos_label"));
        local_mousepos_label->setGeometry(QRect(700, 680, 281, 20));
        seedpos_label = new QLabel(centralWidget);
        seedpos_label->setObjectName(QStringLiteral("seedpos_label"));
        seedpos_label->setGeometry(QRect(700, 710, 281, 20));
        global_mousepos_label = new QLabel(centralWidget);
        global_mousepos_label->setObjectName(QStringLiteral("global_mousepos_label"));
        global_mousepos_label->setGeometry(QRect(700, 650, 281, 20));
        maskviewer_label = new QLabel(centralWidget);
        maskviewer_label->setObjectName(QStringLiteral("maskviewer_label"));
        maskviewer_label->setGeometry(QRect(490, 80, 381, 431));
        maskviewer_label->setFont(font);
        maskviewer_label->setTextFormat(Qt::AutoText);
        maskviewer_label->setAlignment(Qt::AlignCenter);
        MainWindow->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 998, 27));
        menuImage_Scissor = new QMenu(menuBar);
        menuImage_Scissor->setObjectName(QStringLiteral("menuImage_Scissor"));
        menuTool = new QMenu(menuBar);
        menuTool->setObjectName(QStringLiteral("menuTool"));
        menuAction = new QMenu(menuBar);
        menuAction->setObjectName(QStringLiteral("menuAction"));
        MainWindow->setMenuBar(menuBar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        menuBar->addAction(menuImage_Scissor->menuAction());
        menuBar->addAction(menuTool->menuAction());
        menuBar->addAction(menuAction->menuAction());
        menuImage_Scissor->addSeparator();
        menuImage_Scissor->addSeparator();
        menuImage_Scissor->addAction(actionOpen_File);
        menuImage_Scissor->addAction(actionSave_Contour);
        menuImage_Scissor->addAction(actionSave_Mask);
        menuImage_Scissor->addSeparator();
        menuImage_Scissor->addAction(actionExit);
        menuTool->addAction(actionPixel_Node);
        menuTool->addAction(actionCost_Graph);
        menuTool->addAction(actionMinimum_Path);
        menuAction->addAction(actionSet_Seed);
        menuAction->addAction(actionDelete_Latest_Seed);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionSave_Contour->setText(QApplication::translate("MainWindow", "Save Contour", 0));
        actionSave_Contour->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0));
        actionSave_Mask->setText(QApplication::translate("MainWindow", "Save Mask", 0));
        actionSave_Mask->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+S", 0));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0));
        actionExit->setShortcut(QApplication::translate("MainWindow", "Esc", 0));
        actionCost_Graph->setText(QApplication::translate("MainWindow", "Cost Graph", 0));
        actionCost_Graph->setShortcut(QApplication::translate("MainWindow", "Ctrl+G", 0));
        actionMinimum_Path->setText(QApplication::translate("MainWindow", "Minimum Path", 0));
        actionMinimum_Path->setShortcut(QApplication::translate("MainWindow", "Ctrl+M", 0));
        actionSet_Seed->setText(QApplication::translate("MainWindow", "Set Seed", 0));
        actionSet_Seed->setShortcut(QApplication::translate("MainWindow", "Ctrl+Space", 0));
        actionDelete_Latest_Seed->setText(QApplication::translate("MainWindow", "Delete Latest Seed", 0));
        actionDelete_Latest_Seed->setShortcut(QApplication::translate("MainWindow", "Del", 0));
        actionOpen_File->setText(QApplication::translate("MainWindow", "Open File", 0));
        actionOpen_File->setShortcut(QApplication::translate("MainWindow", "Ctrl+F", 0));
        actionPixel_Node->setText(QApplication::translate("MainWindow", "Pixel Node", 0));
        actionPixel_Node->setShortcut(QApplication::translate("MainWindow", "Ctrl+P", 0));
        imgviewer_label->setText(QApplication::translate("MainWindow", "Image Viewer", 0));
        zoomIn->setText(QApplication::translate("MainWindow", "Zoom In(+25%)", 0));
        zoomOut->setText(QApplication::translate("MainWindow", "Zoom Out(-25%)", 0));
        local_mousepos_label->setText(QApplication::translate("MainWindow", "Local Mouse Position x: y:", 0));
        seedpos_label->setText(QApplication::translate("MainWindow", "Seed Position x: y:", 0));
        global_mousepos_label->setText(QApplication::translate("MainWindow", "Global Mouse Position x: y:", 0));
        maskviewer_label->setText(QApplication::translate("MainWindow", "Mask Viewer", 0));
        menuImage_Scissor->setTitle(QApplication::translate("MainWindow", "Image Scissor", 0));
        menuTool->setTitle(QApplication::translate("MainWindow", "Tool", 0));
        menuAction->setTitle(QApplication::translate("MainWindow", "Action", 0));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
