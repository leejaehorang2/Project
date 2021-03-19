/****************************************************************************
** Meta object code from reading C++ file 'device_handler.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Project/device_handler.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'device_handler.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DeviceHandler_t {
    QByteArrayData data[11];
    char stringdata0[149];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DeviceHandler_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DeviceHandler_t qt_meta_stringdata_DeviceHandler = {
    {
QT_MOC_LITERAL(0, 0, 13), // "DeviceHandler"
QT_MOC_LITERAL(1, 14, 16), // "measuringChanged"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 12), // "aliveChanged"
QT_MOC_LITERAL(4, 45, 12), // "statsChanged"
QT_MOC_LITERAL(5, 58, 16), // "startMeasurement"
QT_MOC_LITERAL(6, 75, 15), // "stopMeasurement"
QT_MOC_LITERAL(7, 91, 17), // "disconnectService"
QT_MOC_LITERAL(8, 109, 11), // "AddressType"
QT_MOC_LITERAL(9, 121, 13), // "PublicAddress"
QT_MOC_LITERAL(10, 135, 13) // "RandomAddress"

    },
    "DeviceHandler\0measuringChanged\0\0"
    "aliveChanged\0statsChanged\0startMeasurement\0"
    "stopMeasurement\0disconnectService\0"
    "AddressType\0PublicAddress\0RandomAddress"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DeviceHandler[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       1,   50, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x06 /* Public */,
       3,    0,   45,    2, 0x06 /* Public */,
       4,    0,   46,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   47,    2, 0x0a /* Public */,
       6,    0,   48,    2, 0x0a /* Public */,
       7,    0,   49,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // enums: name, flags, count, data
       8, 0x2,    2,   54,

 // enum data: key, value
       9, uint(DeviceHandler::AddressType::PublicAddress),
      10, uint(DeviceHandler::AddressType::RandomAddress),

       0        // eod
};

void DeviceHandler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DeviceHandler *_t = static_cast<DeviceHandler *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->measuringChanged(); break;
        case 1: _t->aliveChanged(); break;
        case 2: _t->statsChanged(); break;
        case 3: _t->startMeasurement(); break;
        case 4: _t->stopMeasurement(); break;
        case 5: _t->disconnectService(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (DeviceHandler::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DeviceHandler::measuringChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (DeviceHandler::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DeviceHandler::aliveChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (DeviceHandler::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DeviceHandler::statsChanged)) {
                *result = 2;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject DeviceHandler::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DeviceHandler.data,
      qt_meta_data_DeviceHandler,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *DeviceHandler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DeviceHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DeviceHandler.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int DeviceHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void DeviceHandler::measuringChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void DeviceHandler::aliveChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void DeviceHandler::statsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
