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

#include <iomanip>  //for setw, setfill
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sys/stat.h>   //for 'mkdir'
#include <vector>

#include <math.h>
//#include <future>
//#include <chrono>

using namespace std;
using namespace this_thread;
using namespace chrono;


static WarbleGatt* warbleGatt = nullptr;
static WarbleGattChar* charGattChar = nullptr;
static WarbleGattChar* enableGattChar = nullptr;

//For Test
static int testValue = 0;

static unordered_map<const void*, MblMwFnIntVoidPtrArray> notifyHandlers;
static unordered_map<const void*, MblMwFnVoidVoidPtrInt> disconnectHandlers;

static void read_gatt_char(void* context, const void* caller, const MblMwGattChar* characteristic, MblMwFnIntVoidPtrArray handler);
static void write_gatt_char(void* context, const void* caller, MblMwGattCharWriteType write_type, const MblMwGattChar* characteristic, const uint8_t* value, uint8_t length);
static void enable_char_notify(void* context, const void* caller, const MblMwGattChar* characteristic, MblMwFnIntVoidPtrArray handler, MblMwFnVoidVoidPtrInt ready);
static void on_disconnect(void* context, const void* caller, MblMwFnVoidVoidPtrInt handler);

GestureSensor::GestureSensor()
{
    mmrBoard = nullptr;
    running = false;
}

GestureSensor::~GestureSensor()
{
//    m_TT.wait();

    if(warbleGatt){
        if(warble_gatt_is_connected(warbleGatt)){
            warble_gatt_disconnect(warbleGatt);
        }
    }

    if(mmrBoard){
//        mbl_mw_debug_reset(m_board);
        mbl_mw_logging_clear_entries(mmrBoard);
        mbl_mw_metawearboard_tear_down(mmrBoard);
    }
}

void GestureSensor::startThread()
{
    running = true;
    scanThread = thread(&GestureSensor::bluetoothScanThread,this);
}

void GestureSensor::stopThread()
{
    if(scanThread.joinable()){
        running = false;
        scanThread.join();
    }
}

void GestureSensor::bluetoothScanThread()
{
    while(running){

        warble_scanner_set_handler(nullptr,[](void* context, const WarbleScanResult* result) -> void{
            int a = 0;
        });

        warble_scanner_start(0,nullptr);

        sleep_for(std::chrono::milliseconds(10000));

        warble_scanner_stop();

        sleep_for(std::chrono::milliseconds(10));
    }
}

void GestureSensor::connectMMRSensor()
{
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

    promise<void> connectTask;

    cout << "connecting to : " << "F8:B2:A7:B6:30:7E" << endl;

    warble_gatt_connect_async(warbleGatt, &connectTask, [](void* context, WarbleGatt* caller, const char* value){

        auto task = (promise<void>*) context;

        if(value != nullptr){
            cout << "You done fucked up" << endl;
            task->set_exception(make_exception_ptr(runtime_error(value)));
        }
        else{
            cout << "Am I Connected ?" << warble_gatt_is_connected(caller) << endl;
            this_thread::sleep_for(std::chrono::milliseconds(3000));
            task->set_value();
        }
    });

    connectTask.get_future().get();

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
}

void GestureSensor::testLED()
{
    MblMwLedPattern pattern1;
    pattern1.repeat_count = 255;

    mbl_mw_led_load_preset_pattern(&pattern1, MBL_MW_LED_PRESET_SOLID);
    mbl_mw_led_write_pattern(mmrBoard,&pattern1,MBL_MW_LED_COLOR_BLUE);
    mbl_mw_led_play(mmrBoard);

    this_thread::sleep_for(std::chrono::milliseconds(3000));

    mbl_mw_led_stop(mmrBoard);

    this_thread::sleep_for(std::chrono::milliseconds(1000));

    MblMwLedPattern pattern2;
    pattern2.repeat_count = 20;

    mbl_mw_led_load_preset_pattern(&pattern2, MBL_MW_LED_PRESET_BLINK);
    mbl_mw_led_write_pattern(mmrBoard,&pattern2,MBL_MW_LED_COLOR_RED);
    mbl_mw_led_play(mmrBoard);

    this_thread::sleep_for(std::chrono::milliseconds(3000));

    mbl_mw_led_stop(mmrBoard);

    this_thread::sleep_for(std::chrono::milliseconds(1000));

    MblMwLedPattern pattern3;
    pattern3.repeat_count = 255;

    mbl_mw_led_load_preset_pattern(&pattern3, MBL_MW_LED_PRESET_SOLID);
    mbl_mw_led_write_pattern(mmrBoard,&pattern3,MBL_MW_LED_COLOR_GREEN);
    mbl_mw_led_play(mmrBoard);

    this_thread::sleep_for(std::chrono::milliseconds(3000));

    mbl_mw_led_stop_and_clear(mmrBoard);
}

void GestureSensor::testAccelerometer()
{
    mbl_mw_logging_clear_entries(mmrBoard);

    MblMwDataSignal* accelSignal = mbl_mw_acc_get_acceleration_data_signal(mmrBoard);

    promise<MblMwDataLogger*> getLoggerTask;

    mbl_mw_datasignal_log(accelSignal,&getLoggerTask,[](void* context, MblMwDataLogger* logger) -> void {
        auto task = (promise<MblMwDataLogger*>*)context;
        task->set_value(logger);
    });

    MblMwDataLogger* logger = getLoggerTask.get_future().get();

    if(!logger){
        cout << "Failed to create logger!" << endl;
        return;
    }
    else{
        cout << "Logger create success!" << endl;
    }


    mbl_mw_acc_set_odr(mmrBoard,100.f);
    mbl_mw_acc_write_acceleration_config(mmrBoard);
    mbl_mw_logging_start(mmrBoard, 0);
    mbl_mw_acc_enable_acceleration_sampling(mmrBoard);
    mbl_mw_acc_start(mmrBoard);

    cout << "Accelerometer data for 5 sec...." << endl;

    this_thread::sleep_for(std::chrono::milliseconds(5000));

    mbl_mw_acc_stop(mmrBoard);
    mbl_mw_acc_disable_acceleration_sampling(mmrBoard);
    mbl_mw_logging_stop(mmrBoard);

    cout << "Downloading data..." << endl;
    mbl_mw_settings_set_connection_parameters(mmrBoard, 7.5, 7.5, 0, 6000);

    this_thread::sleep_for(std::chrono::milliseconds(1000));

    promise<void> downloadTask;

    static auto progressUpdate = [](void* context, uint32_t entriesLeft, uint32_t totalEntries)-> void {

        auto task = (promise<void>*)context;

        cout << "Download progress = " << (double)entriesLeft / (double)totalEntries << endl;

        if (!entriesLeft) {
            cout << "download complete! " << endl;
            task->set_value();
        }
    };

    static auto unknownEntry = [](void* context, uint8_t id, int64_t epoch, const uint8_t* data, uint8_t length) -> void {
    };

    static auto receivedUnhandledEntry = [](void *context, const MblMwData *data) -> void {
    };

    static MblMwLogDownloadHandler download_handler = { &downloadTask, progressUpdate, unknownEntry, receivedUnhandledEntry};


    if(logger){
        mbl_mw_logger_subscribe(logger, nullptr, [](void* context, const MblMwData* data) -> void {
            auto acceleration = (MblMwCartesianFloat*) data->value;
            cout << "X : " << acceleration->x << "  Y : " << acceleration->y << "  Z : " << acceleration->z << endl;
        });
    }

    mbl_mw_logging_download(mmrBoard, 100, &download_handler);

    downloadTask.get_future().get();

    mbl_mw_logger_remove(logger);
}

void GestureSensor::testGyro()
{
    mbl_mw_logging_clear_entries(mmrBoard);

    MblMwDataSignal* gyroSignal = mbl_mw_gyro_bmi160_get_rotation_data_signal(mmrBoard);

    promise<MblMwDataLogger*> getLoggerTask;

    mbl_mw_datasignal_log(gyroSignal,&getLoggerTask,[](void* context, MblMwDataLogger* logger) -> void {
        auto task = (promise<MblMwDataLogger*>*)context;
        task->set_value(logger);
    });

    MblMwDataLogger* logger = getLoggerTask.get_future().get();

    if(!logger){
        cout << "Failed to create logger!" << endl;
        return;
    }
    else{
        cout << "Logger create success!" << endl;
    }


//    mbl_mw_acc_set_odr(mmrBoard,100.f);
//    mbl_mw_logging_start(mmrBoard, 0);
//    mbl_mw_acc_enable_acceleration_sampling(mmrBoard);
//    mbl_mw_acc_start(mmrBoard);

    mbl_mw_gyro_bmi160_set_odr(mmrBoard,MBL_MW_GYRO_BMI160_ODR_100Hz);
    mbl_mw_gyro_bmi160_set_range(mmrBoard,MBL_MW_GYRO_BMI160_RANGE_125dps);
    mbl_mw_gyro_bmi160_write_config(mmrBoard);
    mbl_mw_logging_start(mmrBoard, 0);
    mbl_mw_gyro_bmi160_enable_rotation_sampling(mmrBoard);
    mbl_mw_gyro_bmi160_start(mmrBoard);

    cout << "Gyro data for 5 sec...." << endl;

    this_thread::sleep_for(std::chrono::milliseconds(5000));

    mbl_mw_gyro_bmi160_stop(mmrBoard);
    mbl_mw_gyro_bmi160_disable_rotation_sampling(mmrBoard);
    mbl_mw_logging_stop(mmrBoard);

    cout << "Downloading data..." << endl;
    mbl_mw_settings_set_connection_parameters(mmrBoard, 7.5, 7.5, 0, 6000);

    this_thread::sleep_for(std::chrono::milliseconds(1000));

    promise<void> downloadTask;

    static auto progressUpdate = [](void* context, uint32_t entriesLeft, uint32_t totalEntries)-> void {

        auto task = (promise<void>*)context;

        cout << "Download progress = " << (double)entriesLeft / (double)totalEntries << endl;

        if (!entriesLeft) {
            cout << "download complete! " << endl;
            task->set_value();
        }
    };

    static auto unknownEntry = [](void* context, uint8_t id, int64_t epoch, const uint8_t* data, uint8_t length) -> void {
    };

    static auto receivedUnhandledEntry = [](void *context, const MblMwData *data) -> void {
    };

    static MblMwLogDownloadHandler downloadHandler = { &downloadTask, progressUpdate, unknownEntry, receivedUnhandledEntry};


    if(logger){
        mbl_mw_logger_subscribe(logger, nullptr, [](void* context, const MblMwData* data) -> void {
            auto rotateRate = (MblMwCartesianFloat*) data->value;
            cout << "X : " << rotateRate->x << "  Y : " << rotateRate->y << "  Z : " << rotateRate->z << endl;
        });
    }

    mbl_mw_logging_download(mmrBoard, 100, &downloadHandler);

    downloadTask.get_future().get();

    mbl_mw_logger_remove(logger);
}

void GestureSensor::testAccelAndGyro()
{
    mbl_mw_logging_clear_entries(mmrBoard);

    MblMwDataSignal* accelSignal = mbl_mw_acc_get_acceleration_data_signal(mmrBoard);

    promise<MblMwDataLogger*> getAccelLoggerTask;

    mbl_mw_datasignal_log(accelSignal,&getAccelLoggerTask,[](void* context, MblMwDataLogger* logger) -> void {
        auto task = (promise<MblMwDataLogger*>*)context;
        task->set_value(logger);
    });

    MblMwDataLogger* accelLogger = getAccelLoggerTask.get_future().get();

    if(!accelLogger){
        cout << "Failed to create accel logger!" << endl;
        return;
    }
    else{
        cout << "Logger create success!" << endl;
    }

    MblMwDataSignal* gyroSignal = mbl_mw_gyro_bmi160_get_rotation_data_signal(mmrBoard);

    promise<MblMwDataLogger*> getGyroLoggerTask;

    mbl_mw_datasignal_log(gyroSignal,&getGyroLoggerTask,[](void* context, MblMwDataLogger* logger) -> void {
        auto task = (promise<MblMwDataLogger*>*)context;
        task->set_value(logger);
    });

    MblMwDataLogger* gyroLogger = getGyroLoggerTask.get_future().get();

    if(!gyroLogger){
        cout << "Failed to create gyro logger!" << endl;
        return;
    }
    else{
        cout << "Logger create success!" << endl;
    }

    mbl_mw_acc_set_odr(mmrBoard,100.f);
    mbl_mw_acc_write_acceleration_config(mmrBoard);

    mbl_mw_gyro_bmi160_set_odr(mmrBoard,MBL_MW_GYRO_BMI160_ODR_100Hz);
    mbl_mw_gyro_bmi160_set_range(mmrBoard,MBL_MW_GYRO_BMI160_RANGE_125dps);
    mbl_mw_gyro_bmi160_write_config(mmrBoard);

    mbl_mw_logging_start(mmrBoard, 0);

    mbl_mw_acc_enable_acceleration_sampling(mmrBoard);
    mbl_mw_gyro_bmi160_enable_rotation_sampling(mmrBoard);

    mbl_mw_acc_start(mmrBoard);
    mbl_mw_gyro_bmi160_start(mmrBoard);

    cout << "Accelerometer + Gyro data for 5 sec...." << endl;

    this_thread::sleep_for(std::chrono::milliseconds(5000));

    mbl_mw_acc_stop(mmrBoard);
    mbl_mw_gyro_bmi160_stop(mmrBoard);

    mbl_mw_acc_disable_acceleration_sampling(mmrBoard);
    mbl_mw_gyro_bmi160_disable_rotation_sampling(mmrBoard);
    mbl_mw_logging_stop(mmrBoard);

    cout << "Downloading data..." << endl;
    mbl_mw_settings_set_connection_parameters(mmrBoard, 7.5, 7.5, 0, 6000);

    this_thread::sleep_for(std::chrono::milliseconds(1000));

//    promise<void> downloadTask, accelDownTask, gyroDownTask;

//    shared_future<void> taskFuture(downloadTask.get_future());

//    sTest sss1;
//    sss1.t2 = &taskFuture;

//    sTest sss2;
//    sss2.t2 = &taskFuture;

    bool accelEnd = false;
    bool gyroEnd = false;

    static auto accelProgressUpdate = [](void* context, uint32_t entriesLeft, uint32_t totalEntries)-> void {

        bool *task = (bool*)context;

        cout << "[Accel] Download progress = " << (double)entriesLeft / (double)totalEntries << endl;

        if (!entriesLeft) {
            cout << "[Accel] Download complete! " << endl;
            *task = true;
        }
    };

    static auto accelUnknownEntry = [](void* context, uint8_t id, int64_t epoch, const uint8_t* data, uint8_t length) -> void {
    };

    static auto accelReceivedUnhandledEntry = [](void *context, const MblMwData *data) -> void {
    };

    static MblMwLogDownloadHandler accelDownloadhandler = { &accelEnd, accelProgressUpdate, accelUnknownEntry, accelReceivedUnhandledEntry};


//    promise<void> downloadGyroTask;

    static auto gyroProgressUpdate = [](void* context, uint32_t entriesLeft, uint32_t totalEntries)-> void {

        bool *task = (bool*)context;

        cout << "[Gyro] Download progress = " << (double)entriesLeft / (double)totalEntries << endl;

        if (!entriesLeft) {
            cout << "[Gyro] Download complete! " << endl;
            *task = true;
        }
    };

    static auto gyroUnknownEntry = [](void* context, uint8_t id, int64_t epoch, const uint8_t* data, uint8_t length) -> void {
    };

    static auto gyroReceivedUnhandledEntry = [](void *context, const MblMwData *data) -> void {
    };

    static MblMwLogDownloadHandler gyroDownloadHandler = { &gyroEnd, gyroProgressUpdate, gyroUnknownEntry, gyroReceivedUnhandledEntry};

    if(accelLogger){
        mbl_mw_logger_subscribe(accelLogger, nullptr, [](void* context, const MblMwData* data) -> void {
            auto accel = (MblMwCartesianFloat*) data->value;
            cout << "Accel X : " << accel->x << " Accel Y : " << accel->y << " Accel Z : " << accel->z << endl;
        });
    }

    if(gyroLogger){
        mbl_mw_logger_subscribe(gyroLogger, nullptr, [](void* context, const MblMwData* data) -> void {
            auto gyro = (MblMwCartesianFloat*) data->value;
            cout << "Rol X: " << gyro->x << " Rol Y : " << gyro->y << " Rol Z : " << gyro->z << endl;
        });
    }

    mbl_mw_logging_download(mmrBoard, 100, &accelDownloadhandler);
    mbl_mw_logging_download(mmrBoard, 100, &gyroDownloadHandler);

//    sss2.t1.get_future().wait();
//    sss1.t1.get_future().wait();

//    downloadTask.get_future().get();

//    downloadTask.set_value();

    system_clock::time_point start = system_clock::now();

//    microseconds microSec =

    while(true){
        system_clock::time_point end = system_clock::now();

        if(accelEnd && gyroEnd){
            break;
        }
        this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    mbl_mw_logger_remove(accelLogger);
    mbl_mw_logger_remove(gyroLogger);

    cout << "REAL END!!!!!!!!!!!!!!" << endl;
}



void GestureSensor::testSensorFusion()
{
    cout << "Downloading data..." << endl;
    mbl_mw_settings_set_connection_parameters(mmrBoard, 7.5, 7.5, 0, 6000);

    this_thread::sleep_for(std::chrono::milliseconds(1500));

    MblMwDataSignal* accelSignal = mbl_mw_acc_get_acceleration_data_signal(mmrBoard);
    MblMwDataSignal* gyroSignal = mbl_mw_gyro_bmi160_get_rotation_data_signal(mmrBoard);

//    promise<MblMwDataProcessor*> fuserCreateTask;
    dataProcessorStruct dataProc;
    dataProc.data.reserve(256);

    static auto procCreated = [](void* context, MblMwDataProcessor* fuser){
        auto task = (dataProcessorStruct*)context;

        if(fuser){
            mbl_mw_datasignal_subscribe((MblMwDataSignal*)fuser, context, [](void* context, const MblMwData* data){
                auto fused = (MblMwData**)data->value;

                auto acc = (MblMwCartesianFloat*)fused[0]->value;
                auto gyro = (MblMwCartesianFloat*)fused[1]->value;

//                cout << "DATA COUNT : " << ++testValue << endl;

                sensorData sData;

                sData.value[AXIS_X_ACCELEROMETER] = floor(acc->x * 1000.f) / 1000.f;
                sData.value[AXIS_Y_ACCELEROMETER] = floor(acc->y * 1000.f) / 1000.f;
                sData.value[AXIS_Z_ACCELEROMETER] = floor(acc->z * 1000.f) / 1000.f;
                sData.value[AXIS_X_GYRO] = floor(gyro->x * 1000.f) / 1000.f;
                sData.value[AXIS_Y_GYRO] = floor(gyro->y * 1000.f) / 1000.f;
                sData.value[AXIS_Z_GYRO] = floor(gyro->z * 1000.f) / 1000.f;

//                sData.accelX = acc->x;
//                sData.accelY = acc->y;
//                sData.accelZ = acc->z;
//                sData.rotateX = gyro->x;
//                sData.rotateY = gyro->y;
//                sData.rotateZ = gyro->z;

                auto task = (dataProcessorStruct*)context;

                task->data.push_back(sData);

//                cout << "ACC X: " << acc->x << " ACC Y : " << acc->y << " ACC Z : " << acc->z << " ROL X: " << gyro->x << " ROL Y : " << gyro->y << " ROL Z : " << gyro->z << endl;
//                cout << "ACC X: " << sData.accelX << " ACC Y : " << sData.accelY << " ACC Z : " << sData.accelZ << " ROL X: " << sData.rotateX << " ROL Y : " << sData.rotateY << " ROL Z : " << sData.rotateZ << endl;
                cout << "ACC X: " << sData.value[AXIS_X_ACCELEROMETER] << " ACC Y : " << sData.value[AXIS_Y_ACCELEROMETER] <<
                        " ACC Z : " << sData.value[AXIS_Z_ACCELEROMETER] << " ROL X: " << sData.value[AXIS_X_GYRO] <<
                        " ROL Y : " << sData.value[AXIS_Y_GYRO] << " ROL Z : " << sData.value[AXIS_Z_GYRO] << endl;
            });
        }
        cout << "Downloading..." << endl;

        task->fuserCreateTask.set_value(fuser);

    };

    mbl_mw_dataprocessor_fuser_create(accelSignal, &gyroSignal, 1, &dataProc, procCreated);

    mbl_mw_acc_set_odr(mmrBoard,250.f);
    mbl_mw_acc_set_range(mmrBoard,20.f);
    mbl_mw_acc_write_acceleration_config(mmrBoard);

    mbl_mw_gyro_bmi160_set_odr(mmrBoard,MBL_MW_GYRO_BMI160_ODR_100Hz);
    mbl_mw_gyro_bmi160_set_range(mmrBoard,MBL_MW_GYRO_BMI160_RANGE_250dps);
    mbl_mw_gyro_bmi160_write_config(mmrBoard);

    mbl_mw_gyro_bmi160_enable_rotation_sampling(mmrBoard);
    mbl_mw_acc_enable_acceleration_sampling(mmrBoard);

    mbl_mw_gyro_bmi160_start(mmrBoard);
    mbl_mw_acc_start(mmrBoard);

    this_thread::sleep_for(std::chrono::milliseconds(5000));

    mbl_mw_acc_stop(mmrBoard);
    mbl_mw_gyro_bmi160_stop(mmrBoard);

    mbl_mw_acc_disable_acceleration_sampling(mmrBoard);
    mbl_mw_gyro_bmi160_disable_rotation_sampling(mmrBoard);

    MblMwDataProcessor* fuser = nullptr;
    fuser = dataProc.fuserCreateTask.get_future().get();

    cout << "Download Complete!" << endl;

    if(fuser){
        mbl_mw_sensor_fusion_clear_enabled_mask(mmrBoard);
        mbl_mw_dataprocessor_remove(fuser);
    }

    cout << "REAL END!!" << endl;

    testValue = 0;

    connectionHelper->requestSetChartData(dataProc.data);
}

void GestureSensor::testBluetoothScan()
{
    warble_scanner_set_handler(nullptr,[](void* context, const WarbleScanResult* result) -> void{
        int a = 0;
    });

//    WarbleOption option[2];
//    option[0].key = "hci";
//    option[1].key = "scan-type";

//    warble_scanner_start(0,option);

    warble_scanner_start(0,nullptr);

    sleep_for(std::chrono::milliseconds(10000));

    warble_scanner_stop();
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
            disconnectHandlers.begin()->second(_caller,value);
        }
    });
}
