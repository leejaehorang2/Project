#include "main_window.hpp"
#include "ui_main_window.h"

#include <QGridLayout>
#include <QLineSeries>

#include <iostream>

using namespace std;

//QT_CHARTS_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initialize()
{
    setWindowTitle("Gesture Recognition");

    connect(ui->buttonTest1, SIGNAL(clicked()), this, SLOT(clickedTest1()));
    connect(ui->buttonTest2, SIGNAL(clicked()), this, SLOT(clickedTest2()));
    connect(ui->buttonTest3, SIGNAL(clicked()), this, SLOT(clickedTest3()));
    connect(ui->buttonTest4, SIGNAL(clicked()), this, SLOT(clickedTest4()));
    connect(ui->buttonTest5, SIGNAL(clicked()), this, SLOT(clickedTest5()));
    connect(ui->buttonTest6, SIGNAL(clicked()), this, SLOT(clickedTest6()));

    connect(connectionHelper, SIGNAL(setChartData(vector<sensorData>)), this, SLOT(setChartData(vector<sensorData>)));

    //Init
    for(int i = 0; i < AXIS_TOTAL; ++i){
        QChart *chart = new QChart;
        chart->legend()->hide();
//        chart->createDefaultAxes();
        chart->setTitle(GetChartName((_axis)i));

        axisXY[i].axisX = new QValueAxis;
        axisXY[i].axisX->setTickCount(4);
        axisXY[i].axisX->setRange(0,256);
        axisXY[i].axisX->setLabelFormat("%d");
        chart->addAxis(axisXY[i].axisX, Qt::AlignBottom);

        axisXY[i].axisY = new QValueAxis;
        axisXY[i].axisY->setTickCount(6);
        axisXY[i].axisY->setRange(-1.5,1.5);
        axisXY[i].axisY->setLabelFormat("%.3f");
        chart->addAxis(axisXY[i].axisY, Qt::AlignLeft);

        chartViews[i] = new QChartView(chart);
        chartViews[i]->setRenderHint(QPainter::Antialiasing);

        QGridLayout *layout = new QGridLayout;
        layout->addWidget(chartViews[i]);

        switch(i){

        case AXIS_X_ACCELEROMETER:
        {
            ui->accelXWidget->setLayout(layout);
            ui->accelXWidget->setVisible(true);
        }
            break;

        case AXIS_Y_ACCELEROMETER:
        {
            ui->accelYWidget->setLayout(layout);
            ui->accelYWidget->setVisible(false);
        }
            break;

        case AXIS_Z_ACCELEROMETER:
        {
            ui->accelZWidget->setLayout(layout);
            ui->accelZWidget->setVisible(false);
        }
            break;

        case AXIS_X_GYRO:
        {
            ui->rotateXWidget->setLayout(layout);
            ui->rotateXWidget->setVisible(false);
        }
            break;

        case AXIS_Y_GYRO:
        {
            ui->rotateYWidget->setLayout(layout);
            ui->rotateYWidget->setVisible(false);
        }
            break;

        case AXIS_Z_GYRO:
        {
            ui->rotateZWidget->setLayout(layout);
            ui->rotateZWidget->setVisible(false);
        }
            break;
        }

    }
}

void MainWindow::setChartData(vector<sensorData> data)
{
    for(int i = 0; i < AXIS_TOTAL; ++i){
        chartViews[i]->chart()->removeAllSeries();

        QLineSeries *series = new QLineSeries;

//        for(int i = 0; i < (int)data.size(); ++i){
//            *series << QPointF((qreal)i,(qreal)data[i].value[i]);
//        }

        for(int i = 0; i < 10; ++i){
            QPointF q((qreal)i,(qreal)data[i].value[i]);
            *series << q;
        }

        chartViews[i]->chart()->addSeries(series);

        series->attachAxis(axisXY[i].axisX);
        series->attachAxis(axisXY[i].axisY);

        chartViews[i]->chart()->show();
    }
}

const char* MainWindow::GetChartName(_axis axis)
{
    const char* chartNames[AXIS_TOTAL] = { "Acceleration X", "Acceleration Y", "Acceleration Z", "Rotation X", "Rotation Y", "Rotation Z"};
    return chartNames[axis];
}

void MainWindow::clickedTest1()
{
    sensorObj->connectMMRSensor();
}


void MainWindow::clickedTest2()
{
    sensorObj->testLED();
}


void MainWindow::clickedTest3()
{
    sensorObj->testAccelerometer();
}

void MainWindow::clickedTest4()
{
    sensorObj->testGyro();
}

void MainWindow::clickedTest5()
{
    sensorObj->testSensorFusion();
}

void MainWindow::clickedTest6()
{
    sensorObj->testAccelAndGyro();
}
