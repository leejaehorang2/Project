#include "main_window.hpp"

#include <QApplication>
#include <iostream>

#include "gesture_sensor.hpp"
#include "nm_helper.hpp"

#define MINIF_VALUE 20
#define MAXIF_VALUE 7500

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    GestureSensor sensorObject;

    w.setGestureSensorObject(&sensorObject);

    w.initializeWidgets();
    w.initialize();
    w.showMaximized();
    w.showHelpDialog();

    w.setMinIFValue(MINIF_VALUE);
    w.setMaxIFValue(MAXIF_VALUE);
    w.setCategoryValue(1);

    //NM500 Test
    if(!NMHelper::getInstance()->nmGetDevices()) {
        cout << "Failed to get devices" << endl;
        w.addLog("Failed to get devices");
        return 1;
    }
    else{
        w.addLog("Get device success.");
    }

    if(!NMHelper::getInstance()->nmConnect()) {
        cout << "Failed to connect" << endl;
        w.addLog("Failed to connect");
        return 1;
    }
    else{
       w.addLog("Device connection success.");
    }

    if(!NMHelper::getInstance()->nmForget()) {
        cout << "Failed to forget" << endl;
        w.addLog("Failed to forget");
        return 1;
    }
    else{
       w.addLog("Forget command success.");
    }

    if(!NMHelper::getInstance()->nmSetNetworkTypeToRBF()) {
        cout << "Failed to set network type to RBF" << endl;
        w.addLog("Failed to set network type to RBF");
        return 1;
    }
    else{
       w.addLog("Set network type success : RBF");
    }

    if(!NMHelper::getInstance()->nmWriteMaxIF(MAXIF_VALUE)) {
        cout << "Failed to set MAX IF" << endl;
        w.addLog("Failed to set MAX IF");
        return 1;
    }

    a.exec();

    if(!NMHelper::getInstance()->nmPowerSave()){
        cout << "Failed to powersave" << endl;
        return 1;
    }

    if(!NMHelper::getInstance()->nmClose()){
        cout << "Failed to close" << endl;
        return 1;
    }

    NMHelper::releaseInstance();

    return 0;
}
