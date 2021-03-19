#include "main_window.hpp"
#include <QApplication>

#include "gesture_sensor.hpp"
#include "uiconnection_helper.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    GestureSensor sensorObject;
    UIConnectionHelper helper;

    w.setGestureSensorObject(&sensorObject);
    w.setUIConnectionHelper(&helper);
    sensorObject.setUIConnectionHelper(&helper);

    w.initialize();

    w.show();

    return a.exec();
}
