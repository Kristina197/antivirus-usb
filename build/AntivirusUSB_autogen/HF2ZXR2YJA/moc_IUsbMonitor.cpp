/****************************************************************************
** Meta object code from reading C++ file 'IUsbMonitor.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/usecases/IUsbMonitor.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'IUsbMonitor.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_IUsbMonitor_t {
    uint offsetsAndSizes[12];
    char stringdata0[12];
    char stringdata1[16];
    char stringdata2[1];
    char stringdata3[11];
    char stringdata4[7];
    char stringdata5[19];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_IUsbMonitor_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_IUsbMonitor_t qt_meta_stringdata_IUsbMonitor = {
    {
        QT_MOC_LITERAL(0, 11),  // "IUsbMonitor"
        QT_MOC_LITERAL(12, 15),  // "deviceConnected"
        QT_MOC_LITERAL(28, 0),  // ""
        QT_MOC_LITERAL(29, 10),  // "DeviceInfo"
        QT_MOC_LITERAL(40, 6),  // "device"
        QT_MOC_LITERAL(47, 18)   // "deviceDisconnected"
    },
    "IUsbMonitor",
    "deviceConnected",
    "",
    "DeviceInfo",
    "device",
    "deviceDisconnected"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_IUsbMonitor[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   26,    2, 0x06,    1 /* Public */,
       5,    1,   29,    2, 0x06,    3 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

Q_CONSTINIT const QMetaObject IUsbMonitor::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_IUsbMonitor.offsetsAndSizes,
    qt_meta_data_IUsbMonitor,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_IUsbMonitor_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<IUsbMonitor, std::true_type>,
        // method 'deviceConnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const DeviceInfo &, std::false_type>,
        // method 'deviceDisconnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const DeviceInfo &, std::false_type>
    >,
    nullptr
} };

void IUsbMonitor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<IUsbMonitor *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->deviceConnected((*reinterpret_cast< std::add_pointer_t<DeviceInfo>>(_a[1]))); break;
        case 1: _t->deviceDisconnected((*reinterpret_cast< std::add_pointer_t<DeviceInfo>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (IUsbMonitor::*)(const DeviceInfo & );
            if (_t _q_method = &IUsbMonitor::deviceConnected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (IUsbMonitor::*)(const DeviceInfo & );
            if (_t _q_method = &IUsbMonitor::deviceDisconnected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject *IUsbMonitor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *IUsbMonitor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_IUsbMonitor.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int IUsbMonitor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void IUsbMonitor::deviceConnected(const DeviceInfo & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void IUsbMonitor::deviceDisconnected(const DeviceInfo & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
