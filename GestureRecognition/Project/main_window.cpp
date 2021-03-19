#include "main_window.hpp"
#include "ui_main_window.h"

#include <QGridLayout>
#include <QLineSeries>
#include <QDesktopWidget>
#include <QToolButton>

#include <iostream>
#include <fstream>
#include <math.h>

#include "nm_helper.hpp"

using namespace std;

#define NEURON_DATA_PATH "../Data/NeuronData.knf"
#define CATEGORY_DATA_PATH "../Data/CategoryData.txt"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    helpDialog = nullptr;
    isMotionStartClicked = false;
}

MainWindow::~MainWindow()
{
    if(helpDialog){
        delete helpDialog;
        helpDialog = nullptr;
    }

    delete ui;
}

void MainWindow::initializeWidgets()
{
    statusBar()->hide();
    setWindowTitle("Gesture Recognition");

    QDesktopWidget desktopWidget;

    int desktopWidth = desktopWidget.width();
    int desktopHeight = desktopWidget.height() - 64;    //1016

    //define widgets size
    const int toolbarWidth = (int)(ceil(desktopWidth * 0.0916));       //128 + 24 + 24
    const int toolbarHeight = (int)ceil((desktopHeight * 0.1351));  //120
    const int toolbarSaveModelWidth = (int)(ceil(desktopWidth * 0.0786));       //128 + 23
    const int toolbarLoadModelWidth = (int)(ceil(desktopWidth * 0.0666));       //128
    const int toolbarResetWidth = (int)(ceil(desktopWidth * 0.0791));           //128 + 24

    desktopHeight -= toolbarHeight;

    const int categoryListLabelWidth = toolbarWidth * 2 + 2;
    const int categoryListLabelHeight = (int)(ceil(desktopHeight * 0.0683));  //60
    const int categoryListLabelStrideY = 0;

    const int categoryListTableWidth = categoryListLabelWidth;
    const int categoryListTableHeight = (int)(ceil(desktopHeight * 0.5922));    //520
    const int categoryListTableStrideY = categoryListLabelHeight;
    const int categoryListTableRowHeight = (int)ceil(desktopHeight * 0.0455); //40
    const int categoryListTableIDColumnWidth = (int)(ceil(desktopWidth * 0.0520) - 2); //100
    const int categoryListTableNameColumnWidth = categoryListTableWidth - categoryListTableIDColumnWidth - 2; //252

    const int logLabelWidth = categoryListLabelWidth;
    const int logLabelHeight = (int)(ceil(desktopHeight * 0.0683));  //60
    const int logLabelStrideY = categoryListTableStrideY + categoryListTableHeight;

    const int logListWidth = categoryListLabelWidth;
    const int logListHeight = (int)(ceil(desktopHeight * 0.2995));  //263
    const int logListStrideY = logLabelStrideY + logLabelHeight;

    int chartStrideX = categoryListLabelWidth;
    int chartStrideY = categoryListLabelStrideY;
    const int chartWidth = (int)(ceil(desktopWidth * 0.2958));  //568
    const int chartHeight = (int)(ceil(desktopHeight * 0.3405));    //299

    const int settingLabelWidth = (int)(ceil(desktopWidth * 0.2250)); //432
    const int settingLabelHeight = (int)(ceil(desktopHeight * 0.0683));  //60
    const int settingLabelStrideX = categoryListLabelWidth + (chartWidth * 2);
    const int settingLabelStrideY = 0;

    const int settingTableWidth = settingLabelWidth;
    const int settingTableHeight = (int)(ceil(desktopHeight * 0.1822));  //160
    const int settingTableStrideX = settingLabelStrideX;
    const int settingTableStrideY = settingLabelStrideY + settingLabelHeight;
    const int settingTableRowHeight = (int)ceil(desktopHeight * 0.0455); //40
    const int settingTableParameterWidth = (int)(ceil(desktopWidth * 0.1614)); //310
    const int settingTableHeaderWidth = settingTableWidth - settingTableParameterWidth;//123

    const int statusLabelWidth = settingLabelWidth;
    const int statusLabelHeight = (int)(ceil(desktopHeight * 0.0683));  //60
    const int statusLabelStrideX = settingLabelStrideX;
    const int statusLabelStrideY = settingTableStrideY + settingTableHeight;

    const int statusListWidth = settingLabelWidth;
    const int statusListHeight = (int)(ceil(desktopHeight * 0.2676));  //235
    const int statusListStrideX = settingLabelStrideX;
    const int statusListStrideY = statusLabelStrideY + statusLabelHeight;

    const int resultLabelWidth = settingLabelWidth;
    const int resultLabelHeight = (int)(ceil(desktopHeight * 0.0683));  //60
    const int resultLabelStrideX = settingLabelStrideX;
    const int resultLabelStrideY = statusListStrideY + statusListHeight;

    const int recognitionResultWidth = settingLabelWidth;
    const int recognitionResultHeight = (int)(ceil(desktopHeight * 0.3690));  //324
    const int recognitionResultStrideX = settingLabelStrideX;
    const int recognitionResultStrideY = resultLabelStrideY + resultLabelHeight;

    ui->mainToolBar->setIconSize(QSize(toolbarWidth,toolbarHeight));
    ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->mainToolBar->setMovable(false);

    iconImageList[ICON_IMAGE_NORMAL_DEVICE_CONNECT].load("../Icon/scan.svg");
    iconImageList[ICON_IMAGE_ACTIVE_DEVICE_CONNECT].load("../Icon/scan-active.svg");
    iconImageList[ICON_IMAGE_NORMAL_DEVICE_DISCONNECT].load("../Icon/disconnect.svg");
    iconImageList[ICON_IMAGE_ACTIVE_DEVICE_DISCONNECT].load("../Icon/disconnect-active.svg");
    iconImageList[ICON_IMAGE_NORMAL_START_MOTION].load("../Icon/motion-start.png");
    iconImageList[ICON_IMAGE_ACTIVE_START_MOTION].load("../Icon/motion-start-active.png");
    iconImageList[ICON_IMAGE_NORMAL_STOP_MOTION].load("../Icon/motion-stop.png");
    iconImageList[ICON_IMAGE_ACTIVE_STOP_MOTION].load("../Icon/motion-stop-active.png");
    iconImageList[ICON_IMAGE_NORMAL_LEARNING].load("../Icon/learning.svg");
    iconImageList[ICON_IMAGE_ACTIVE_LEARNING].load("../Icon/learning-active.svg");
    iconImageList[ICON_IMAGE_NORMAL_RECOGNITION].load("../Icon/recognition.svg");
    iconImageList[ICON_IMAGE_ACTIVE_RECOGNITION].load("../Icon/recognition-active.svg");
    iconImageList[ICON_IMAGE_NORMAL_SAVE].load("../Icon/save.svg");
    iconImageList[ICON_IMAGE_ACTIVE_SAVE].load("../Icon/save-active.svg");
    iconImageList[ICON_IMAGE_NORMAL_LOAD].load("../Icon/load.svg");
    iconImageList[ICON_IMAGE_ACTIVE_LOAD].load("../Icon/load-active.svg");
    iconImageList[ICON_IMAGE_NORMAL_RESET].load("../Icon/reset.svg");
    iconImageList[ICON_IMAGE_ACTIVE_RESET].load("../Icon/reset-active.svg");

    toolbarIconList[ACTION_DEVICE_CONNECT].addPixmap(iconImageList[ICON_IMAGE_NORMAL_DEVICE_CONNECT], QIcon::Normal, QIcon::On);
    toolbarIconList[ACTION_DEVICE_CONNECT].addPixmap(iconImageList[ICON_IMAGE_ACTIVE_DEVICE_CONNECT], QIcon::Active, QIcon::On);
    toolbarIconList[ACTION_DEVICE_DISCONNECT].addPixmap(iconImageList[ICON_IMAGE_NORMAL_DEVICE_DISCONNECT], QIcon::Normal, QIcon::On);
    toolbarIconList[ACTION_DEVICE_DISCONNECT].addPixmap(iconImageList[ICON_IMAGE_ACTIVE_DEVICE_DISCONNECT], QIcon::Active, QIcon::On);
    toolbarIconList[ACTION_START_MOTION].addPixmap(iconImageList[ICON_IMAGE_NORMAL_START_MOTION], QIcon::Normal, QIcon::On);
    toolbarIconList[ACTION_START_MOTION].addPixmap(iconImageList[ICON_IMAGE_ACTIVE_START_MOTION], QIcon::Active, QIcon::On);
    toolbarIconList[ACTION_STOP_MOTION].addPixmap(iconImageList[ICON_IMAGE_NORMAL_STOP_MOTION], QIcon::Normal, QIcon::On);
    toolbarIconList[ACTION_STOP_MOTION].addPixmap(iconImageList[ICON_IMAGE_ACTIVE_STOP_MOTION], QIcon::Active, QIcon::On);
    toolbarIconList[ACTION_LEARNING].addPixmap(iconImageList[ICON_IMAGE_NORMAL_LEARNING], QIcon::Normal, QIcon::On);
    toolbarIconList[ACTION_LEARNING].addPixmap(iconImageList[ICON_IMAGE_ACTIVE_LEARNING], QIcon::Active, QIcon::On);
    toolbarIconList[ACTION_RECOGNITION].addPixmap(iconImageList[ICON_IMAGE_NORMAL_RECOGNITION], QIcon::Normal, QIcon::On);
    toolbarIconList[ACTION_RECOGNITION].addPixmap(iconImageList[ICON_IMAGE_ACTIVE_RECOGNITION], QIcon::Active, QIcon::On);
    toolbarIconList[ACTION_SAVE].addPixmap(iconImageList[ICON_IMAGE_NORMAL_SAVE], QIcon::Normal, QIcon::On);
    toolbarIconList[ACTION_SAVE].addPixmap(iconImageList[ICON_IMAGE_ACTIVE_SAVE], QIcon::Active, QIcon::On);
    toolbarIconList[ACTION_LOAD].addPixmap(iconImageList[ICON_IMAGE_NORMAL_LOAD], QIcon::Normal, QIcon::On);
    toolbarIconList[ACTION_LOAD].addPixmap(iconImageList[ICON_IMAGE_ACTIVE_LOAD], QIcon::Active, QIcon::On);
    toolbarIconList[ACTION_RESET].addPixmap(iconImageList[ICON_IMAGE_NORMAL_RESET], QIcon::Normal, QIcon::On);
    toolbarIconList[ACTION_RESET].addPixmap(iconImageList[ICON_IMAGE_ACTIVE_RESET], QIcon::Active, QIcon::On);

    motionImageList[MOTION_IMAGE_01].load("../Icon/motion-01.png");
    motionImageList[MOTION_IMAGE_02].load("../Icon/motion-02.png");
    motionImageList[MOTION_IMAGE_03].load("../Icon/motion-03.png");
    motionImageList[MOTION_IMAGE_04].load("../Icon/motion-04.png");
    motionImageList[MOTION_IMAGE_05].load("../Icon/motion-05.png");
    motionImageList[MOTION_IMAGE_06].load("../Icon/motion-06.png");
    motionImageList[MOTION_IMAGE_07].load("../Icon/motion-07.png");
    motionImageList[MOTION_IMAGE_08].load("../Icon/motion-08.png");
    motionImageList[MOTION_IMAGE_09].load("../Icon/motion-09.png");
    motionImageList[MOTION_IMAGE_10].load("../Icon/motion-10.png");
    motionImageList[MOTION_IMAGE_11].load("../Icon/motion-11.png");
    motionImageList[MOTION_IMAGE_12].load("../Icon/motion-12.png");

    QString fontName = "Helvetica";

    QFont toolbarFont(fontName,12);
    QFont labelFont(fontName,18);
    QFont categoryListHeaderFont(fontName,16);
    categoryListHeaderFont.setBold(true);

    for(int i = 0; i < TOOLBAR_COUNT; ++i){

        toolButtons[i]= new QToolButton;

        toolButtons[i]->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        toolButtons[i]->setFont(toolbarFont);
        toolButtons[i]->setAutoFillBackground(true);
        toolButtons[i]->setStyleSheet("QToolButton::menu-indicator{image:none;}"
                            "QToolButton::hover{background-color:rgba(41,44,50,255);color:rgba(91,154,248,255);border:none;}"
                            "QToolButton::!hover{background-color:rgba(41,44,50,255);color:rgba(255,255,255,153);border:none;}");
        toolButtons[i]->setFixedWidth(toolbarWidth);
        toolButtons[i]->setFixedHeight(toolbarHeight);


        ui->mainToolBar->addWidget(toolButtons[i]);

        switch(i){

            case TOOLBAR_DEVICE:
            {
                toolButtons[i]->setIcon(toolbarIconList[ACTION_DEVICE_CONNECT]);

                actions[ACTION_DEVICE_CONNECT] = new QAction;
                actions[ACTION_DEVICE_DISCONNECT] = new QAction;
//                toolButtons[i]->addAction(actions[i]);
                QList<QAction*> actionList = {actions[ACTION_DEVICE_CONNECT], actions[ACTION_DEVICE_DISCONNECT]};
                toolButtons[i]->addActions(actionList);
                toolButtons[i]->setText("Device Scan");
                ui->mainToolBar->addSeparator();
            }
                break;

            case TOOLBAR_MOTION:
            {
                toolButtons[i]->setIcon(toolbarIconList[ACTION_START_MOTION]);

                actions[ACTION_START_MOTION] = new QAction;
                actions[ACTION_STOP_MOTION] = new QAction;
//                toolButtons[i]->addAction(actions[i]);
                QList<QAction*> actionList = {actions[ACTION_START_MOTION], actions[ACTION_STOP_MOTION]};
                toolButtons[i]->addActions(actionList);
                toolButtons[i]->setText("Motion Start");

                ui->mainToolBar->addSeparator();
            }
                break;

            case TOOLBAR_LEARNING:
            {
                toolButtons[i]->setIcon(toolbarIconList[ACTION_LEARNING]);
                actions[ACTION_LEARNING] = new QAction;
                toolButtons[i]->addAction(actions[ACTION_LEARNING]);
                toolButtons[i]->setText("Learning");
                toolButtons[i]->setStyleSheet("QToolButton::menu-indicator{image:none;}"
                                    "QToolButton::hover{background-color:rgba(20,22,26,255);color:rgba(91,154,248,255);border:none;}"
                                    "QToolButton::!hover{background-color:rgba(20,22,26,255);color:rgba(255,255,255,153);border:none;}");
                ui->mainToolBar->addSeparator();
            }
                break;

            case TOOLBAR_RECOGNITION:
            {
                toolButtons[i]->setIcon(toolbarIconList[ACTION_RECOGNITION]);
                actions[ACTION_RECOGNITION] = new QAction;
                toolButtons[i]->addAction(actions[ACTION_RECOGNITION]);
                toolButtons[i]->setText("Recognition");
                toolButtons[i]->setStyleSheet("QToolButton::menu-indicator{image:none;}"
                                    "QToolButton::hover{background-color:rgba(20,22,26,255);color:rgba(91,154,248,255);border:none;}"
                                    "QToolButton::!hover{background-color:rgba(20,22,26,255);color:rgba(255,255,255,153);border:none;}");

                ui->mainToolBar->addSeparator();

                QWidget* emptyWidget = new QWidget();
                emptyWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
//                emptyWidget->setFixedWidth(785);
                emptyWidget->setStyleSheet("background-color:rgba(41,44,50,255);");

                ui->mainToolBar->addWidget(emptyWidget);
                ui->mainToolBar->addSeparator();

            }
                break;

            case TOOLBAR_SAVE:
            {
                toolButtons[i]->setIcon(toolbarIconList[ACTION_SAVE]);

                actions[ACTION_SAVE] = new QAction;
                toolButtons[i]->addAction(actions[ACTION_SAVE]);
                toolButtons[i]->setText("Save Model");
                toolButtons[i]->setFixedWidth(toolbarSaveModelWidth);
            }
                break;

            case TOOLBAR_LOAD:
            {
                toolButtons[i]->setIcon(toolbarIconList[ACTION_LOAD]);
                actions[ACTION_LOAD] = new QAction;
                toolButtons[i]->addAction(actions[ACTION_LOAD]);
                toolButtons[i]->setText("Load Model");
                toolButtons[i]->setFixedWidth(toolbarLoadModelWidth);
            }
                break;

            case TOOLBAR_RESET:
            {
                toolButtons[i]->setIcon(toolbarIconList[ACTION_RESET]);
                actions[ACTION_RESET] = new QAction;
                toolButtons[i]->addAction(actions[ACTION_RESET]);
                toolButtons[i]->setText("Reset");
                toolButtons[i]->setFixedWidth(toolbarResetWidth);
            }
                break;

            default:
                break;
        }
    }
    ui->mainToolBar->setStyleSheet("QToolBar{border:none; background-color:rgb(41,44,50);color : rgba(255,255,255,153);}"
                           "QToolBar::separator{background-color:rgba(151,151,151,255);width:1px;}"
                           "QToolButton{padding-top:16px;}");
;
    ui->categoryListLabel->move(0, categoryListLabelStrideY);
    ui->categoryListLabel->resize(categoryListLabelWidth, categoryListLabelHeight);
    ui->categoryListLabel->setFont(labelFont);
    ui->categoryListLabel->setStyleSheet("padding-left:20px; border:1px solid #979797; background-color: rgba(65,85,113,255); color:rgba(255,255,255,255);");

    ui->categoryListTable->move(0, categoryListTableStrideY);
    ui->categoryListTable->resize(categoryListTableWidth, categoryListTableHeight);
    ui->categoryListTable->setAutoScroll(false);
    ui->categoryListTable->setRowCount(12);
    ui->categoryListTable->setColumnCount(2);

    QStringList list = {"ID","NAME"};
    ui->categoryListTable->setHorizontalHeaderLabels(list);
    ui->categoryListTable->verticalHeader()->hide();

    for(int i = 0; i < 12; ++i){
        if(i == 11){
            ui->categoryListTable->setRowHeight(i,categoryListTableRowHeight-1);
        }
        else{
            ui->categoryListTable->setRowHeight(i,categoryListTableRowHeight);
        }
    }
    ui->categoryListTable->horizontalHeader()->setFixedHeight(categoryListTableRowHeight-1);
    ui->categoryListTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->categoryListTable->setStyleSheet("QTableView{border:1px solid #7a7a7a; background-color: rgba(57,57,57,255); color:rgba(222,222,222,255);}"
                                         "QTableView::item{border-right:1px solid #7a7a7a; border-bottom:1px solid #7a7a7a;}"
                                         "QHeaderView::section{border:1px solid #7a7a7a; background-color: rgba(80,80,80,255); color:rgba(222,222,222,255);}"
                                         "QTableCornerButton::section{border:1px solid #7a7a7a; background-color: rgba(57,57,57,255); color:rgba(222,222,222,255);}");

    ui->categoryListTable->setColumnWidth(0,categoryListTableIDColumnWidth + 2);
    ui->categoryListTable->setColumnWidth(1,categoryListTableNameColumnWidth - 2);
    ui->categoryListTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->logLabel->move(0, logLabelStrideY);
    ui->logLabel->resize(logLabelWidth, logLabelHeight);
    ui->logLabel->setStyleSheet("padding-left:20px; border:1px solid #979797; background-color: rgba(65,85,113,255); color:rgba(255,255,255,255);");
    ui->logLabel->setFont(labelFont);

    ui->logList->move(0, logListStrideY);
    ui->logList->resize(logListWidth, logListHeight);
    ui->logList->setStyleSheet("border:1px solid #979797; background-color: rgba(38,38,38,255); color:rgba(255,255,255,255);");

    ui->chartWidget->move(chartStrideX, chartStrideY);
    ui->chartWidget->resize(chartWidth * 2, chartHeight * 3);

    ui->settingLabel->move(settingLabelStrideX, settingLabelStrideY);
    ui->settingLabel->resize(settingLabelWidth, settingLabelHeight);
    ui->settingLabel->setStyleSheet("padding-left:20px; border:1px solid #979797; background-color: rgba(65,85,113,255); color:rgba(255,255,255,255);");
    ui->settingLabel->setFont(labelFont);

    QStringList settingTableSections = {"MinIF","MaxIF","Category","","","","",""};

    ui->settingTable->move(settingTableStrideX, settingTableStrideY);
    ui->settingTable->resize(settingTableWidth, settingTableHeight);
    ui->settingTable->setAutoScroll(false);
    ui->settingTable->setRowCount(4);
    ui->settingTable->setColumnCount(1);
    ui->settingTable->setVerticalHeaderLabels(settingTableSections);
    ui->settingTable->horizontalHeader()->hide();

    for(int i = 0; i < 4; ++i){
        if(i >= 2){
            ui->settingTable->setRowHeight(i,settingTableRowHeight-1);
        }
        else{
            ui->settingTable->setRowHeight(i,settingTableRowHeight);
        }

        QTableWidgetItem* item = new QTableWidgetItem;
//        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->settingTable->setItem(i,0,item);
    }
    ui->settingTable->verticalHeader()->setFixedWidth(settingTableHeaderWidth);
    ui->settingTable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->settingTable->setStyleSheet("QTableView{border:1px solid #7a7a7a; background-color: rgba(57,57,57,255); color:rgba(222,222,222,255);}"
                                         "QTableView::item{border-right:1px solid #7a7a7a; border-bottom:1px solid #7a7a7a;}"
                                         "QHeaderView::section{border:1px solid #7a7a7a; background-color: rgba(80,80,80,255); color:rgba(222,222,222,255);}"
                                         "QTableCornerButton::section{border:1px solid #7a7a7a; background-color: rgba(57,57,57,255); color:rgba(222,222,222,255);}");

    ui->settingTable->setColumnWidth(0,settingTableParameterWidth);
//    ui->settingTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->statusLabel->move(statusLabelStrideX, statusLabelStrideY);
    ui->statusLabel->resize(statusLabelWidth, statusLabelHeight);
    ui->statusLabel->setStyleSheet("padding-left:20px; border:1px solid #979797; background-color: rgba(65,85,113,255); color:rgba(255,255,255,255);");
    ui->statusLabel->setFont(labelFont);

    ui->statusList->move(statusListStrideX, statusListStrideY);
    ui->statusList->resize(statusListWidth, statusListHeight);
    ui->statusList->setStyleSheet("border:1px solid #979797; background-color: rgba(38,38,38,255); color:rgba(255,255,255,255);");



    ui->resultLabel->move(resultLabelStrideX, resultLabelStrideY);
    ui->resultLabel->resize(resultLabelWidth, resultLabelHeight);
    ui->resultLabel->setStyleSheet("padding-left:20px; border:1px solid #979797; background-color: rgba(65,85,113,255); color:rgba(255,255,255,255);");
    ui->resultLabel->setFont(labelFont);

    ui->recognitionResultLabel->move(recognitionResultStrideX, recognitionResultStrideY);
    ui->recognitionResultLabel->resize(recognitionResultWidth, recognitionResultHeight);
    ui->recognitionResultLabel->setStyleSheet("border:1px solid #979797; background-color: rgba(38,38,38,255); color:rgba(255,255,255,255);");
}

void MainWindow::initialize()
{
    connect(toolButtons[TOOLBAR_DEVICE], SIGNAL(clicked()), this, SLOT(clickedDeviceConnect()));
//    connect(toolButtons[TOOLBAR_DEVICE], SIGNAL(clicked()),   this, SLOT(clickedDeviceDisconnect()));
    connect(toolButtons[TOOLBAR_MOTION], SIGNAL(clicked()), this, SLOT(clickedMotion()));
    connect(toolButtons[TOOLBAR_LEARNING], SIGNAL(clicked()), this, SLOT(clickedLearning()));
    connect(toolButtons[TOOLBAR_RECOGNITION], SIGNAL(clicked()), this, SLOT(clickedRecognition()));
    connect(toolButtons[TOOLBAR_SAVE], SIGNAL(clicked()), this, SLOT(clickedSaveModel()));
    connect(toolButtons[TOOLBAR_LOAD], SIGNAL(clicked()), this, SLOT(clickedLoadModel()));
    connect(toolButtons[TOOLBAR_RESET], SIGNAL(clicked()), this, SLOT(clickedResetModel()));

    connect(sensorObj, SIGNAL(setChartData(preprocessingDataSet)), this, SLOT(setChartData(preprocessingDataSet)));
    connect(sensorObj, SIGNAL(addLearningResult(QString)), this, SLOT(addLearningResult(QString)));
    connect(sensorObj, SIGNAL(clearLearningResult()), this, SLOT(clearLearningResult()));

    QString fontName = "Helvetica";
    QFont titleFont(fontName,16);
    titleFont.setBold(true);


    QGridLayout *layout = new QGridLayout;
    //Init
    for(int i = 0; i < DATA_TOTAL; ++i){

        axisXY[i].axisX = new QValueAxis;
        axisXY[i].axisX->setTickCount(8);
        axisXY[i].axisX->setRange(1,256);
        axisXY[i].axisX->setLabelFormat("%d");

        axisXY[i].axisY = new QValueAxis;
        axisXY[i].axisY->setTickCount(6);
        axisXY[i].axisY->setLabelFormat("%d");
        axisXY[i].axisY->setRange(0,255);

        QChart *chart = new QChart;
        chart->legend()->hide();
        chart->setTitle(GetChartName((_data_list)i));
        chart->addAxis(axisXY[i].axisX, Qt::AlignBottom);
        chart->addAxis(axisXY[i].axisY, Qt::AlignLeft);
        chart->setBackgroundRoundness(0);
        chart->setTitleFont(titleFont);

        chartViews[i] = new QChartView(chart);
        chartViews[i]->setRenderHint(QPainter::Antialiasing);
        chartViews[i]->setStyleSheet("border:1px solid #d9d9d9; background-color: rgba(255,255,255,255);");


        switch(i){

        case DATA_AXIS_X_ACCELERATION:
        {
            layout->addWidget(chartViews[i], 0, 0);
        }
            break;

        case DATA_AXIS_Y_ACCELERATION:
        {
            layout->addWidget(chartViews[i], 1, 0);
        }
            break;

        case DATA_AXIS_Z_ACCELERATION:
        {
            layout->addWidget(chartViews[i], 2, 0);
        }
            break;

        case DATA_PITCH:
        {
            layout->addWidget(chartViews[i], 0, 1);
        }
            break;

        case DATA_ROLL:
        {
            layout->addWidget(chartViews[i], 1, 1);
        }
            break;

        case DATA_YAW:
        {
            layout->addWidget(chartViews[i], 2, 1);
        }
            break;
        }


    }

    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    ui->chartWidget->setLayout(layout);
}

void MainWindow::setChartData(preprocessingDataSet data)
{
    for(int i = 0; i < DATA_TOTAL; ++i){
        chartViews[i]->chart()->removeAllSeries();

        QLineSeries *series = new QLineSeries;

        switch(i){

        case DATA_AXIS_X_ACCELERATION:
        {
            int size = (int)data.accelX.size();

            for(int i = 0; i < size; ++i){
                *series << QPoint((qreal)i,(qreal)data.accelX[i]);
            }

            axisXY[i].axisX->setRange(0,size);

        }
            break;

        case DATA_AXIS_Y_ACCELERATION:
        {
            int size = (int)data.accelY.size();

            for(int i = 0; i < size; ++i){
                *series << QPoint((qreal)i,(qreal)data.accelY[i]);
            }

            axisXY[i].axisX->setRange(0,size);
        }
            break;

        case DATA_AXIS_Z_ACCELERATION:
        {
            int size = (int)data.accelZ.size();

            for(int i = 0; i < size; ++i){
                *series << QPoint((qreal)i,(qreal)data.accelZ[i]);
            }

            axisXY[i].axisX->setRange(0,size);
        }
            break;

        case DATA_PITCH:
        {
            int size = (int)data.pitch.size();

            for(int i = 0; i < size; ++i){
                *series << QPoint((qreal)i,(qreal)data.pitch[i]);
            }

            axisXY[i].axisX->setRange(0,size);
        }
            break;

        case DATA_ROLL:
        {
            int size = (int)data.roll.size();

            for(int i = 0; i < (int)data.roll.size(); ++i){
                *series << QPoint((qreal)i,(qreal)data.roll[i]);
            }

            axisXY[i].axisX->setRange(0,size);
        }
            break;

        case DATA_YAW:
        {
            int size = (int)data.yaw.size();

            for(int i = 0; i < (int)data.yaw.size(); ++i){
                *series << QPoint((qreal)i,(qreal)data.yaw[i]);
            }

            axisXY[i].axisX->setRange(0,size);
        }
            break;
        }

        axisXY[i].axisY->setRange(0,255);

        chartViews[i]->chart()->addSeries(series);

        series->attachAxis(axisXY[i].axisX);
        series->attachAxis(axisXY[i].axisY);
    }
}

void MainWindow::showHelpDialog()
{
    if(!helpDialog){
         helpDialog = new HelpDialog;
    }

    helpDialog->show();
}

void MainWindow::addLearningResult(QString result)
{
    ui->statusList->addItem(result);
}

void MainWindow::clearLearningResult()
{
    ui->statusList->clear();
}

const char* MainWindow::GetChartName(_data_list data)
{
    const char* chartNames[DATA_TOTAL] = { "Acceleration X", "Acceleration Y", "Acceleration Z", "Pitch", "Roll", "Yaw"};
    return chartNames[data];
}

void MainWindow::setMinIFValue(uint16_t minIF)
{
    ui->settingTable->item(0,0)->setText(QString::number(minIF));
}

void MainWindow::setMaxIFValue(uint16_t maxIF)
{
    ui->settingTable->item(1,0)->setText(QString::number(maxIF));
}

void MainWindow::setCategoryValue(uint16_t category)
{
    ui->settingTable->item(2,0)->setText(QString::number(category));
}

void MainWindow::addLog(string log)
{
    ui->logList->addItem(QString::fromStdString(log));
}

uint16_t MainWindow::getCategoryNumber(string category)
{
    uint16_t categoryNumber = 0;

    //First learned
    if(!categoryNameVector.size()){
        categoryNameVector.push_back(category);
        categoryNumber = 1;
    }
    else{

        bool alreadyLearned = false;

        for(int i = 0; i < (int)categoryNameVector.size(); ++i){
            if(categoryNameVector[i] == category){
                categoryNumber = (uint16_t)(i + 1);   //categoty start number is 1.
                alreadyLearned = true;
                break;
            }
        }
        if(!alreadyLearned){
            categoryNameVector.push_back(category);
            categoryNumber = (uint16_t)categoryNameVector.size();
        }
    }

    return categoryNumber;
}

void MainWindow::addCategoryListItem(uint16_t categoryNumber, string category)
{
    QTableWidgetItem* categoryNumberItem = ui->categoryListTable->item(categoryNumber - 1, 0);
    if(!categoryNumberItem){
        categoryNumberItem = new QTableWidgetItem;
    }
    categoryNumberItem->setText(QString::number(categoryNumber));
    ui->categoryListTable->setItem(categoryNumber - 1, 0, categoryNumberItem);

    QTableWidgetItem* categoryNameItem = ui->categoryListTable->item(categoryNumber - 1, 1);
    if(!categoryNameItem){
        categoryNameItem = new QTableWidgetItem;
    }
    categoryNameItem->setText(QString::fromStdString(category));
    ui->categoryListTable->setItem(categoryNumber - 1, 1, categoryNameItem);
}

void MainWindow::setResultImage(uint16_t category)
{
    ui->recognitionResultLabel->setPixmap(motionImageList[category-1].scaled(ui->recognitionResultLabel->size(), Qt::KeepAspectRatio));
}

void MainWindow::createHelpDialog()
{

}

void MainWindow::clickedDeviceConnect()
{
    sensorObj->connectMMRSensor();
}

void MainWindow::clickedDeviceDisconnect()
{

}

void MainWindow::clickedMotion()
{
    if(isMotionStartClicked){
        toolButtons[TOOLBAR_MOTION]->setChecked(true);
        toolButtons[TOOLBAR_MOTION]->setIcon(toolbarIconList[ACTION_START_MOTION]);
        toolButtons[TOOLBAR_MOTION]->setText("Motion Start");
        toolButtons[TOOLBAR_DEVICE]->setEnabled(true);
        toolButtons[TOOLBAR_RECOGNITION]->setEnabled(true);
        toolButtons[TOOLBAR_LEARNING]->setEnabled(true);

        sensorObj->stopMotion();
    }
    else{
        toolButtons[TOOLBAR_MOTION]->setChecked(false);
        toolButtons[TOOLBAR_MOTION]->setIcon(toolbarIconList[ACTION_STOP_MOTION]);
        toolButtons[TOOLBAR_MOTION]->setText("Motion Stop");
        toolButtons[TOOLBAR_DEVICE]->setEnabled(false);
        toolButtons[TOOLBAR_RECOGNITION]->setEnabled(false);
        toolButtons[TOOLBAR_LEARNING]->setEnabled(false);

        sensorObj->startMotion();
    }

    isMotionStartClicked = !isMotionStartClicked;
}

void MainWindow::clickedTrainingStart()
{
    if(!sensorObj->isConnectedMMRSensor()){
        return;
    }

    toolButtons[TOOLBAR_MOTION]->setChecked(false);
    toolButtons[TOOLBAR_MOTION]->setIcon(toolbarIconList[ACTION_STOP_MOTION]);
    toolButtons[TOOLBAR_MOTION]->setText("Motion Stop");

    //Please clear
    ui->statusList->clear();
    ui->statusList->update();
//    ui->recognitionResultLabel->clear();
    ui->recognitionResultLabel->setText(" ");

    sensorObj->startMotion();
}

void MainWindow::clickedTrainingStop()
{
    toolButtons[TOOLBAR_MOTION]->setChecked(true);
    toolButtons[TOOLBAR_MOTION]->setIcon(toolbarIconList[ACTION_START_MOTION]);
    toolButtons[TOOLBAR_MOTION]->setText("Motion Start");
    sensorObj->stopMotion();
}

void MainWindow::clickedRecognition()
{
    uint16_t minIF = (uint16_t)ui->settingTable->item(0,0)->text().toInt();
    uint16_t maxIF = (uint16_t)ui->settingTable->item(1,0)->text().toInt();

    uint16_t category = sensorObj->recognizeGesture(minIF, maxIF);

//    uint16_t category = sensorObj->recognizeGesture2(minIF, maxIF);

//    uint16_t category = sensorObj->recognizeGesture3(minIF, maxIF);

    QString result;

    if(category == 0xffff){
        result = "Recognition Result : Unknown";
    }
    else{
        result = "Recognition Result : ";
        result.append(QString::fromStdString(categoryNameVector[category-1]));
//        setResultImage(category);
    }


    ui->recognitionResultLabel->setText(result);
}

void MainWindow::clickedLearning()
{
    //temporary exception
    if(categoryNameVector.size() > 12)
        return;

    uint16_t minIF = (uint16_t)ui->settingTable->item(0,0)->text().toInt();
    uint16_t maxIF = (uint16_t)ui->settingTable->item(1,0)->text().toInt();

    string category = ui->settingTable->item(2,0)->text().toStdString();

    uint16_t categoryNumber = getCategoryNumber(category);

    NMHelper::getInstance()->nmWriteMinIF(minIF);
    NMHelper::getInstance()->nmWriteMaxIF(maxIF);

    bool result = sensorObj->LearnGesture(categoryNumber, minIF, maxIF);

//    bool result = sensorObj->LearnGesture2(categoryNumber, minIF, maxIF);

//    bool result = sensorObj->LearnGesture3(categoryNumber, minIF, maxIF);

    if(result){
        addCategoryListItem(categoryNumber, category);
    }
}

void MainWindow::clickedSaveModel()
{
    remove(NEURON_DATA_PATH);

    NMHelper::getInstance()->nmSaveModel(NEURON_DATA_PATH);

    remove(CATEGORY_DATA_PATH);;
    ofstream fileName(CATEGORY_DATA_PATH);

    if(!fileName.is_open()){
        //please add log
        return;
    }

    for(size_t i = 0; i < categoryNameVector.size(); ++i){
        fileName << categoryNameVector[i]<< std::endl;
    }

    fileName << std::endl;
    fileName.close();
}

void MainWindow::clickedLoadModel()
{
    NMHelper::getInstance()->nmLoadModel(NEURON_DATA_PATH);

    std::ifstream fileInput(CATEGORY_DATA_PATH);
    if(!fileInput.is_open()){
        return;
    }

    //cat matching
    categoryNameVector.clear();

    while(true){
        string nameData;
        getline(fileInput, nameData);

        if(nameData.length() <= 0)
            break;

        categoryNameVector.push_back(nameData);
    }

    fileInput.close();

    //please check QTableWidget reset command.
    for(int i = 0; i < (int)categoryNameVector.size(); ++i){
        addCategoryListItem((uint16_t)i+1, categoryNameVector[i]);
    }
}

void MainWindow::clickedResetModel()
{
    NMHelper::getInstance()->nmForget();

    for(int i = 0; i < ui->categoryListTable->rowCount(); ++i){
        QTableWidgetItem* itemID = ui->categoryListTable->item(i,0);
        if(itemID){
            itemID->setText("");
        }

        QTableWidgetItem* itemName = ui->categoryListTable->item(i,1);
        if(itemName){
            itemName->setText("");
        }
    }

    categoryNameVector.clear();
}
