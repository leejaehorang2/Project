/****************************************************************************
** Meta object code from reading C++ file 'main_window.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../GestureRecognition_ChartTemp/Project/main_window.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'main_window.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[18];
    char stringdata0[283];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 12), // "setChartData"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 20), // "preprocessingDataSet"
QT_MOC_LITERAL(4, 46, 4), // "data"
QT_MOC_LITERAL(5, 51, 17), // "addLearningResult"
QT_MOC_LITERAL(6, 69, 6), // "result"
QT_MOC_LITERAL(7, 76, 19), // "clearLearningResult"
QT_MOC_LITERAL(8, 96, 20), // "clickedDeviceConnect"
QT_MOC_LITERAL(9, 117, 23), // "clickedDeviceDisconnect"
QT_MOC_LITERAL(10, 141, 13), // "clickedMotion"
QT_MOC_LITERAL(11, 155, 20), // "clickedTrainingStart"
QT_MOC_LITERAL(12, 176, 19), // "clickedTrainingStop"
QT_MOC_LITERAL(13, 196, 18), // "clickedRecognition"
QT_MOC_LITERAL(14, 215, 15), // "clickedLearning"
QT_MOC_LITERAL(15, 231, 16), // "clickedSaveModel"
QT_MOC_LITERAL(16, 248, 16), // "clickedLoadModel"
QT_MOC_LITERAL(17, 265, 17) // "clickedResetModel"

    },
    "MainWindow\0setChartData\0\0preprocessingDataSet\0"
    "data\0addLearningResult\0result\0"
    "clearLearningResult\0clickedDeviceConnect\0"
    "clickedDeviceDisconnect\0clickedMotion\0"
    "clickedTrainingStart\0clickedTrainingStop\0"
    "clickedRecognition\0clickedLearning\0"
    "clickedSaveModel\0clickedLoadModel\0"
    "clickedResetModel"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

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
       1,    1,   79,    2, 0x08 /* Private */,
       5,    1,   82,    2, 0x08 /* Private */,
       7,    0,   85,    2, 0x08 /* Private */,
       8,    0,   86,    2, 0x08 /* Private */,
       9,    0,   87,    2, 0x08 /* Private */,
      10,    0,   88,    2, 0x08 /* Private */,
      11,    0,   89,    2, 0x08 /* Private */,
      12,    0,   90,    2, 0x08 /* Private */,
      13,    0,   91,    2, 0x08 /* Private */,
      14,    0,   92,    2, 0x08 /* Private */,
      15,    0,   93,    2, 0x08 /* Private */,
      16,    0,   94,    2, 0x08 /* Private */,
      17,    0,   95,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setChartData((*reinterpret_cast< preprocessingDataSet(*)>(_a[1]))); break;
        case 1: _t->addLearningResult((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->clearLearningResult(); break;
        case 3: _t->clickedDeviceConnect(); break;
        case 4: _t->clickedDeviceDisconnect(); break;
        case 5: _t->clickedMotion(); break;
        case 6: _t->clickedTrainingStart(); break;
        case 7: _t->clickedTrainingStop(); break;
        case 8: _t->clickedRecognition(); break;
        case 9: _t->clickedLearning(); break;
        case 10: _t->clickedSaveModel(); break;
        case 11: _t->clickedLoadModel(); break;
        case 12: _t->clickedResetModel(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
