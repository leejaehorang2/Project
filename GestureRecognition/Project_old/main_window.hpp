#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

#include "gesture_sensor.hpp"
#include "uiconnection_helper.hpp"

#include <QValueAxis>
#include <QChartView>

using namespace std;

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    struct chartAxis{
        QValueAxis *axisX;
        QValueAxis *axisY;
    };

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initialize();

    void setGestureSensorObject(GestureSensor *sensor)   {   sensorObj = sensor;    }
    void setUIConnectionHelper(UIConnectionHelper *helper)    {   connectionHelper = helper;    }

    void setTTT();

private slots:
    void setChartData(vector<sensorData> data);

    void clickedTest1();
    void clickedTest2();
    void clickedTest3();
    void clickedTest4();
    void clickedTest5();
    void clickedTest6();

private:
    const char* GetChartName(_axis axis);

private:
    Ui::MainWindow *ui;

    GestureSensor *sensorObj;
    UIConnectionHelper *connectionHelper;

    QChartView *chartViews[AXIS_TOTAL];
    chartAxis axisXY[AXIS_TOTAL];

//    QValueAxis *axisX;
//    QValueAxis *axisY;
//    queue<QLineSeries*> lineSeries;
};


#endif // MAIN_WINDOW_H
