/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/ui/MainWindow.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_MainWindow_t {
    uint offsetsAndSizes[36];
    char stringdata0[11];
    char stringdata1[18];
    char stringdata2[1];
    char stringdata3[11];
    char stringdata4[7];
    char stringdata5[21];
    char stringdata6[20];
    char stringdata7[26];
    char stringdata8[24];
    char stringdata9[15];
    char stringdata10[24];
    char stringdata11[8];
    char stringdata12[11];
    char stringdata13[18];
    char stringdata14[13];
    char stringdata15[19];
    char stringdata16[9];
    char stringdata17[10];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_MainWindow_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
        QT_MOC_LITERAL(0, 10),  // "MainWindow"
        QT_MOC_LITERAL(11, 17),  // "onDeviceConnected"
        QT_MOC_LITERAL(29, 0),  // ""
        QT_MOC_LITERAL(30, 10),  // "DeviceInfo"
        QT_MOC_LITERAL(41, 6),  // "device"
        QT_MOC_LITERAL(48, 20),  // "onDeviceDisconnected"
        QT_MOC_LITERAL(69, 19),  // "onScanButtonClicked"
        QT_MOC_LITERAL(89, 25),  // "onQuarantineButtonClicked"
        QT_MOC_LITERAL(115, 23),  // "onSettingsButtonClicked"
        QT_MOC_LITERAL(139, 14),  // "onScanFinished"
        QT_MOC_LITERAL(154, 23),  // "std::vector<ScanResult>"
        QT_MOC_LITERAL(178, 7),  // "results"
        QT_MOC_LITERAL(186, 10),  // "totalFiles"
        QT_MOC_LITERAL(197, 17),  // "showThreatSummary"
        QT_MOC_LITERAL(215, 12),  // "threatsFound"
        QT_MOC_LITERAL(228, 18),  // "handleInfectedFile"
        QT_MOC_LITERAL(247, 8),  // "filePath"
        QT_MOC_LITERAL(256, 9)   // "virusName"
    },
    "MainWindow",
    "onDeviceConnected",
    "",
    "DeviceInfo",
    "device",
    "onDeviceDisconnected",
    "onScanButtonClicked",
    "onQuarantineButtonClicked",
    "onSettingsButtonClicked",
    "onScanFinished",
    "std::vector<ScanResult>",
    "results",
    "totalFiles",
    "showThreatSummary",
    "threatsFound",
    "handleInfectedFile",
    "filePath",
    "virusName"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_MainWindow[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   62,    2, 0x08,    1 /* Private */,
       5,    1,   65,    2, 0x08,    3 /* Private */,
       6,    0,   68,    2, 0x08,    5 /* Private */,
       7,    0,   69,    2, 0x08,    6 /* Private */,
       8,    0,   70,    2, 0x08,    7 /* Private */,
       9,    3,   71,    2, 0x08,    8 /* Private */,
      13,    2,   78,    2, 0x08,   12 /* Private */,
      15,    2,   83,    2, 0x08,   15 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 10, QMetaType::Int,    4,   11,   12,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   14,   12,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   16,   17,

       0        // eod
};

Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.offsetsAndSizes,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_MainWindow_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>,
        // method 'onDeviceConnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const DeviceInfo &, std::false_type>,
        // method 'onDeviceDisconnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const DeviceInfo &, std::false_type>,
        // method 'onScanButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onQuarantineButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSettingsButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onScanFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const DeviceInfo &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const std::vector<ScanResult> &, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'showThreatSummary'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'handleInfectedFile'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
    >,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onDeviceConnected((*reinterpret_cast< std::add_pointer_t<DeviceInfo>>(_a[1]))); break;
        case 1: _t->onDeviceDisconnected((*reinterpret_cast< std::add_pointer_t<DeviceInfo>>(_a[1]))); break;
        case 2: _t->onScanButtonClicked(); break;
        case 3: _t->onQuarantineButtonClicked(); break;
        case 4: _t->onSettingsButtonClicked(); break;
        case 5: _t->onScanFinished((*reinterpret_cast< std::add_pointer_t<DeviceInfo>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<std::vector<ScanResult>>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3]))); break;
        case 6: _t->showThreatSummary((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 7: _t->handleInfectedFile((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        default: ;
        }
    }
}

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
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
