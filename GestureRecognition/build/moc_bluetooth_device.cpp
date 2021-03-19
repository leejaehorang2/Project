/****************************************************************************
** Meta object code from reading C++ file 'bluetooth_device.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Project/bluetooth_device.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bluetooth_device.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_BluetoothDevice_t {
    QByteArrayData data[26];
    char stringdata0[429];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BluetoothDevice_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BluetoothDevice_t qt_meta_stringdata_BluetoothDevice = {
    {
QT_MOC_LITERAL(0, 0, 15), // "BluetoothDevice"
QT_MOC_LITERAL(1, 16, 20), // "startDeviceDiscovery"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 12), // "scanServices"
QT_MOC_LITERAL(4, 51, 7), // "address"
QT_MOC_LITERAL(5, 59, 16), // "connectToService"
QT_MOC_LITERAL(6, 76, 11), // "serviceUuid"
QT_MOC_LITERAL(7, 88, 20), // "disconnectFromDevice"
QT_MOC_LITERAL(8, 109, 9), // "addDevice"
QT_MOC_LITERAL(9, 119, 20), // "QBluetoothDeviceInfo"
QT_MOC_LITERAL(10, 140, 10), // "deviceInfo"
QT_MOC_LITERAL(11, 151, 18), // "deviceScanFinished"
QT_MOC_LITERAL(12, 170, 15), // "deviceScanError"
QT_MOC_LITERAL(13, 186, 37), // "QBluetoothDeviceDiscoveryAgen..."
QT_MOC_LITERAL(14, 224, 5), // "error"
QT_MOC_LITERAL(15, 230, 19), // "addLowEnergyService"
QT_MOC_LITERAL(16, 250, 14), // "QBluetoothUuid"
QT_MOC_LITERAL(17, 265, 4), // "uuid"
QT_MOC_LITERAL(18, 270, 15), // "deviceConnected"
QT_MOC_LITERAL(19, 286, 13), // "errorReceived"
QT_MOC_LITERAL(20, 300, 27), // "QLowEnergyController::Error"
QT_MOC_LITERAL(21, 328, 15), // "serviceScanDone"
QT_MOC_LITERAL(22, 344, 18), // "deviceDisconnected"
QT_MOC_LITERAL(23, 363, 24), // "serviceDetailsDiscovered"
QT_MOC_LITERAL(24, 388, 31), // "QLowEnergyService::ServiceState"
QT_MOC_LITERAL(25, 420, 8) // "newState"

    },
    "BluetoothDevice\0startDeviceDiscovery\0"
    "\0scanServices\0address\0connectToService\0"
    "serviceUuid\0disconnectFromDevice\0"
    "addDevice\0QBluetoothDeviceInfo\0"
    "deviceInfo\0deviceScanFinished\0"
    "deviceScanError\0QBluetoothDeviceDiscoveryAgent::Error\0"
    "error\0addLowEnergyService\0QBluetoothUuid\0"
    "uuid\0deviceConnected\0errorReceived\0"
    "QLowEnergyController::Error\0serviceScanDone\0"
    "deviceDisconnected\0serviceDetailsDiscovered\0"
    "QLowEnergyService::ServiceState\0"
    "newState"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BluetoothDevice[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x0a /* Public */,
       3,    1,   80,    2, 0x0a /* Public */,
       5,    1,   83,    2, 0x0a /* Public */,
       7,    0,   86,    2, 0x0a /* Public */,
       8,    1,   87,    2, 0x08 /* Private */,
      11,    0,   90,    2, 0x08 /* Private */,
      12,    1,   91,    2, 0x08 /* Private */,
      15,    1,   94,    2, 0x08 /* Private */,
      18,    0,   97,    2, 0x08 /* Private */,
      19,    1,   98,    2, 0x08 /* Private */,
      21,    0,  101,    2, 0x08 /* Private */,
      22,    0,  102,    2, 0x08 /* Private */,
      23,    1,  103,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void, 0x80000000 | 16,   17,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 20,   14,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 24,   25,

       0        // eod
};

void BluetoothDevice::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BluetoothDevice *_t = static_cast<BluetoothDevice *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->startDeviceDiscovery(); break;
        case 1: _t->scanServices((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->connectToService((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->disconnectFromDevice(); break;
        case 4: _t->addDevice((*reinterpret_cast< const QBluetoothDeviceInfo(*)>(_a[1]))); break;
        case 5: _t->deviceScanFinished(); break;
        case 6: _t->deviceScanError((*reinterpret_cast< QBluetoothDeviceDiscoveryAgent::Error(*)>(_a[1]))); break;
        case 7: _t->addLowEnergyService((*reinterpret_cast< const QBluetoothUuid(*)>(_a[1]))); break;
        case 8: _t->deviceConnected(); break;
        case 9: _t->errorReceived((*reinterpret_cast< QLowEnergyController::Error(*)>(_a[1]))); break;
        case 10: _t->serviceScanDone(); break;
        case 11: _t->deviceDisconnected(); break;
        case 12: _t->serviceDetailsDiscovered((*reinterpret_cast< QLowEnergyService::ServiceState(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QBluetoothDeviceInfo >(); break;
            }
            break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QBluetoothUuid >(); break;
            }
            break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QLowEnergyController::Error >(); break;
            }
            break;
        case 12:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QLowEnergyService::ServiceState >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject BluetoothDevice::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_BluetoothDevice.data,
      qt_meta_data_BluetoothDevice,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *BluetoothDevice::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BluetoothDevice::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_BluetoothDevice.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int BluetoothDevice::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
