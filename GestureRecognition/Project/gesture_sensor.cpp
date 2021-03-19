#include "gesture_sensor.hpp"

#include "warble/warble.h"

#include "metawear/core/status.h"
#include "metawear/peripheral/led.h"
#include "metawear/sensor/accelerometer.h"
#include "metawear/core/logging.h"
#include "metawear/core/datasignal.h"
#include "metawear/core/settings.h"
#include "metawear/core/types.h"
#include "metawear/sensor/gyro_bmi160.h"
#include "metawear/sensor/sensor_fusion.h"
#include "metawear/platform/memory.h"
#include "metawear/processor/dataprocessor.h"
#include "metawear/processor/fuser.h"
#include "metawear/sensor/magnetometer_bmm150.h"
#include "metawear/core/debug.h"

#include <iomanip>  //for setw, setfill
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sys/stat.h>   //for 'mkdir'

#include <math.h>

//Test
#include "madgwick_ahrs.hpp"
#include <fstream>

using namespace std;
using namespace this_thread;
using namespace chrono;

static WarbleGatt* warbleGatt = nullptr;
static WarbleGattChar* charGattChar = nullptr;
static WarbleGattChar* enableGattChar = nullptr;

static unordered_map<const void*, MblMwFnIntVoidPtrArray> notifyHandlers;
static unordered_map<const void*, MblMwFnVoidVoidPtrInt> disconnectHandlers;

static void read_gatt_char(void* context, const void* caller, const MblMwGattChar* characteristic, MblMwFnIntVoidPtrArray handler);
static void write_gatt_char(void* context, const void* caller, MblMwGattCharWriteType write_type, const MblMwGattChar* characteristic, const uint8_t* value, uint8_t length);
static void enable_char_notify(void* context, const void* caller, const MblMwGattChar* characteristic, MblMwFnIntVoidPtrArray handler, MblMwFnVoidVoidPtrInt ready);
static void on_disconnect(void* context, const void* caller, MblMwFnVoidVoidPtrInt handler);

static mutex funcMutex;

#define ACCEL_X_THRESHOLD 0.1f
#define ACCEL_Y_THRESHOLD 0.1f
#define ACCEL_Z_THRESHOLD 1.0f

#define GYRO_X_THRESHOLD 0.3f
#define GYRO_Y_THRESHOLD 0.6f
#define GYRO_Z_THRESHOLD 0.5f

#define PI 3.14159265

GestureSensor::GestureSensor(QObject *parent) : QObject(parent)
{
    mmrBoard = nullptr;
//    dataProc.data.reserve(256);

    dataSet.accelX.reserve(256);
    dataSet.accelY.reserve(256);
    dataSet.accelZ.reserve(256);
    dataSet.gyroX.reserve(256);
    dataSet.gyroY.reserve(256);
    dataSet.gyroZ.reserve(256);

    accelInputVector.reserve(256);
    rotationInputVector.reserve(256);
}

GestureSensor::~GestureSensor()
{
    if(warbleGatt){
        if(warble_gatt_is_connected(warbleGatt)){
            warble_gatt_disconnect(warbleGatt);
        }
//        warble_gatt_delete(warbleGatt);
    }

    if(mmrBoard){

        this_thread::sleep_for(std::chrono::milliseconds(1000));

        mbl_mw_logging_clear_entries(mmrBoard);

        mbl_mw_sensor_fusion_clear_enabled_mask(mmrBoard);

        mbl_mw_sensor_fusion_stop(mmrBoard);

        this_thread::sleep_for(std::chrono::milliseconds(1000));

//        mbl_mw_metawearboard_tear_down(mmrBoard);

        mbl_mw_debug_reset(mmrBoard);

        mbl_mw_debug_disconnect(mmrBoard);
    }
}

void GestureSensor::connectMMRSensor()
{
    if(warbleGatt && warble_gatt_is_connected(warbleGatt)){
        cout << "Already Connected!" << endl;
        return;
    }

    warbleGatt = warble_gatt_create("F8:B2:A7:B6:30:7E");

    btleConnection = { nullptr, write_gatt_char, read_gatt_char, enable_char_notify, on_disconnect};

    mmrBoard = mbl_mw_metawearboard_create(&btleConnection);

    if(!mmrBoard){
        cout << " board is nullptr !" << endl;
        return;
    }

    mbl_mw_metawearboard_set_time_for_response(mmrBoard,1000);

    ifstream instream;
    ofstream outstream;
    instream.open("../.metawear");

    if(!instream){
        mkdir("../.metawear",0755);
    }
    instream.close();

    instream.open("../.metawear/F8B2A7B6307E.json");

    vector<int> state;

    if(instream.is_open()){
        while(!instream.eof()){
            std::string temp;
            std::getline(instream, temp);
            int a = std::stoi(temp);
            state.push_back(a);
        }
        instream.close();
    }

    uint8_t* stat = new uint8_t[state.size()];
    for(int i = 0; i < state.size(); ++i){
        stat[i] = state[i];
    }

    mbl_mw_metawearboard_deserialize(mmrBoard,stat,state.size());

    promise<int> connectTask;

    cout << "connecting to : " << "F8:B2:A7:B6:30:7E" << endl;

    warble_gatt_connect_async(warbleGatt, &connectTask, [](void* context, WarbleGatt* caller, const char* value){

        auto task = (promise<int>*) context;

        if(value != nullptr){
            cout << "You done fucked up" << endl;
            task->set_value(1);
//            task->set_exception(make_exception_ptr(runtime_error(value)));
        }
        else{
//            cout << "Am I Connected ?" << warble_gatt_is_connected(caller) << endl;
            this_thread::sleep_for(std::chrono::milliseconds(3000));
            task->set_value(0);
        }
    });

    int returnValue = connectTask.get_future().get();

    //Faile
    if(returnValue == 1){
        cout << "Please retry" << endl;

        mbl_mw_logging_clear_entries(mmrBoard);

        mbl_mw_sensor_fusion_clear_enabled_mask(mmrBoard);

        mbl_mw_sensor_fusion_stop(mmrBoard);

        this_thread::sleep_for(std::chrono::milliseconds(1000));

        mbl_mw_debug_reset(mmrBoard);

        warble_gatt_delete(warbleGatt);
        warbleGatt = nullptr;

        //One more time
        connectMMRSensor();

        return;
    }

    cout << "Connect Complete! " << endl;

    enableGattChar = warble_gatt_find_characteristic(warbleGatt,"326a9006-85cb-9195-d9dd-464cfbbae75a");
    charGattChar = warble_gatt_find_characteristic(warbleGatt,"326a9001-85cb-9195-d9dd-464cfbbae75a");

    uint32_t size = 0;
    mbl_mw_metawearboard_serialize(mmrBoard,&size);

    promise<void> initTask;

    mbl_mw_metawearboard_initialize(mmrBoard, &initTask, [](void* context, MblMwMetaWearBoard* board, int32_t status) -> void {

        auto task = (promise<void>*)context;

        if(!board){
            cout << "Failed to initialize board!! Result : " << mbl_mw_metawearboard_is_initialized(board) << ", " << status <<endl;
            task->set_value();
        }
        else{
            cout << "Board initialized! Result : " << mbl_mw_metawearboard_is_initialized(board) << ", " << status <<endl;
            task->set_value();
        }
    });

    initTask.get_future().get();

//    startSensorFusion();
    mbl_mw_settings_set_connection_parameters(mmrBoard, 7.5, 7.5, 0, 6000);
}

bool GestureSensor::isConnectedMMRSensor()
{
    bool result = false;

    if(warbleGatt && warble_gatt_is_connected(warbleGatt)){
        result = true;
    }

    return result;
}

void GestureSensor::startMotion()
{
    clearAllVector();

    MblMwDataSignal* accelSignal = mbl_mw_acc_get_acceleration_data_signal(mmrBoard);
    if(!accelSignal){
        cout << "Failed to get accelerometer signal!" << endl;
    }

    MblMwDataSignal* gyroSignal = mbl_mw_gyro_bmi160_get_rotation_data_signal(mmrBoard);
    if(!gyroSignal){
        cout << "Failed to get gyro signal!" << endl;
    }

    static auto procCreated = [](void* context, MblMwDataProcessor* fuser){
        auto task = (sensorDataSet*)context;

        cout << "Downloading..." << endl;

        if(fuser){
            mbl_mw_datasignal_subscribe((MblMwDataSignal*)fuser, context, [](void* context, const MblMwData* data){

                auto task = (sensorDataSet*)context;

                auto fused = (MblMwData**)data->value;
                auto acc = (MblMwCorrectedCartesianFloat*)fused[0]->value;
                auto gyro = (MblMwCorrectedCartesianFloat*)fused[1]->value;

                task->accelX.push_back(acc->x);
                task->accelY.push_back(acc->y);
                task->accelZ.push_back(acc->z);
                task->gyroX.push_back(gyro->x);
                task->gyroY.push_back(gyro->y);
                task->gyroZ.push_back(gyro->z);

                cout << "ACC X: " << acc->x << " ACC Y : " << acc->y << " ACC Z : " << acc->z
                     << " GYRO X: " << gyro->x << " GYRO Y : " << gyro->y << " GYRO Z : " << gyro->z << endl;

            });
        }
        else{
            cout << "Failed to get MblMwDataProcessor!" << endl;
        }
    };



//    mbl_mw_sensor_fusion_set_mode(mmrBoard, MBL_MW_SENSOR_FUSION_MODE_IMU_PLUS);
    mbl_mw_sensor_fusion_set_mode(mmrBoard, MBL_MW_SENSOR_FUSION_MODE_NDOF);

//    mbl_mw_dataprocessor_fuser_create(accelSignal, &gyroSignal, 1, &dataSet, procCreated);
    mbl_mw_dataprocessor_fuser_create(accelSignal, &gyroSignal, 1, &dataSet, procCreated);

    mbl_mw_sensor_fusion_set_acc_range(mmrBoard,MBL_MW_SENSOR_FUSION_ACC_RANGE_8G);
    mbl_mw_sensor_fusion_set_gyro_range(mmrBoard,MBL_MW_SENSOR_FUSION_GYRO_RANGE_250DPS);

    mbl_mw_sensor_fusion_write_config(mmrBoard);

    mbl_mw_sensor_fusion_start(mmrBoard);
}

void GestureSensor::stopMotion()
{
    mbl_mw_logging_clear_entries(mmrBoard);

    mbl_mw_sensor_fusion_clear_enabled_mask(mmrBoard);

    mbl_mw_sensor_fusion_stop(mmrBoard);

    this_thread::sleep_for(std::chrono::milliseconds(1000));

    mbl_mw_metawearboard_tear_down(mmrBoard);

    cout << "Download Complete!" << endl;

    //        preprocessData();
    //        testPreprocessData();

    //        newPreprocessData();
//    onlyRotatepreprocessData();

    tPD();
}

void GestureSensor::tPD()
{
    if(!dataSet.accelX.size()){
        return;
    }

    if(!dataSet.accelY.size()){
        return;
    }

    if(!dataSet.accelZ.size()){
        return;
    }

    if(!dataSet.gyroX.size()){
        return;
    }

    if(!dataSet.gyroY.size()){
        return;
    }

    if(!dataSet.gyroZ.size()){
        return;
    }

    //Threshold
    thresholdSensorData();

    vector<quaternion> quatVct;
    vector<quaternion> quatConjVct;

    for(int i = 0; i < dataSet.accelX.size(); ++i){
        MadgwickAHRSupdateIMU(dataSet.gyroX[i], dataSet.gyroY[i], dataSet.gyroZ[i], dataSet.accelX[i], dataSet.accelY[i], dataSet.accelZ[i]);
        float tempQ0 = q0;
        float tempQ1 = q1;
        float tempQ2 = q2;
        float tempQ3 = q3;

        quaternion quat;
        quat.q0 = tempQ0;
        quat.q1 = tempQ1;
        quat.q2 = tempQ2;
        quat.q3 = tempQ3;

        quatVct.push_back(quat);
    }

    for(int i = 0; i < dataSet.accelX.size(); ++i){
        quaternion quatConj;
        getQuaternionConjugate(quatVct[i].q0, quatVct[i].q1, quatVct[i].q2, quatVct[i].q3, quatConj);

        quatConjVct.push_back(quatConj);
    }

    vector<float> roll;
    vector<float> pitch;
    vector<float> yaw;

    for(int i = 0; i < dataSet.accelX.size(); ++i){
        euler e;
        convertQuaternionToEuler(quatConjVct[i].q0, quatConjVct[i].q1, quatConjVct[i].q2, quatConjVct[i].q3, e);
        e.first = e.first * (180.f / PI);
        e.second = e.second * (180.f / PI);
        e.third = e.third * (180.f / PI);

//        eVct.push_back(e);
        roll.push_back(e.first);
        pitch.push_back(e.second);
        yaw.push_back(e.third);

        cout << "X : " << e.first << " Y : " << e.second << " Z : " << e.third << endl;
    }

    initializeQuaternionVariable();

    vector<float> interpolationAccelX;
    vector<float> interpolationAccelY;
    vector<float> interpolationAccelZ;
    vector<float> interpolationRoll;
    vector<float> interpolationPitch;
    vector<float> interpolationYaw;

    linearInterpolation(dataSet.accelX, interpolationAccelX, 85);
    linearInterpolation(dataSet.accelY, interpolationAccelY, 85);
    linearInterpolation(dataSet.accelZ, interpolationAccelZ, 85);
    linearInterpolation(roll, interpolationRoll, 85);
    linearInterpolation(pitch, interpolationPitch, 85);
    linearInterpolation(yaw, interpolationYaw, 85);


    vector<uint8_t> normalizationAccelX;
    vector<uint8_t> normalizationAccelY;
    vector<uint8_t> normalizationAccelZ;
    vector<uint8_t> normalizationRoll;
    vector<uint8_t> normalizationPitch;
    vector<uint8_t> normalizationYaw;

    normalizeData(-8.f, 8.f, interpolationAccelX, normalizationAccelX);
    normalizeData(-8.f, 8.f, interpolationAccelY, normalizationAccelY);
    normalizeData(-8.f, 8.f, interpolationAccelZ, normalizationAccelZ);
    normalizeData(-180.f, 180.f, interpolationRoll, normalizationRoll);
    normalizeData(-90.f, 90.f, interpolationPitch, normalizationPitch);
    normalizeData(-180.f, 180.f, interpolationYaw, normalizationYaw);

    preprocessingDataSet preproData;

    preproData.accelX = normalizationAccelX;
    preproData.accelY = normalizationAccelY;
    preproData.accelZ = normalizationAccelZ;
    preproData.roll = normalizationRoll;
    preproData.pitch = normalizationPitch;
    preproData.yaw = normalizationYaw;

    emit setChartData(preproData);

    for(int i = 0; i < normalizationAccelX.size(); ++i){
        accelInputVector.push_back(normalizationAccelX[i]);
    }

    for(int i = 0; i < normalizationAccelY.size(); ++i){
        accelInputVector.push_back(normalizationAccelY[i]);
    }

    for(int i = 0; i < normalizationAccelZ.size(); ++i){
        accelInputVector.push_back(normalizationAccelZ[i]);
    }

    for(int i = 0; i < normalizationRoll.size(); ++i){
        rotationInputVector.push_back(normalizationRoll[i]);
    }

    for(int i = 0; i < normalizationPitch.size(); ++i){
        rotationInputVector.push_back(normalizationPitch[i]);
    }

    for(int i = 0; i < normalizationYaw.size(); ++i){
        rotationInputVector.push_back(normalizationYaw[i]);
    }
}

void GestureSensor::onlyRotatepreprocessData()
{
    if(!dataSet.accelX.size()){
        return;
    }

    if(!dataSet.accelY.size()){
        return;
    }

    if(!dataSet.accelZ.size()){
        return;
    }

    if(!dataSet.gyroX.size()){
        return;
    }

    if(!dataSet.gyroY.size()){
        return;
    }

    if(!dataSet.gyroZ.size()){
        return;
    }

    //Threshold
    thresholdSensorData();

    vector<quaternion> quatVct;
    vector<quaternion> quatConjVct;

    for(int i = 0; i < dataSet.accelX.size(); ++i){
        MadgwickAHRSupdateIMU(dataSet.gyroX[i], dataSet.gyroY[i], dataSet.gyroZ[i], dataSet.accelX[i], dataSet.accelY[i], dataSet.accelZ[i]);
        float tempQ0 = q0;
        float tempQ1 = q1;
        float tempQ2 = q2;
        float tempQ3 = q3;

        quaternion quat;
        quat.q0 = tempQ0;
        quat.q1 = tempQ1;
        quat.q2 = tempQ2;
        quat.q3 = tempQ3;

        quatVct.push_back(quat);
    }

    for(int i = 0; i < dataSet.accelX.size(); ++i){
        quaternion quatConj;
        getQuaternionConjugate(quatVct[i].q0, quatVct[i].q1, quatVct[i].q2, quatVct[i].q3, quatConj);

        quatConjVct.push_back(quatConj);
    }

    vector<float> roll;
    vector<float> pitch;
    vector<float> yaw;






    //test
    std::ofstream fileName("../Data/1.txt");

    if(!fileName.is_open()){
        cout << "NNNNOOO!!!!!!" << endl;
        return;
    }


    //







    for(int i = 0; i < dataSet.accelX.size(); ++i){
        euler e;
        convertQuaternionToEuler(quatConjVct[i].q0, quatConjVct[i].q1, quatConjVct[i].q2, quatConjVct[i].q3, e);
        e.first = e.first * (180.f / PI);
        e.second = e.second * (180.f / PI);
        e.third = e.third * (180.f / PI);

//        eVct.push_back(e);
        roll.push_back(e.first);
        pitch.push_back(e.second);
        yaw.push_back(e.third);

//        cout << "X : " << e.first << " Y : " << e.second << " Z : " << e.third << endl;

        //Roll, Pitch, Yaw
        cout << e.first << ", " << e.second << ", " << e.third << endl;
    }





    //test

    for(int i = 0; i < roll.size(); ++i){
        fileName << roll[i] << " ";
        fileName << pitch[i] << " ";

        if(i == roll.size() - 1){
            fileName << yaw[i];
        }
        else{
            fileName << yaw[i] << " ";
        }
    }

    fileName.close();


    //




    initializeQuaternionVariable();

//    vector<float> interpolationAccelX;
//    vector<float> interpolationAccelY;
//    vector<float> interpolationAccelZ;
    vector<float> interpolationRoll;
    vector<float> interpolationPitch;
    vector<float> interpolationYaw;

//    linearInterpolation(dataSet.accelX, interpolationAccelX, 256);
//    linearInterpolation(dataSet.accelY, interpolationAccelY, 256);
//    linearInterpolation(dataSet.accelZ, interpolationAccelZ, 256);
    linearInterpolation(roll, interpolationRoll, 256);
    linearInterpolation(pitch, interpolationPitch, 256);
    linearInterpolation(yaw, interpolationYaw, 256);


//    vector<uint8_t> normalizationAccelX;
//    vector<uint8_t> normalizationAccelY;
//    vector<uint8_t> normalizationAccelZ;
    vector<uint8_t> normalizationRoll;
    vector<uint8_t> normalizationPitch;
    vector<uint8_t> normalizationYaw;

//    normalizeData(-8.f, 8.f, interpolationAccelX, normalizationAccelX);
//    normalizeData(-8.f, 8.f, interpolationAccelY, normalizationAccelY);
//    normalizeData(-8.f, 8.f, interpolationAccelZ, normalizationAccelZ);
    normalizeData(-180.f, 180.f, interpolationRoll, normalizationRoll);
    normalizeData(-90.f, 90.f, interpolationPitch, normalizationPitch);
    normalizeData(-180.f, 180.f, interpolationYaw, normalizationYaw);

    preprocessingDataSet preproData;
//    preproData.accelX = normalizationAccelX;
//    preproData.accelY = normalizationAccelY;
//    preproData.accelZ = normalizationAccelZ;
    preproData.roll = normalizationRoll;
    preproData.pitch = normalizationPitch;
    preproData.yaw = normalizationYaw;

    emit setChartData(preproData);

//    for(int i = 0; i < normalizationAccelX.size(); ++i){
//        accelXInputVector.push_back(normalizationAccelX[i]);
//    }

//    for(int i = 0; i < normalizationAccelY.size(); ++i){
//        accelYInputVector.push_back(normalizationAccelY[i]);
//    }

//    for(int i = 0; i < normalizationAccelZ.size(); ++i){
//        accelZInputVector.push_back(normalizationAccelZ[i]);
//    }

    for(int i = 0; i < normalizationRoll.size(); ++i){
        rollInputVector.push_back(normalizationRoll[i]);
    }

    for(int i = 0; i < normalizationPitch.size(); ++i){
        pitchInputVector.push_back(normalizationPitch[i]);
    }

    for(int i = 0; i < normalizationYaw.size(); ++i){
        yawInputVector.push_back(normalizationYaw[i]);
    }
}



void GestureSensor::newPreprocessData()
{
    if(!dataSet.accelX.size()){
        return;
    }

    if(!dataSet.accelY.size()){
        return;
    }

    if(!dataSet.accelZ.size()){
        return;
    }

    if(!dataSet.gyroX.size()){
        return;
    }

    if(!dataSet.gyroY.size()){
        return;
    }

    if(!dataSet.gyroZ.size()){
        return;
    }

    //Threshold
    thresholdSensorData();

    vector<quaternion> quatVct;
    vector<quaternion> quatConjVct;

    for(int i = 0; i < dataSet.accelX.size(); ++i){
        MadgwickAHRSupdateIMU(dataSet.gyroX[i], dataSet.gyroY[i], dataSet.gyroZ[i], dataSet.accelX[i], dataSet.accelY[i], dataSet.accelZ[i]);
        float tempQ0 = q0;
        float tempQ1 = q1;
        float tempQ2 = q2;
        float tempQ3 = q3;

        cout << "Q0 : " << tempQ0 << " Q1 : " << tempQ1 << " Q2 : " << tempQ2 << " Q3 : " << tempQ3 << endl;

        quaternion quat;
        quat.q0 = tempQ0;
        quat.q1 = tempQ1;
        quat.q2 = tempQ2;
        quat.q3 = tempQ3;

        quatVct.push_back(quat);
    }

    for(int i = 0; i < dataSet.accelX.size(); ++i){
        quaternion quatConj;
        getQuaternionConjugate(quatVct[i].q0, quatVct[i].q1, quatVct[i].q2, quatVct[i].q3, quatConj);

        quatConjVct.push_back(quatConj);
    }

    vector<float> roll;
    vector<float> pitch;
    vector<float> yaw;

    for(int i = 0; i < dataSet.accelX.size(); ++i){
        euler e;
        convertQuaternionToEuler(quatConjVct[i].q0, quatConjVct[i].q1, quatConjVct[i].q2, quatConjVct[i].q3, e);
        e.first = e.first * (180.f / PI);
        e.second = e.second * (180.f / PI);
        e.third = e.third * (180.f / PI);

//        eVct.push_back(e);
        roll.push_back(e.first);
        pitch.push_back(e.second);
        yaw.push_back(e.third);

//        cout << "X : " << e.first << " Y : " << e.second << " Z : " << e.third << endl;

        //Roll, Pitch, Yaw
        cout << e.first << ", " << e.second << ", " << e.third << endl;
    }


    initializeQuaternionVariable();

    vector<float> interpolationAccelX;
    vector<float> interpolationAccelY;
    vector<float> interpolationAccelZ;
    vector<float> interpolationRoll;
    vector<float> interpolationPitch;
    vector<float> interpolationYaw;

    linearInterpolation(dataSet.accelX, interpolationAccelX, 256);
    linearInterpolation(dataSet.accelY, interpolationAccelY, 256);
    linearInterpolation(dataSet.accelZ, interpolationAccelZ, 256);
    linearInterpolation(roll, interpolationRoll, 256);
    linearInterpolation(pitch, interpolationPitch, 256);
    linearInterpolation(yaw, interpolationYaw, 256);


    vector<uint8_t> normalizationAccelX;
    vector<uint8_t> normalizationAccelY;
    vector<uint8_t> normalizationAccelZ;
    vector<uint8_t> normalizationRoll;
    vector<uint8_t> normalizationPitch;
    vector<uint8_t> normalizationYaw;

    normalizeData(-8.f, 8.f, interpolationAccelX, normalizationAccelX);
    normalizeData(-8.f, 8.f, interpolationAccelY, normalizationAccelY);
    normalizeData(-8.f, 8.f, interpolationAccelZ, normalizationAccelZ);
    normalizeData(-180.f, 180.f, interpolationRoll, normalizationRoll);
    normalizeData(-90.f, 90.f, interpolationPitch, normalizationPitch);
    normalizeData(-180.f, 180.f, interpolationYaw, normalizationYaw);

    preprocessingDataSet preproData;
    preproData.accelX = normalizationAccelX;
    preproData.accelY = normalizationAccelY;
    preproData.accelZ = normalizationAccelZ;
    preproData.roll = normalizationRoll;
    preproData.pitch = normalizationPitch;
    preproData.yaw = normalizationYaw;

    emit setChartData(preproData);

    for(int i = 0; i < normalizationAccelX.size(); ++i){
        accelXInputVector.push_back(normalizationAccelX[i]);
    }

    for(int i = 0; i < normalizationAccelY.size(); ++i){
        accelYInputVector.push_back(normalizationAccelY[i]);
    }

    for(int i = 0; i < normalizationAccelZ.size(); ++i){
        accelZInputVector.push_back(normalizationAccelZ[i]);
    }

    for(int i = 0; i < normalizationRoll.size(); ++i){
        rollInputVector.push_back(normalizationRoll[i]);
    }

    for(int i = 0; i < normalizationPitch.size(); ++i){
        pitchInputVector.push_back(normalizationPitch[i]);
    }

    for(int i = 0; i < normalizationYaw.size(); ++i){
        yawInputVector.push_back(normalizationYaw[i]);
    }
}



void GestureSensor::testPreprocessData()
{
    if(!dataSet.accelX.size()){
        return;
    }

    if(!dataSet.accelY.size()){
        return;
    }

    if(!dataSet.accelZ.size()){
        return;
    }

    if(!dataSet.gyroX.size()){
        return;
    }

    if(!dataSet.gyroY.size()){
        return;
    }

    if(!dataSet.gyroZ.size()){
        return;
    }

    //Threshold
    thresholdSensorData();

    vector<float> interpolationAccelX;
    vector<float> interpolationAccelY;
    vector<float> interpolationAccelZ;
    vector<float> interpolationGyroX;
    vector<float> interpolationGyroY;
    vector<float> interpolationGyroZ;

    linearInterpolation(dataSet.accelX, interpolationAccelX, 85);
    linearInterpolation(dataSet.accelY, interpolationAccelY, 85);
    linearInterpolation(dataSet.accelZ, interpolationAccelZ, 85);
    linearInterpolation(dataSet.gyroX, interpolationGyroX, 85);
    linearInterpolation(dataSet.gyroY, interpolationGyroY, 85);
    linearInterpolation(dataSet.gyroZ, interpolationGyroZ, 85);

    vector<quaternion> quatVct;
    vector<quaternion> quatConjVct;

    for(int i = 0; i < 85; ++i){
        MadgwickAHRSupdateIMU(interpolationGyroX[i],interpolationGyroY[i],interpolationGyroZ[i],interpolationAccelX[i],interpolationAccelY[i],interpolationAccelZ[i]);
        float tempQ0 = q0;
        float tempQ1 = q1;
        float tempQ2 = q2;
        float tempQ3 = q3;

        cout << "Q0 : " << tempQ0 << " Q1 : " << tempQ1 << " Q2 : " << tempQ2 << " Q3 : " << tempQ3 << endl;

        quaternion quat;
        quat.q0 = tempQ0;
        quat.q1 = tempQ1;
        quat.q2 = tempQ2;
        quat.q3 = tempQ3;

        quatVct.push_back(quat);
    }

    for(int i = 0; i < 85; ++i){
        quaternion quatConj;
        getQuaternionConjugate(quatVct[i].q0,quatVct[i].q1,quatVct[i].q2,quatVct[i].q3,quatConj);

        quatConjVct.push_back(quatConj);
    }

    vector<float> roll;
    vector<float> pitch;
    vector<float> yaw;

    for(int i = 0; i < 85; ++i){
        euler e;
        convertQuaternionToEuler(quatConjVct[i].q0, quatConjVct[i].q1, quatConjVct[i].q2, quatConjVct[i].q3, e);
        e.first = e.first * (180.f / PI);
        e.second = e.second * (180.f / PI);
        e.third = e.third * (180.f / PI);

//        eVct.push_back(e);
        roll.push_back(e.first);
        pitch.push_back(e.second);
        yaw.push_back(e.third);

//        cout << "X : " << e.first << " Y : " << e.second << " Z : " << e.third << endl;

        //Roll, Pitch, Yaw
        cout << e.first << ", " << e.second << ", " << e.third << endl;
    }

    initializeQuaternionVariable();

    vector<uint8_t> normalizationAccelX;
    vector<uint8_t> normalizationAccelY;
    vector<uint8_t> normalizationAccelZ;
    vector<uint8_t> normalizationRoll;
    vector<uint8_t> normalizationPitch;
    vector<uint8_t> normalizationYaw;

    normalizeData(-4.f, 4.f, interpolationAccelX, normalizationAccelX);
    normalizeData(-4.f, 4.f, interpolationAccelY, normalizationAccelY);
    normalizeData(-4.f, 4.f, interpolationAccelZ, normalizationAccelZ);
    normalizeData(-180.f, 180.f, roll, normalizationRoll);
    normalizeData(-90.f, 90.f, pitch, normalizationPitch);
    normalizeData(-180.f, 180.f, yaw, normalizationYaw);

    preprocessingDataSet preproData;
    preproData.accelX = normalizationAccelX;
    preproData.accelY = normalizationAccelY;
    preproData.accelZ = normalizationAccelZ;
    preproData.roll = normalizationRoll;
    preproData.pitch = normalizationPitch;
    preproData.yaw = normalizationYaw;

    emit setChartData(preproData);

    for(int i = 0; i < normalizationAccelX.size(); ++i){
        accelInputVector.push_back(normalizationAccelX[i]);
    }

    for(int i = 0; i < normalizationAccelY.size(); ++i){
        accelInputVector.push_back(normalizationAccelY[i]);
    }

    for(int i = 0; i < normalizationAccelZ.size(); ++i){
        accelInputVector.push_back(normalizationAccelZ[i]);
    }

    for(int i = 0; i < normalizationRoll.size(); ++i){
        rotationInputVector.push_back(normalizationRoll[i]);
    }

    for(int i = 0; i < normalizationPitch.size(); ++i){
        rotationInputVector.push_back(normalizationPitch[i]);
    }

    for(int i = 0; i < normalizationYaw.size(); ++i){
        rotationInputVector.push_back(normalizationYaw[i]);
    }
}

void GestureSensor::oldPreprocessData()
{
    if(!dataSet.accelX.size()){
        return;
    }

    if(!dataSet.accelY.size()){
        return;
    }

    if(!dataSet.accelZ.size()){
        return;
    }

    vector<float> interpolationAccelX;
    vector<float> interpolationAccelY;
    vector<float> interpolationAccelZ;
    vector<float> pitch;
    vector<float> roll;
    vector<float> yaw;

    linearInterpolation(dataSet.accelX, interpolationAccelX, 85);
    linearInterpolation(dataSet.accelY, interpolationAccelY, 85);
    linearInterpolation(dataSet.accelZ, interpolationAccelZ, 85);

    getPitchVector(interpolationAccelX, interpolationAccelY, interpolationAccelZ, pitch);
    getRollVector(interpolationAccelX, interpolationAccelY, interpolationAccelZ, roll);
    getYawVector(interpolationAccelX, interpolationAccelY, interpolationAccelZ, yaw);

    vector<uint8_t> normalizationAccelX;
    vector<uint8_t> normalizationAccelY;
    vector<uint8_t> normalizationAccelZ;
    vector<uint8_t> normalizationPitch;
    vector<uint8_t> normalizationRoll;
    vector<uint8_t> normalizationYaw;

    normalizeData(-4.f, 4.f, interpolationAccelX, normalizationAccelX);
    normalizeData(-4.f, 4.f, interpolationAccelY, normalizationAccelY);
    normalizeData(-4.f, 4.f, interpolationAccelZ, normalizationAccelZ);
    normalizeData(-90.f, 90.f, pitch, normalizationPitch);
    normalizeData(-90.f, 90.f, roll, normalizationRoll);
    normalizeData(-90.f, 90.f, yaw, normalizationYaw);

    preprocessingDataSet dataSet;
    dataSet.accelX = normalizationAccelX;
    dataSet.accelY = normalizationAccelY;
    dataSet.accelZ = normalizationAccelZ;
    dataSet.pitch = normalizationPitch;
    dataSet.roll = normalizationRoll;
    dataSet.yaw = normalizationYaw;

    emit setChartData(dataSet);

    for(int i = 0; i < normalizationAccelX.size(); ++i){
        accelInputVector.push_back(normalizationAccelX[i]);
    }

    for(int i = 0; i < normalizationAccelY.size(); ++i){
        accelInputVector.push_back(normalizationAccelY[i]);
    }

    for(int i = 0; i < normalizationAccelZ.size(); ++i){
        accelInputVector.push_back(normalizationAccelZ[i]);
    }

    for(int i = 0; i < normalizationPitch.size(); ++i){
        rotationInputVector.push_back(normalizationPitch[i]);
    }

    for(int i = 0; i < normalizationRoll.size(); ++i){
        rotationInputVector.push_back(normalizationRoll[i]);
    }

    for(int i = 0; i < normalizationYaw.size(); ++i){
        rotationInputVector.push_back(normalizationYaw[i]);
    }
}


bool GestureSensor::LearnGestureOnlyRot(uint16_t category, uint16_t minIF, uint16_t maxIF)
{
    bool result = false;

    do{

        if(!accelInputVector.size()){
            break;
        }

        if(!rotationInputVector.size()){
            break;
        }

        emit clearLearningResult();

        nm_learn_req accelLearningReq;
        memset(&accelLearningReq,0,sizeof(nm_learn_req));

        uint16_t accelVectorSize = (uint16_t)accelInputVector.size();

        for(int i = 0; i < accelVectorSize; ++i){
            accelLearningReq.vector[i] = accelInputVector[i];
        }

        accelLearningReq.vector_size = accelVectorSize;
        accelLearningReq.query_affected = 1;
        accelLearningReq.category = category;

        NMHelper::getInstance()->nmSetContext(1, minIF, maxIF);
        result = NMHelper::getInstance()->nmLearn(&accelLearningReq);

        if(result){

            if(accelLearningReq.affected_count > 0){
                for(int i = 0; i < accelLearningReq.affected_count; ++i){
                    QString result;
                    result = "[Context-1 Affected Neuron] NID : ";
                    result.append(QString::number(accelLearningReq.affected_neurons[i].nid));
                    result.append(" AIF : ");
                    result.append(QString::number(accelLearningReq.affected_neurons[i].aif));
                    result.append(" MINIF : ");
                    result.append(QString::number(accelLearningReq.affected_neurons[i].minif));

                    emit addLearningResult(result);

                    cout << result.toStdString() << endl;
                }
            }
        }
        else{
            QString result = "Context-1 Failed to learn!";

            emit addLearningResult(result);

            cout << result.toStdString() << endl;
            break;
        }

        nm_learn_req rotationLearningReq;
        memset(&rotationLearningReq,0,sizeof(nm_learn_req));

        uint16_t rotationVectorSize = (uint16_t)rotationInputVector.size();

        for(int i = 0; i < rotationVectorSize; ++i){
            rotationLearningReq.vector[i] = rotationInputVector[i];
        }

        rotationLearningReq.vector_size = rotationVectorSize;
        rotationLearningReq.query_affected = 1;
        rotationLearningReq.category = category;

        NMHelper::getInstance()->nmSetContext(2, minIF, maxIF);
        result = NMHelper::getInstance()->nmLearn(&rotationLearningReq);

        if(result){
            if(rotationLearningReq.affected_count > 0){
                for(int i = 0; i < rotationLearningReq.affected_count; ++i){
                    QString result;
                    result = "[Context-2 Affected Neuron] NID : ";
                    result.append(QString::number(rotationLearningReq.affected_neurons[i].nid));
                    result.append(" AIF : ");
                    result.append(QString::number(rotationLearningReq.affected_neurons[i].aif));
                    result.append(" MINIF : ");
                    result.append(QString::number(rotationLearningReq.affected_neurons[i].minif));

                    emit addLearningResult(result);
                    cout << result.toStdString() << endl;
                }
            }
        }
        else{
            QString result = "Context-2 Failed to learn!";

            emit addLearningResult(result);
            cout << result.toStdString() << endl;
            break;
        }

        uint32_t nCount = 0;
        NMHelper::getInstance()->nmGetNeuronCount(nCount);

        QString result = "Total Neuron Count : ";
        result.append(QString::number(nCount));

        emit addLearningResult(result);

        cout << result.toStdString() << endl;

    }while(false);

    return result;
}


uint16_t GestureSensor::recognizeGestureOnlyRot(uint16_t minIF, uint16_t maxIF)
{
    uint16_t category = 0xffff;

    do{

        if(!accelInputVector.size()){
            break;
        }

        if(!rotationInputVector.size()){
            break;
        }

        nm_classify_req accelClassify;
        memset(&accelClassify, 0, sizeof(nm_classify_req));

        nm_classify_req rotationClassify;
        memset(&rotationClassify, 0, sizeof(nm_classify_req));

        uint16_t accelVectorSize = (uint16_t)accelInputVector.size();
        uint16_t rotationVectorSize = (uint16_t)rotationInputVector.size();

        for(int i = 0; i < accelVectorSize; ++i){
            accelClassify.vector[i] = accelInputVector[i];
        }
        for(int i = 0; i < rotationVectorSize; ++i){
            rotationClassify.vector[i] = rotationInputVector[i];
        }

        accelClassify.vector_size = accelVectorSize;
        accelClassify.k = 9;

        rotationClassify.vector_size = rotationVectorSize;
        rotationClassify.k = 9;

        NMHelper::getInstance()->nmSetContext(1, minIF, maxIF);
        NMHelper::getInstance()->nmClassify(&accelClassify);

        NMHelper::getInstance()->nmSetContext(2, minIF, maxIF);
        NMHelper::getInstance()->nmClassify(&rotationClassify);

        category = judgeCategory(accelClassify.category[0], accelClassify.distance[0], rotationClassify.category[0], rotationClassify.distance[0]);

        if(category == 0xffff){
            cout << "Unknown Category. Failed to recognition." << endl;
        }
        else{
            cout << "Recognition result category : " << category << endl;
        }

    }while(false);

    return category;
}

bool GestureSensor::LearnGesture3(uint16_t category, uint16_t minIF, uint16_t maxIF)
{
    bool result = false;

    do{

        if(!rollInputVector.size()){
            break;
        }

        if(!pitchInputVector.size()){
            break;
        }

        if(!yawInputVector.size()){
            break;
        }

        emit clearLearningResult();

        nm_learn_req rollLearningReq;
        nm_learn_req pitchLearningReq;
        nm_learn_req yawLearningReq;

        memset(&rollLearningReq, 0, sizeof(nm_learn_req));
        memset(&pitchLearningReq, 0, sizeof(nm_learn_req));
        memset(&yawLearningReq, 0, sizeof(nm_learn_req));

        uint16_t accelVectorSize = (uint16_t)rollInputVector.size();

        for(int i = 0; i < accelVectorSize; ++i){
            rollLearningReq.vector[i] = rollInputVector[i];
            pitchLearningReq.vector[i] = pitchInputVector[i];
            yawLearningReq.vector[i] = yawInputVector[i];
        }

        rollLearningReq.vector_size = accelVectorSize;
        rollLearningReq.query_affected = 1;
        rollLearningReq.category = category;

        pitchLearningReq.vector_size = accelVectorSize;
        pitchLearningReq.query_affected = 1;
        pitchLearningReq.category = category;

        yawLearningReq.vector_size = accelVectorSize;
        yawLearningReq.query_affected = 1;
        yawLearningReq.category = category;

        NMHelper::getInstance()->nmSetContext(1, minIF, maxIF);
        result = NMHelper::getInstance()->nmLearn(&rollLearningReq);

        if(result){

            if(rollLearningReq.affected_count > 0){
                for(int i = 0; i < rollLearningReq.affected_count; ++i){
                    QString result;
                    result = "[Context-1 Affected Neuron] NID : ";
                    result.append(QString::number(rollLearningReq.affected_neurons[i].nid));
                    result.append(" AIF : ");
                    result.append(QString::number(rollLearningReq.affected_neurons[i].aif));
                    result.append(" MINIF : ");
                    result.append(QString::number(rollLearningReq.affected_neurons[i].minif));

                    emit addLearningResult(result);

                    cout << result.toStdString() << endl;
                }
            }
        }
        else{
            QString result = "Context-1 Failed to learn!";

            emit addLearningResult(result);

            cout << result.toStdString() << endl;
            break;
        }

        NMHelper::getInstance()->nmSetContext(2, minIF, maxIF);
        result = NMHelper::getInstance()->nmLearn(&pitchLearningReq);

        if(result){

            if(pitchLearningReq.affected_count > 0){
                for(int i = 0; i < pitchLearningReq.affected_count; ++i){
                    QString result;
                    result = "[Context-2 Affected Neuron] NID : ";
                    result.append(QString::number(pitchLearningReq.affected_neurons[i].nid));
                    result.append(" AIF : ");
                    result.append(QString::number(pitchLearningReq.affected_neurons[i].aif));
                    result.append(" MINIF : ");
                    result.append(QString::number(pitchLearningReq.affected_neurons[i].minif));

                    emit addLearningResult(result);

                    cout << result.toStdString() << endl;
                }
            }
        }
        else{
            QString result = "Context-2 Failed to learn!";

            emit addLearningResult(result);

            cout << result.toStdString() << endl;
            break;
        }

        NMHelper::getInstance()->nmSetContext(3, minIF, maxIF);
        result = NMHelper::getInstance()->nmLearn(&yawLearningReq);

        if(result){

            if(yawLearningReq.affected_count > 0){
                for(int i = 0; i < yawLearningReq.affected_count; ++i){
                    QString result;
                    result = "[Context-3 Affected Neuron] NID : ";
                    result.append(QString::number(yawLearningReq.affected_neurons[i].nid));
                    result.append(" AIF : ");
                    result.append(QString::number(yawLearningReq.affected_neurons[i].aif));
                    result.append(" MINIF : ");
                    result.append(QString::number(yawLearningReq.affected_neurons[i].minif));

                    emit addLearningResult(result);

                    cout << result.toStdString() << endl;
                }
            }
        }
        else{
            QString result = "Context-3 Failed to learn!";

            emit addLearningResult(result);

            cout << result.toStdString() << endl;
            break;
        }

        uint32_t nCount = 0;
        NMHelper::getInstance()->nmGetNeuronCount(nCount);

        QString result = "Total Neuron Count : ";
        result.append(QString::number(nCount));

        emit addLearningResult(result);

        cout << result.toStdString() << endl;

    }while(false);

    return result;
}

uint16_t GestureSensor::recognizeGesture3(uint16_t minIF, uint16_t maxIF)
{
    uint16_t category = 0xffff;

    do{

        if(!rollInputVector.size()){
            break;
        }

        if(!pitchInputVector.size()){
            break;
        }

        if(!yawInputVector.size()){
            break;
        }

        nm_classify_req rollClassify;
        nm_classify_req pitchClassify;
        nm_classify_req yawClassify;

        memset(&rollClassify, 0, sizeof(nm_classify_req));
        memset(&pitchClassify, 0, sizeof(nm_classify_req));
        memset(&yawClassify, 0, sizeof(nm_classify_req));

        uint16_t vectorSize = (uint16_t)rollInputVector.size();

        for(int i = 0; i < vectorSize; ++i){
            rollClassify.vector[i] = rollInputVector[i];
        }

        for(int i = 0; i < vectorSize; ++i){
            pitchClassify.vector[i] = pitchInputVector[i];
        }

        for(int i = 0; i < vectorSize; ++i){
            yawClassify.vector[i] = yawInputVector[i];
        }

        rollClassify.vector_size = vectorSize;
        rollClassify.k = 9;
        pitchClassify.vector_size = vectorSize;
        pitchClassify.k = 9;
        yawClassify.vector_size = vectorSize;
        yawClassify.k = 9;

        NMHelper::getInstance()->nmSetContext(1, minIF, maxIF);
        NMHelper::getInstance()->nmClassify(&rollClassify);

        NMHelper::getInstance()->nmSetContext(2, minIF, maxIF);
        NMHelper::getInstance()->nmClassify(&pitchClassify);

        NMHelper::getInstance()->nmSetContext(3, minIF, maxIF);
        NMHelper::getInstance()->nmClassify(&yawClassify);

        //please add version
//        category = judgeCategory(accelClassify.category[0], accelClassify.distance[0], rotationClassify.category[0], rotationClassify.distance[0]);
        classifyStruct cst;
        cst.rollClassify = rollClassify;
        cst.pitchClassify = pitchClassify;
        cst.yawClassify = yawClassify;

        category = judgeCategory3(cst);

        if(category == 0xffff){
            cout << "Unknown Category. Failed to recognition." << endl;
        }
        else{
            cout << "Recognition result category : " << category << endl;
        }

    }while(false);

    return category;
}


bool GestureSensor::LearnGesture2(uint16_t category, uint16_t minIF, uint16_t maxIF)
{
    bool result = false;

    do{

        if(!accelXInputVector.size()){
            break;
        }

        if(!accelYInputVector.size()){
            break;
        }

        if(!accelZInputVector.size()){
            break;
        }

        if(!rollInputVector.size()){
            break;
        }

        if(!pitchInputVector.size()){
            break;
        }

        if(!yawInputVector.size()){
            break;
        }

        emit clearLearningResult();

        nm_learn_req accelXLearningReq;
        nm_learn_req accelYLearningReq;
        nm_learn_req accelZLearningReq;
        nm_learn_req rollLearningReq;
        nm_learn_req pitchLearningReq;
        nm_learn_req yawLearningReq;

        memset(&accelXLearningReq, 0, sizeof(nm_learn_req));
        memset(&accelYLearningReq, 0, sizeof(nm_learn_req));
        memset(&accelZLearningReq, 0, sizeof(nm_learn_req));
        memset(&rollLearningReq, 0, sizeof(nm_learn_req));
        memset(&pitchLearningReq, 0, sizeof(nm_learn_req));
        memset(&yawLearningReq, 0, sizeof(nm_learn_req));

        uint16_t accelVectorSize = (uint16_t)accelXInputVector.size();

        for(int i = 0; i < accelVectorSize; ++i){
            accelXLearningReq.vector[i] = accelXInputVector[i];
            accelYLearningReq.vector[i] = accelYInputVector[i];
            accelZLearningReq.vector[i] = accelZInputVector[i];
            rollLearningReq.vector[i] = rollInputVector[i];
            pitchLearningReq.vector[i] = pitchInputVector[i];
            yawLearningReq.vector[i] = yawInputVector[i];
        }

        accelXLearningReq.vector_size = accelVectorSize;
        accelXLearningReq.query_affected = 1;
        accelXLearningReq.category = category;

        accelYLearningReq.vector_size = accelVectorSize;
        accelYLearningReq.query_affected = 1;
        accelYLearningReq.category = category;

        accelZLearningReq.vector_size = accelVectorSize;
        accelZLearningReq.query_affected = 1;
        accelZLearningReq.category = category;

        rollLearningReq.vector_size = accelVectorSize;
        rollLearningReq.query_affected = 1;
        rollLearningReq.category = category;

        pitchLearningReq.vector_size = accelVectorSize;
        pitchLearningReq.query_affected = 1;
        pitchLearningReq.category = category;

        yawLearningReq.vector_size = accelVectorSize;
        yawLearningReq.query_affected = 1;
        yawLearningReq.category = category;

        NMHelper::getInstance()->nmSetContext(1, minIF, maxIF);
        result = NMHelper::getInstance()->nmLearn(&accelXLearningReq);

        if(result){

            if(accelXLearningReq.affected_count > 0){
                for(int i = 0; i < accelXLearningReq.affected_count; ++i){
                    QString result;
                    result = "[Context-1 Affected Neuron] NID : ";
                    result.append(QString::number(accelXLearningReq.affected_neurons[i].nid));
                    result.append(" AIF : ");
                    result.append(QString::number(accelXLearningReq.affected_neurons[i].aif));
                    result.append(" MINIF : ");
                    result.append(QString::number(accelXLearningReq.affected_neurons[i].minif));

                    emit addLearningResult(result);

                    cout << result.toStdString() << endl;
                }
            }
        }
        else{
            QString result = "Context-1 Failed to learn!";

            emit addLearningResult(result);

            cout << result.toStdString() << endl;
            break;
        }

        NMHelper::getInstance()->nmSetContext(2, minIF, maxIF);
        result = NMHelper::getInstance()->nmLearn(&accelYLearningReq);

        if(result){

            if(accelYLearningReq.affected_count > 0){
                for(int i = 0; i < accelYLearningReq.affected_count; ++i){
                    QString result;
                    result = "[Context-2 Affected Neuron] NID : ";
                    result.append(QString::number(accelYLearningReq.affected_neurons[i].nid));
                    result.append(" AIF : ");
                    result.append(QString::number(accelYLearningReq.affected_neurons[i].aif));
                    result.append(" MINIF : ");
                    result.append(QString::number(accelYLearningReq.affected_neurons[i].minif));

                    emit addLearningResult(result);

                    cout << result.toStdString() << endl;
                }
            }
        }
        else{
            QString result = "Context-2 Failed to learn!";

            emit addLearningResult(result);

            cout << result.toStdString() << endl;
            break;
        }

        NMHelper::getInstance()->nmSetContext(3, minIF, maxIF);
        result = NMHelper::getInstance()->nmLearn(&accelZLearningReq);

        if(result){

            if(accelZLearningReq.affected_count > 0){
                for(int i = 0; i < accelZLearningReq.affected_count; ++i){
                    QString result;
                    result = "[Context-3 Affected Neuron] NID : ";
                    result.append(QString::number(accelZLearningReq.affected_neurons[i].nid));
                    result.append(" AIF : ");
                    result.append(QString::number(accelZLearningReq.affected_neurons[i].aif));
                    result.append(" MINIF : ");
                    result.append(QString::number(accelZLearningReq.affected_neurons[i].minif));

                    emit addLearningResult(result);

                    cout << result.toStdString() << endl;
                }
            }
        }
        else{
            QString result = "Context-3 Failed to learn!";

            emit addLearningResult(result);

            cout << result.toStdString() << endl;
            break;
        }

        NMHelper::getInstance()->nmSetContext(4, minIF, maxIF);
        result = NMHelper::getInstance()->nmLearn(&rollLearningReq);

        if(result){

            if(rollLearningReq.affected_count > 0){
                for(int i = 0; i < rollLearningReq.affected_count; ++i){
                    QString result;
                    result = "[Context-4 Affected Neuron] NID : ";
                    result.append(QString::number(rollLearningReq.affected_neurons[i].nid));
                    result.append(" AIF : ");
                    result.append(QString::number(rollLearningReq.affected_neurons[i].aif));
                    result.append(" MINIF : ");
                    result.append(QString::number(rollLearningReq.affected_neurons[i].minif));

                    emit addLearningResult(result);

                    cout << result.toStdString() << endl;
                }
            }
        }
        else{
            QString result = "Context-4 Failed to learn!";

            emit addLearningResult(result);

            cout << result.toStdString() << endl;
            break;
        }

        NMHelper::getInstance()->nmSetContext(5, minIF, maxIF);
        result = NMHelper::getInstance()->nmLearn(&pitchLearningReq);

        if(result){

            if(pitchLearningReq.affected_count > 0){
                for(int i = 0; i < pitchLearningReq.affected_count; ++i){
                    QString result;
                    result = "[Context-5 Affected Neuron] NID : ";
                    result.append(QString::number(pitchLearningReq.affected_neurons[i].nid));
                    result.append(" AIF : ");
                    result.append(QString::number(pitchLearningReq.affected_neurons[i].aif));
                    result.append(" MINIF : ");
                    result.append(QString::number(pitchLearningReq.affected_neurons[i].minif));

                    emit addLearningResult(result);

                    cout << result.toStdString() << endl;
                }
            }
        }
        else{
            QString result = "Context-5 Failed to learn!";

            emit addLearningResult(result);

            cout << result.toStdString() << endl;
            break;
        }

        NMHelper::getInstance()->nmSetContext(6, minIF, maxIF);
        result = NMHelper::getInstance()->nmLearn(&yawLearningReq);

        if(result){

            if(yawLearningReq.affected_count > 0){
                for(int i = 0; i < yawLearningReq.affected_count; ++i){
                    QString result;
                    result = "[Context-6 Affected Neuron] NID : ";
                    result.append(QString::number(yawLearningReq.affected_neurons[i].nid));
                    result.append(" AIF : ");
                    result.append(QString::number(yawLearningReq.affected_neurons[i].aif));
                    result.append(" MINIF : ");
                    result.append(QString::number(yawLearningReq.affected_neurons[i].minif));

                    emit addLearningResult(result);

                    cout << result.toStdString() << endl;
                }
            }
        }
        else{
            QString result = "Context-6 Failed to learn!";

            emit addLearningResult(result);

            cout << result.toStdString() << endl;
            break;
        }

        uint32_t nCount = 0;
        NMHelper::getInstance()->nmGetNeuronCount(nCount);

        QString result = "Total Neuron Count : ";
        result.append(QString::number(nCount));

        emit addLearningResult(result);

        cout << result.toStdString() << endl;

    }while(false);

    return result;
}

uint16_t GestureSensor::recognizeGesture2(uint16_t minIF, uint16_t maxIF)
{
    uint16_t category = 0xffff;

    do{

        if(!accelXInputVector.size()){
            break;
        }

        if(!accelYInputVector.size()){
            break;
        }

        if(!accelZInputVector.size()){
            break;
        }

        if(!rollInputVector.size()){
            break;
        }

        if(!pitchInputVector.size()){
            break;
        }

        if(!yawInputVector.size()){
            break;
        }

        nm_classify_req accelXClassify;
        nm_classify_req accelYClassify;
        nm_classify_req accelZClassify;
        nm_classify_req rollClassify;
        nm_classify_req pitchClassify;
        nm_classify_req yawClassify;

        memset(&accelXClassify, 0, sizeof(nm_classify_req));
        memset(&accelYClassify, 0, sizeof(nm_classify_req));
        memset(&accelZClassify, 0, sizeof(nm_classify_req));
        memset(&rollClassify, 0, sizeof(nm_classify_req));
        memset(&pitchClassify, 0, sizeof(nm_classify_req));
        memset(&yawClassify, 0, sizeof(nm_classify_req));

        uint16_t vectorSize = (uint16_t)accelXInputVector.size();

        for(int i = 0; i < vectorSize; ++i){
            accelXClassify.vector[i] = accelXInputVector[i];
        }

        for(int i = 0; i < vectorSize; ++i){
            accelYClassify.vector[i] = accelYInputVector[i];
        }

        for(int i = 0; i < vectorSize; ++i){
            accelZClassify.vector[i] = accelZInputVector[i];
        }

        for(int i = 0; i < vectorSize; ++i){
            rollClassify.vector[i] = rollInputVector[i];
        }

        for(int i = 0; i < vectorSize; ++i){
            pitchClassify.vector[i] = pitchInputVector[i];
        }

        for(int i = 0; i < vectorSize; ++i){
            yawClassify.vector[i] = yawInputVector[i];
        }

        accelXClassify.vector_size = vectorSize;
        accelXClassify.k = 9;
        accelYClassify.vector_size = vectorSize;
        accelYClassify.k = 9;
        accelZClassify.vector_size = vectorSize;
        accelZClassify.k = 9;
        rollClassify.vector_size = vectorSize;
        rollClassify.k = 9;
        pitchClassify.vector_size = vectorSize;
        pitchClassify.k = 9;
        yawClassify.vector_size = vectorSize;
        yawClassify.k = 9;

        NMHelper::getInstance()->nmSetContext(1, minIF, maxIF);
        NMHelper::getInstance()->nmClassify(&accelXClassify);

        NMHelper::getInstance()->nmSetContext(2, minIF, maxIF);
        NMHelper::getInstance()->nmClassify(&accelYClassify);

        NMHelper::getInstance()->nmSetContext(3, minIF, maxIF);
        NMHelper::getInstance()->nmClassify(&accelZClassify);

        NMHelper::getInstance()->nmSetContext(4, minIF, maxIF);
        NMHelper::getInstance()->nmClassify(&rollClassify);

        NMHelper::getInstance()->nmSetContext(5, minIF, maxIF);
        NMHelper::getInstance()->nmClassify(&pitchClassify);

        NMHelper::getInstance()->nmSetContext(6, minIF, maxIF);
        NMHelper::getInstance()->nmClassify(&yawClassify);

        //please add version
//        category = judgeCategory(accelClassify.category[0], accelClassify.distance[0], rotationClassify.category[0], rotationClassify.distance[0]);
        classifyStruct cst;
        cst.accelXClassify = accelXClassify;
        cst.accelYClassify = accelYClassify;
        cst.accelZClassify = accelZClassify;
        cst.rollClassify = rollClassify;
        cst.pitchClassify = pitchClassify;
        cst.yawClassify = yawClassify;

        category = judgeCategory2(cst);

        if(category == 0xffff){
            cout << "Unknown Category. Failed to recognition." << endl;
        }
        else{
            cout << "Recognition result category : " << category << endl;
        }

    }while(false);

    return category;
}



bool GestureSensor::LearnGesture(uint16_t category, uint16_t minIF, uint16_t maxIF)
{
    bool result = false;

    do{

        if(!accelInputVector.size()){
            break;
        }

        if(!rotationInputVector.size()){
            break;
        }

        emit clearLearningResult();

        nm_learn_req accelLearningReq;
        memset(&accelLearningReq,0,sizeof(nm_learn_req));

        uint16_t accelVectorSize = (uint16_t)accelInputVector.size();

        for(int i = 0; i < accelVectorSize; ++i){
            accelLearningReq.vector[i] = accelInputVector[i];
        }

        accelLearningReq.vector_size = accelVectorSize;
        accelLearningReq.query_affected = 1;
        accelLearningReq.category = category;

        NMHelper::getInstance()->nmSetContext(1, minIF, maxIF);
        result = NMHelper::getInstance()->nmLearn(&accelLearningReq);

        if(result){

            if(accelLearningReq.affected_count > 0){
                for(int i = 0; i < accelLearningReq.affected_count; ++i){
                    QString result;
                    result = "[Context-1 Affected Neuron] NID : ";
                    result.append(QString::number(accelLearningReq.affected_neurons[i].nid));
                    result.append(" AIF : ");
                    result.append(QString::number(accelLearningReq.affected_neurons[i].aif));
                    result.append(" MINIF : ");
                    result.append(QString::number(accelLearningReq.affected_neurons[i].minif));

                    emit addLearningResult(result);

                    cout << result.toStdString() << endl;
                }
            }
        }
        else{
            QString result = "Context-1 Failed to learn!";

            emit addLearningResult(result);

            cout << result.toStdString() << endl;
            break;
        }

        nm_learn_req rotationLearningReq;
        memset(&rotationLearningReq,0,sizeof(nm_learn_req));

        uint16_t rotationVectorSize = (uint16_t)rotationInputVector.size();

        for(int i = 0; i < rotationVectorSize; ++i){
            rotationLearningReq.vector[i] = rotationInputVector[i];
        }

        rotationLearningReq.vector_size = rotationVectorSize;
        rotationLearningReq.query_affected = 1;
        rotationLearningReq.category = category;

        NMHelper::getInstance()->nmSetContext(2, minIF, maxIF);
        result = NMHelper::getInstance()->nmLearn(&rotationLearningReq);

        if(result){
            if(rotationLearningReq.affected_count > 0){
                for(int i = 0; i < rotationLearningReq.affected_count; ++i){
                    QString result;
                    result = "[Context-2 Affected Neuron] NID : ";
                    result.append(QString::number(rotationLearningReq.affected_neurons[i].nid));
                    result.append(" AIF : ");
                    result.append(QString::number(rotationLearningReq.affected_neurons[i].aif));
                    result.append(" MINIF : ");
                    result.append(QString::number(rotationLearningReq.affected_neurons[i].minif));

                    emit addLearningResult(result);
                    cout << result.toStdString() << endl;
                }
            }
        }
        else{
            QString result = "Context-2 Failed to learn!";

            emit addLearningResult(result);
            cout << result.toStdString() << endl;
            break;
        }

        uint32_t nCount = 0;
        NMHelper::getInstance()->nmGetNeuronCount(nCount);

        QString result = "Total Neuron Count : ";
        result.append(QString::number(nCount));

        emit addLearningResult(result);

        cout << result.toStdString() << endl;

    }while(false);

    return result;
}

uint16_t GestureSensor::recognizeGesture(uint16_t minIF, uint16_t maxIF)
{
    uint16_t category = 0xffff;

    do{

        if(!accelInputVector.size()){
            break;
        }

        if(!rotationInputVector.size()){
            break;
        }

        nm_classify_req accelClassify;
        memset(&accelClassify, 0, sizeof(nm_classify_req));

        nm_classify_req rotationClassify;
        memset(&rotationClassify, 0, sizeof(nm_classify_req));

        uint16_t accelVectorSize = (uint16_t)accelInputVector.size();
        uint16_t rotationVectorSize = (uint16_t)rotationInputVector.size();

        for(int i = 0; i < accelVectorSize; ++i){
            accelClassify.vector[i] = accelInputVector[i];
        }
        for(int i = 0; i < rotationVectorSize; ++i){
            rotationClassify.vector[i] = rotationInputVector[i];
        }

        accelClassify.vector_size = accelVectorSize;
        accelClassify.k = 9;

        rotationClassify.vector_size = rotationVectorSize;
        rotationClassify.k = 9;

        NMHelper::getInstance()->nmSetContext(1, minIF, maxIF);
        NMHelper::getInstance()->nmClassify(&accelClassify);

        NMHelper::getInstance()->nmSetContext(2, minIF, maxIF);
        NMHelper::getInstance()->nmClassify(&rotationClassify);

        category = judgeCategory(accelClassify.category[0], accelClassify.distance[0], rotationClassify.category[0], rotationClassify.distance[0]);

        if(category == 0xffff){
            cout << "Unknown Category. Failed to recognition." << endl;
        }
        else{
            cout << "Recognition result category : " << category << endl;
        }

    }while(false);

    return category;
}

void GestureSensor::thresholdSensorData()
{
    int size = dataSet.accelX.size();

    for(int i = 0; i < size;){
        if( (dataSet.accelX[i] < ACCEL_X_THRESHOLD && dataSet.accelX[i] > -ACCEL_X_THRESHOLD)&&
            (dataSet.accelY[i] < ACCEL_Y_THRESHOLD && dataSet.accelY[i] > -ACCEL_Y_THRESHOLD)&&
            (dataSet.accelZ[i] < ACCEL_Z_THRESHOLD && dataSet.accelZ[i] > -ACCEL_Z_THRESHOLD)&&
            (dataSet.gyroX[i] < GYRO_X_THRESHOLD && dataSet.gyroX[i] > -GYRO_X_THRESHOLD)&&
            (dataSet.gyroY[i] < GYRO_Y_THRESHOLD && dataSet.gyroY[i] > -GYRO_Y_THRESHOLD)&&
                (dataSet.gyroZ[i] < GYRO_Z_THRESHOLD && dataSet.gyroZ[i] > -GYRO_Z_THRESHOLD) ){
            dataSet.accelX.erase(dataSet.accelX.begin() + i);
            dataSet.accelY.erase(dataSet.accelY.begin() + i);
            dataSet.accelZ.erase(dataSet.accelZ.begin() + i);
            dataSet.gyroX.erase(dataSet.gyroX.begin() + i);
            dataSet.gyroY.erase(dataSet.gyroY.begin() + i);
            dataSet.gyroZ.erase(dataSet.gyroZ.begin() + i);
        }
        else{
            ++i;
//            dataSet.accelX[i] = dataSet.accelX[i] / 0.98f;
//            dataSet.accelY[i] = dataSet.accelY[i] / 0.98f;
//            dataSet.accelZ[i] = dataSet.accelZ[i] / 0.98f;
            dataSet.gyroX[i] = dataSet.gyroX[i] * (PI / 180.f);
            dataSet.gyroY[i] = dataSet.gyroY[i] * (PI / 180.f);
            dataSet.gyroZ[i] = dataSet.gyroZ[i] * (PI / 180.f);
        }
    }
}

void GestureSensor::normalizeData(float oldMin, float oldMax, vector<float> data, vector<uint8_t>& result)
{
    float oldRange = oldMax - oldMin;
    float newRange = 255.f;

    for(int i = 0; i < data.size(); ++i){
        uint8_t newVal = 0;

        float tempVal = (((data[i] - oldMin) * newRange) / oldRange);
        newVal = (uint8_t)tempVal;

        result.push_back(newVal);
    }
}

void GestureSensor::linearInterpolation(vector<float> srcData, vector<float> &dstData, int number)
{
    const int size = (int)srcData.size();

//    if(size < 2){
//        return;
//    }
    if(size == 1){
        for(int i = 0; i < number; ++i){
            dstData.push_back(srcData[0]);
        }
        return;
    }

    dstData.reserve(number);

    for(int i = 0; i < srcData.size(); ++i){
        dstData.push_back(srcData[i]);
    }

    //data add interpolation
    if(size < number){
        size_t beforeIndex = 0;
        size_t nextIndex = 1;

        while(dstData.size() < number){

            float temp = (dstData[beforeIndex] + dstData[nextIndex]) / 2.f;

            auto iter = dstData.begin() + nextIndex;
            dstData.insert(iter, temp);

            ++nextIndex;

            if(nextIndex == dstData.size()-1){
                //1-cycle complete
                nextIndex = 1;
                beforeIndex = 0;
            }
            else{
                beforeIndex = nextIndex;
                nextIndex = beforeIndex + 1;
            }
        }
    }
    //data remove interpolation
    else{
        size_t beforeIndex = 0;
        size_t nextIndex = 2;

        while(dstData.size() > number){

            uint16_t eraseIndex = (beforeIndex + nextIndex) / 2;

            auto iter = dstData.begin() + eraseIndex;
            dstData.erase(iter);

            ++nextIndex;

            if(nextIndex >= dstData.size()){
                //1-cycle complete
                nextIndex = 2;
                beforeIndex = 0;
            }
            else{
                ++nextIndex;
                ++beforeIndex;
            }
        }
    }
}


uint16_t GestureSensor::judgeCategory3(classifyStruct cst)
{
    uint16_t resultCategory = 0;

    map<uint16_t,int> categoryFrequency;
    map<uint16_t,uint32_t> categoryDistance;

    int size = 0;

    if(cst.rollClassify.matched_count > 0){
        ++size;

        if(categoryFrequency.count(cst.rollClassify.category[0]) == 0){
            categoryFrequency.insert(make_pair(cst.rollClassify.category[0],1));
        }
        else{
            categoryFrequency[cst.rollClassify.category[0]] = ++(categoryFrequency.find(cst.rollClassify.category[0])->second);
        }

        if(categoryDistance.count(cst.rollClassify.category[0]) == 0){
            categoryDistance.insert(make_pair(cst.rollClassify.category[0], cst.rollClassify.distance[0]));
        }
        else{
            categoryDistance[cst.rollClassify.category[0]] += categoryDistance.find(cst.rollClassify.category[0])->second;
        }
    }

    if(cst.pitchClassify.matched_count > 0){
        ++size;

        if(categoryFrequency.count(cst.pitchClassify.category[0]) == 0){
            categoryFrequency.insert(make_pair(cst.pitchClassify.category[0],1));
        }
        else{
            categoryFrequency[cst.pitchClassify.category[0]] = ++(categoryFrequency.find(cst.pitchClassify.category[0])->second);
        }

        if(categoryDistance.count(cst.pitchClassify.category[0]) == 0){
            categoryDistance.insert(make_pair(cst.pitchClassify.category[0], cst.pitchClassify.distance[0]));
        }
        else{
            categoryDistance[cst.pitchClassify.category[0]] += categoryDistance.find(cst.pitchClassify.category[0])->second;
        }
    }

    if(cst.yawClassify.matched_count > 0){
        ++size;

        if(categoryFrequency.count(cst.yawClassify.category[0]) == 0){
            categoryFrequency.insert(make_pair(cst.yawClassify.category[0],1));
        }
        else{
            categoryFrequency[cst.yawClassify.category[0]] = ++(categoryFrequency.find(cst.yawClassify.category[0])->second);
        }

        if(categoryDistance.count(cst.yawClassify.category[0]) == 0){
            categoryDistance.insert(make_pair(cst.yawClassify.category[0], cst.yawClassify.distance[0]));
        }
        else{
            categoryDistance[cst.yawClassify.category[0]] += categoryDistance.find(cst.yawClassify.category[0])->second;
        }
    }

    if(size < 2){
        resultCategory = 0xffff;
        return resultCategory;
    }

    int max = 0;
    vector<uint16_t> catVct;

    for(auto iter = categoryFrequency.begin(); iter != categoryFrequency.end(); ++iter){
        if(max < iter->second){
            max = iter->second;
        }
    }

    for(auto iter = categoryFrequency.begin(); iter != categoryFrequency.end(); ++iter){
        if(max == iter->second){
            catVct.push_back(iter->first);
        }
    }

    if(!catVct.size()){
        resultCategory = 0xffff;
    }
    else if(catVct.size() == 1){
        resultCategory = catVct[0];
    }
    else{
        uint32_t min = INT_MAX;

        for(int i = 0; i < catVct.size(); ++i){
            if(categoryDistance[catVct[i]] < min){
                min = categoryDistance[catVct[i]];
                resultCategory = catVct[i];
            }
        }
    }

    return resultCategory;
}


uint16_t GestureSensor::judgeCategory2(classifyStruct cst)
{
    uint16_t resultCategory = 0;

    map<uint16_t,int> categoryFrequency;
    map<uint16_t,uint32_t> categoryDistance;

    if(cst.accelXClassify.matched_count > 0){
        if(categoryFrequency.count(cst.accelXClassify.category[0]) == 0){
            categoryFrequency.insert(make_pair(cst.accelXClassify.category[0],1));
        }
        else{
            categoryFrequency[cst.accelXClassify.category[0]] = ++(categoryFrequency.find(cst.accelXClassify.category[0])->second);
        }

        if(categoryDistance.count(cst.accelXClassify.category[0]) == 0){
            categoryDistance.insert(make_pair(cst.accelXClassify.category[0], cst.accelXClassify.distance[0]));
        }
        else{
            categoryDistance[cst.accelXClassify.category[0]] += categoryDistance.find(cst.accelXClassify.category[0])->second;
        }
    }

    if(cst.accelYClassify.matched_count > 0){
        if(categoryFrequency.count(cst.accelYClassify.category[0]) == 0){
            categoryFrequency.insert(make_pair(cst.accelYClassify.category[0],1));
        }
        else{
            categoryFrequency[cst.accelYClassify.category[0]] = ++(categoryFrequency.find(cst.accelYClassify.category[0])->second);
        }

        if(categoryDistance.count(cst.accelYClassify.category[0]) == 0){
            categoryDistance.insert(make_pair(cst.accelYClassify.category[0], cst.accelYClassify.distance[0]));
        }
        else{
            categoryDistance[cst.accelYClassify.category[0]] += categoryDistance.find(cst.accelYClassify.category[0])->second;
        }
    }

    if(cst.accelZClassify.matched_count > 0){
        if(categoryFrequency.count(cst.accelZClassify.category[0]) == 0){
            categoryFrequency.insert(make_pair(cst.accelZClassify.category[0],1));
        }
        else{
            categoryFrequency[cst.accelZClassify.category[0]] = ++(categoryFrequency.find(cst.accelZClassify.category[0])->second);
        }

        if(categoryDistance.count(cst.accelZClassify.category[0]) == 0){
            categoryDistance.insert(make_pair(cst.accelZClassify.category[0], cst.accelZClassify.distance[0]));
        }
        else{
            categoryDistance[cst.accelZClassify.category[0]] += categoryDistance.find(cst.accelZClassify.category[0])->second;
        }
    }

    if(cst.rollClassify.matched_count > 0){
        if(categoryFrequency.count(cst.rollClassify.category[0]) == 0){
            categoryFrequency.insert(make_pair(cst.rollClassify.category[0],1));
        }
        else{
            categoryFrequency[cst.rollClassify.category[0]] = ++(categoryFrequency.find(cst.rollClassify.category[0])->second);
        }

        if(categoryDistance.count(cst.rollClassify.category[0]) == 0){
            categoryDistance.insert(make_pair(cst.rollClassify.category[0], cst.rollClassify.distance[0]));
        }
        else{
            categoryDistance[cst.rollClassify.category[0]] += categoryDistance.find(cst.rollClassify.category[0])->second;
        }
    }

    if(cst.pitchClassify.matched_count > 0){
        if(categoryFrequency.count(cst.pitchClassify.category[0]) == 0){
            categoryFrequency.insert(make_pair(cst.pitchClassify.category[0],1));
        }
        else{
            categoryFrequency[cst.pitchClassify.category[0]] = ++(categoryFrequency.find(cst.pitchClassify.category[0])->second);
        }

        if(categoryDistance.count(cst.pitchClassify.category[0]) == 0){
            categoryDistance.insert(make_pair(cst.pitchClassify.category[0], cst.pitchClassify.distance[0]));
        }
        else{
            categoryDistance[cst.pitchClassify.category[0]] += categoryDistance.find(cst.pitchClassify.category[0])->second;
        }
    }

    if(cst.yawClassify.matched_count > 0){
        if(categoryFrequency.count(cst.yawClassify.category[0]) == 0){
            categoryFrequency.insert(make_pair(cst.yawClassify.category[0],1));
        }
        else{
            categoryFrequency[cst.yawClassify.category[0]] = ++(categoryFrequency.find(cst.yawClassify.category[0])->second);
        }

        if(categoryDistance.count(cst.yawClassify.category[0]) == 0){
            categoryDistance.insert(make_pair(cst.yawClassify.category[0], cst.yawClassify.distance[0]));
        }
        else{
            categoryDistance[cst.yawClassify.category[0]] += categoryDistance.find(cst.yawClassify.category[0])->second;
        }
    }

    int max = 0;
    vector<uint16_t> catVct;

    for(auto iter = categoryFrequency.begin(); iter != categoryFrequency.end(); ++iter){
        if(max < iter->second){
            max = iter->second;
        }
    }

    for(auto iter = categoryFrequency.begin(); iter != categoryFrequency.end(); ++iter){
        if(max == iter->second){
            catVct.push_back(iter->first);
        }
    }

    if(!catVct.size()){
        resultCategory = 0xffff;
    }
    else if(catVct.size() == 1){
        resultCategory = catVct[0];
    }
    else{
        uint32_t min = INT_MAX;

        for(int i = 0; i < catVct.size(); ++i){
            if(categoryDistance[catVct[i]] < min){
                min = categoryDistance[catVct[i]];
                resultCategory = catVct[i];
            }
        }
    }

    return resultCategory;
}

uint16_t GestureSensor::judgeCategory(uint16_t accelCategory, uint16_t accelDistance, uint16_t rotateCategory, uint16_t rotateDistance)
{
    uint16_t resultCategory = 0;

    if(accelCategory == rotateCategory){
        resultCategory = accelCategory;
    }
    else{
        if(accelCategory != 0xffff && rotateCategory != 0xffff){
            if(accelDistance >= rotateDistance){
//                resultCategory = accelCategory;
                resultCategory = rotateCategory;
            }
            else{
//                resultCategory = rotateCategory;
                resultCategory = accelCategory;
            }
        }
        else if(accelCategory != 0xffff && rotateCategory == 0xffff){
            resultCategory = accelCategory;
        }
        else if(accelCategory == 0xffff && rotateCategory != 0xffff){
            resultCategory = rotateCategory;
        }
    }

    return resultCategory;
}


void GestureSensor::getQuaternionConjugate(float q0, float q1, float q2, float q3, quaternion &quat)
{
    quat.q0 = q0;
    quat.q1 = (-1.f) * q1;
    quat.q2 = (-1.f) * q2;
    quat.q3 = (-1.f) * q3;
}

void GestureSensor::convertQuaternionToEuler(float q0, float q1, float q2, float q3, euler &e)
{
    float temp1 = 0.f, temp2 = 0.f, temp3 = 0.f, temp4 = 0.f, temp5 = 0.f;

    temp1 = (2 * (q0 * q0)) - 1 + (2 * (q1 * q1));
    temp2 = (2 * q1 * q2) - (2 * q0 * q3);
    temp3 = (2 * q1 * q3) + (2 * q0 * q2);
    temp4 = (2 * q2 * q3) - (2 * q0 * q1);
    temp5 = (2 * (q0 * q0)) - 1 + (2 * (q3 * q3));

    e.first = atan2(temp4, temp5); //Roll
//    e.second = -atan(temp3) / sqrt(1 - (temp3 * temp3)); //Pitch
    e.second = -atan(temp3 / sqrt(1 - (temp3 * temp3))); //Pitch
    e.third = atan2(temp2, temp1); // Yaw

    //Convert conjugate
    e.first = (-1.f) * e.first;
    e.second = (-1.f) * e.second;
    e.third = (-1.f) * e.third;
}

void GestureSensor::getPitchVector(vector<float> accelX, vector<float> accelY, vector<float> accelZ, vector<float> &result)
{
    int size = accelX.size();

    for(int i = 0; i < size; ++i){
        float pitch = 180.f * atan(accelX[i] / sqrt(accelY[i] * accelY[i] + accelZ[i] * accelZ[i])) / PI;
        result.push_back(pitch);
    }
}

void GestureSensor::getRollVector(vector<float> accelX, vector<float> accelY, vector<float> accelZ, vector<float> &result)
{
    int size = accelX.size();

    for(int i = 0; i < size; ++i){
        float roll = 180.f * atan(accelY[i] / sqrt(accelX[i] * accelX[i] + accelZ[i] * accelZ[i])) / PI;
        result.push_back(roll);
    }
}

void GestureSensor::getYawVector(vector<float> accelX, vector<float> accelY, vector<float> accelZ, vector<float> &result)
{
    int size = accelX.size();

    for(int i = 0; i < size; ++i){
//        float yaw = 180.f * atan(accelZ[i] / sqrt(accelX[i] * accelX[i] + accelY[i] * accelY[i])) / PI;
        float yaw = 180.f * atan(sqrt(accelX[i] * accelX[i] + accelY[i] * accelY[i]) / accelZ[i]) / PI;
        result.push_back(yaw);
    }
}

void GestureSensor::clearAllVector()
{
    dataSet.accelX.clear();
    dataSet.accelY.clear();
    dataSet.accelZ.clear();
    dataSet.gyroX.clear();
    dataSet.gyroY.clear();
    dataSet.gyroZ.clear();

    accelInputVector.clear();
    rotationInputVector.clear();

    accelXInputVector.clear();
    accelYInputVector.clear();
    accelZInputVector.clear();
    rollInputVector.clear();
    pitchInputVector.clear();
    yawInputVector.clear();
}

static void read_gatt_char(void* context, const void* caller, const MblMwGattChar* characteristic, MblMwFnIntVoidPtrArray handler)
{
    if (characteristic->uuid_high == 0x00002a2400001000 && characteristic->uuid_low == 0x800000805f9b34fb) {
        // simulate response for reading model characteristic
        const uint8_t model[] = "5";
        handler(caller, model, 1);
    }
    else if (characteristic->uuid_high == 0x00002a2600001000 && characteristic->uuid_low == 0x800000805f9b34fb) {
        // simulate response for reading firmware characteristic
        const uint8_t firmware[] = "1.5.0";
        handler(caller, firmware, 5);

    }
    else if (characteristic->uuid_high == 0x00002a2700001000 && characteristic->uuid_low == 0x800000805f9b34fb) {
        // simulate response for reading hardware characteristic
        const uint8_t hardware[] = "0.4";
        handler(caller, hardware, 3);
    }
    else if (characteristic->uuid_high == 0x00002a2900001000 && characteristic->uuid_low == 0x800000805f9b34fb) {
        // simulate response for reading manufacturer characteristic
        const uint8_t manufacturer[] = "MbientLab Inc";
        handler(caller, manufacturer, 13);
    }
    else if (characteristic->uuid_high == 0x00002a2500001000 && characteristic->uuid_low == 0x800000805f9b34fb) {
        // simulate response for reading manufacturer characteristic
        const uint8_t serial[] = "0476B3";
        handler(caller, serial, 6);
    }
}

static void write_gatt_char(void* context, const void* caller, MblMwGattCharWriteType write_type, const MblMwGattChar* characteristic, const uint8_t* value, uint8_t length)
{

//    WarbleGattChar* gattChar = warble_gatt_find_characteristic(m_Gatt,"326a9001-85cb-9195-d9dd-464cfbbae75a");

    if(charGattChar){
        if(write_type == MBL_MW_GATT_CHAR_WRITE_WITHOUT_RESPONSE){
            warble_gattchar_write_without_resp_async(charGattChar,value,length,context,[](void* context, WarbleGattChar* _gattChar, const char* test) -> void {
            });
        }
        else if(write_type == MBL_MW_GATT_CHAR_WRITE_WITH_RESPONSE){
            warble_gattchar_write_async(charGattChar,value,length,context,[](void* context, WarbleGattChar* _gattChar, const char* test) -> void {
            });
        }
    }

    cout << "{write type: " << write_type << ", value: " << hex << setw(2) << setfill('0') << "[0x" << (int)value[0];
    for (uint8_t i = 1; i < length; i++) {
        cout << ", 0x" << (int)value[i];
    }
    cout << "]}" << dec << endl;

}


static void enable_char_notify(void* context, const void* caller, const MblMwGattChar* characteristic, MblMwFnIntVoidPtrArray handler, MblMwFnVoidVoidPtrInt ready)
{
    notifyHandlers.insert({ caller, handler });

    if(enableGattChar){
        warble_gattchar_enable_notifications_async(enableGattChar,context,[](void* _context, WarbleGattChar* _gattChar, const char* test){
            warble_gattchar_on_notification_received(enableGattChar,nullptr,[](void* context, WarbleGattChar* __gattChar, const uint8_t* value, uint8_t length){
                if(__gattChar){
                    const void *_caller = notifyHandlers.begin()->first;
                    notifyHandlers.begin()->second(_caller,value,length);
                }

            });
        });
        ready(caller, MBL_MW_STATUS_OK);
    }
}

static void on_disconnect(void* context, const void* caller, MblMwFnVoidVoidPtrInt handler)
{
    // call this handler everytime connection is lost, use 0 for 'value' parameter
    disconnectHandlers.insert({ caller, handler });

    warble_gatt_on_disconnect(warbleGatt, context, [](void* context, WarbleGatt* caller, int32_t value){
        if(caller){
            const void* _caller = disconnectHandlers.begin()->first;

            if(_caller){
                disconnectHandlers.begin()->second(_caller,value);
                cout << "!!!!!!!!!! DISCONNECT !!!!!!!!!!" << value << endl;
//                value = 0;
            }
        }
    });
}
