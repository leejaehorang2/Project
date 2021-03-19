/********************************************************************************
** Form generated from reading UI file 'main_window.ui'
**
** Created by: Qt User Interface Compiler version 5.11.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAIN_WINDOW_H
#define UI_MAIN_WINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QWidget *chartWidget;
    QLabel *categoryListLabel;
    QTableWidget *categoryListTable;
    QLabel *logLabel;
    QListWidget *logList;
    QLabel *settingLabel;
    QTableWidget *settingTable;
    QLabel *resultLabel;
    QLabel *recognitionResultLabel;
    QLabel *statusLabel;
    QListWidget *statusList;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1480, 783);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        chartWidget = new QWidget(centralWidget);
        chartWidget->setObjectName(QStringLiteral("chartWidget"));
        chartWidget->setGeometry(QRect(290, 140, 311, 211));
        chartWidget->setContextMenuPolicy(Qt::DefaultContextMenu);
        categoryListLabel = new QLabel(centralWidget);
        categoryListLabel->setObjectName(QStringLiteral("categoryListLabel"));
        categoryListLabel->setGeometry(QRect(20, 120, 151, 41));
        categoryListTable = new QTableWidget(centralWidget);
        categoryListTable->setObjectName(QStringLiteral("categoryListTable"));
        categoryListTable->setGeometry(QRect(10, 190, 271, 311));
        logLabel = new QLabel(centralWidget);
        logLabel->setObjectName(QStringLiteral("logLabel"));
        logLabel->setGeometry(QRect(20, 510, 201, 31));
        logList = new QListWidget(centralWidget);
        logList->setObjectName(QStringLiteral("logList"));
        logList->setGeometry(QRect(10, 540, 256, 192));
        settingLabel = new QLabel(centralWidget);
        settingLabel->setObjectName(QStringLiteral("settingLabel"));
        settingLabel->setGeometry(QRect(1220, 80, 151, 41));
        settingTable = new QTableWidget(centralWidget);
        settingTable->setObjectName(QStringLiteral("settingTable"));
        settingTable->setGeometry(QRect(1230, 350, 221, 281));
        resultLabel = new QLabel(centralWidget);
        resultLabel->setObjectName(QStringLiteral("resultLabel"));
        resultLabel->setGeometry(QRect(1240, 650, 151, 41));
        recognitionResultLabel = new QLabel(centralWidget);
        recognitionResultLabel->setObjectName(QStringLiteral("recognitionResultLabel"));
        recognitionResultLabel->setGeometry(QRect(1250, 690, 151, 41));
        recognitionResultLabel->setTextFormat(Qt::PlainText);
        recognitionResultLabel->setAlignment(Qt::AlignCenter);
        statusLabel = new QLabel(centralWidget);
        statusLabel->setObjectName(QStringLiteral("statusLabel"));
        statusLabel->setGeometry(QRect(1230, 240, 151, 41));
        statusList = new QListWidget(centralWidget);
        statusList->setObjectName(QStringLiteral("statusList"));
        statusList->setGeometry(QRect(1220, 280, 256, 192));
        MainWindow->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        categoryListLabel->setText(QApplication::translate("MainWindow", "Category List", nullptr));
        logLabel->setText(QApplication::translate("MainWindow", "Log", nullptr));
        settingLabel->setText(QApplication::translate("MainWindow", "Setting", nullptr));
        resultLabel->setText(QApplication::translate("MainWindow", "Result", nullptr));
        recognitionResultLabel->setText(QApplication::translate("MainWindow", "RESULT : 0", nullptr));
        statusLabel->setText(QApplication::translate("MainWindow", "Status", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAIN_WINDOW_H
