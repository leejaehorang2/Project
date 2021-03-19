//#ifndef BLUETOOTH_SCANNER_HPP
//#define BLUETOOTH_SCANNER_HPP

#ifndef GESTURE_SENSOR_HPP
#define GESTURE_SENSOR_HPP

#include <thread>
#include <future>
#include <QObject>

#include "metawear/core/metawearboard.h"

#include "definition.hpp"
#include "nm_helper.hpp"

using namespace std;

class GestureSensor : public QObject
{
    Q_OBJECT

    struct sensorDataSet{
        vector<float> accelX;
        vector<float> accelY;
        vector<float> accelZ;
        vector<float> gyroX;
        vector<float> gyroY;
        vector<float> gyroZ;
    };

    // test //
    struct quaternion{
        float q0;
        float q1;
        float q2;
        float q3;
    };
    struct euler{
        float first;
        float second;
        float third;
    };

    struct classifyStruct{
//        uint16_t accelXCategory;
//        uint16_t accelYCategory;
//        uint16_t accelZCategory;
//        uint16_t rollCategory;
//        uint16_t pitchCategory;
//        uint16_t yawCategory;

//        uint16_t accelXDistance;
//        uint16_t accelYDistance;
//        uint16_t accelZDistance;
//        uint16_t rollDistance;
//        uint16_t pitchDistance;
//        uint16_t yawDistance;
        nm_classify_req accelXClassify;
        nm_classify_req accelYClassify;
        nm_classify_req accelZClassify;
        nm_classify_req rollClassify;
        nm_classify_req pitchClassify;
        nm_classify_req yawClassify;
    };

public:
    GestureSensor(QObject *parent = nullptr);
    ~GestureSensor();

    void connectMMRSensor();

    bool isConnectedMMRSensor();

    void startMotion();
    void stopMotion();

    bool LearnGesture(uint16_t category, uint16_t minIF, uint16_t maxIF);
    uint16_t recognizeGesture(uint16_t minIF, uint16_t maxIF);

    bool LearnGesture2(uint16_t category, uint16_t minIF, uint16_t maxIF);
    uint16_t recognizeGesture2(uint16_t minIF, uint16_t maxIF);

    bool LearnGesture3(uint16_t category, uint16_t minIF, uint16_t maxIF);
    uint16_t recognizeGesture3(uint16_t minIF, uint16_t maxIF);

    bool LearnGestureOnlyRot(uint16_t category, uint16_t minIF, uint16_t maxIF);
    uint16_t recognizeGestureOnlyRot(uint16_t minIF, uint16_t maxIF);

private:

    void tPD();

    void onlyRotatepreprocessData();

    void testPreprocessData();
    void newPreprocessData();
    void oldPreprocessData();

    void thresholdSensorData();
    void normalizeData(float oldMin, float oldMax, vector<float> data, vector<uint8_t>& result);
    void linearInterpolation(vector<float> srcData, vector<float> &dstData, int number);

    void getPitchVector(vector<float> accelX, vector<float> accelY, vector<float> accelZ, vector<float> &result);
    void getRollVector(vector<float> accelX, vector<float> accelY, vector<float> accelZ, vector<float> &result);
    void getYawVector(vector<float> accelX, vector<float> accelY, vector<float> accelZ, vector<float> &result);

    void clearAllVector();

    uint16_t judgeCategory(uint16_t accelCategory, uint16_t accelDistance, uint16_t rotateCategory, uint16_t rotateDistance);
    uint16_t judgeCategory2(classifyStruct cst);
    uint16_t judgeCategory3(classifyStruct cst);

    // Test //
    void getQuaternionConjugate(float q0, float q1, float q2, float q3, quaternion &quat);
    void convertQuaternionToEuler(float q0, float q1, float q2, float q3, euler &e);
signals:
    void setChartData(preprocessingDataSet data);
    void addLearningResult(QString result);
    void clearLearningResult();

private:
    MblMwMetaWearBoard* mmrBoard;
    MblMwBtleConnection btleConnection;

    sensorDataSet dataSet;

    vector<uint8_t> accelInputVector;
    vector<uint8_t> rotationInputVector;

    vector<uint8_t> accelXInputVector;
    vector<uint8_t> accelYInputVector;
    vector<uint8_t> accelZInputVector;
    vector<uint8_t> rollInputVector;
    vector<uint8_t> pitchInputVector;
    vector<uint8_t> yawInputVector;
};

#endif // GESTURE_SENSOR_HPP
