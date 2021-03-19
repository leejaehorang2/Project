#ifndef BLUETOOTH_SCANNER_HPP
#define BLUETOOTH_SCANNER_HPP

#include <thread>
#include <future>

#include "metawear/core/metawearboard.h"

#include "uiconnection_helper.hpp"

using namespace std;

class MblMwDataProcessor;

class GestureSensor
{
    struct dataProcessorStruct{
        vector<sensorData> data;
        promise<MblMwDataProcessor*> fuserCreateTask;
    };

public:
    GestureSensor();
    ~GestureSensor();

    void setUIConnectionHelper(UIConnectionHelper *helper)    {   connectionHelper = helper;    }

    void startThread();
    void stopThread();

    //Temp
    void connectMMRSensor();
    void testLED();
    void testAccelerometer();
    void testGyro();
    void testSensorFusion();
    void testAccelAndGyro();

    void testBluetoothScan();

private:
    void bluetoothScanThread();

private:
    UIConnectionHelper *connectionHelper;

    MblMwMetaWearBoard* mmrBoard;
    MblMwBtleConnection btleConnection;
    thread scanThread;

    shared_future<void> m_TT;

    volatile bool running;
};

#endif // BLUETOOTH_SCANNER_HPP
