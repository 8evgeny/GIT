/****************************************************************************
** Meta object code from reading C++ file 'vasililoader.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../vasililoader.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'vasililoader.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VasiliLoaderClient_t {
    QByteArrayData data[13];
    char stringdata0[158];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VasiliLoaderClient_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VasiliLoaderClient_t qt_meta_stringdata_VasiliLoaderClient = {
    {
QT_MOC_LITERAL(0, 0, 18), // "VasiliLoaderClient"
QT_MOC_LITERAL(1, 19, 19), // "dumpFilenameChanged"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 6), // "dumpfn"
QT_MOC_LITERAL(4, 47, 13), // "statusChanged"
QT_MOC_LITERAL(5, 61, 1), // "s"
QT_MOC_LITERAL(6, 63, 11), // "textChanged"
QT_MOC_LITERAL(7, 75, 19), // "stopButtonSetEnable"
QT_MOC_LITERAL(8, 95, 12), // "workFinished"
QT_MOC_LITERAL(9, 108, 8), // "stopWork"
QT_MOC_LITERAL(10, 117, 9), // "doWorkNew"
QT_MOC_LITERAL(11, 127, 28), // "VasiliLoaderClient::WorkType"
QT_MOC_LITERAL(12, 156, 1) // "w"

    },
    "VasiliLoaderClient\0dumpFilenameChanged\0"
    "\0dumpfn\0statusChanged\0s\0textChanged\0"
    "stopButtonSetEnable\0workFinished\0"
    "stopWork\0doWorkNew\0VasiliLoaderClient::WorkType\0"
    "w"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VasiliLoaderClient[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,
       4,    1,   52,    2, 0x06 /* Public */,
       6,    1,   55,    2, 0x06 /* Public */,
       7,    1,   58,    2, 0x06 /* Public */,
       8,    0,   61,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,   62,    2, 0x0a /* Public */,
      10,    1,   63,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Bool, 0x80000000 | 11,   12,

       0        // eod
};

void VasiliLoaderClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VasiliLoaderClient *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->dumpFilenameChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->statusChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->textChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->stopButtonSetEnable((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->workFinished(); break;
        case 5: _t->stopWork(); break;
        case 6: { bool _r = _t->doWorkNew((*reinterpret_cast< VasiliLoaderClient::WorkType(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (VasiliLoaderClient::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VasiliLoaderClient::dumpFilenameChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (VasiliLoaderClient::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VasiliLoaderClient::statusChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (VasiliLoaderClient::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VasiliLoaderClient::textChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (VasiliLoaderClient::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VasiliLoaderClient::stopButtonSetEnable)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (VasiliLoaderClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VasiliLoaderClient::workFinished)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject VasiliLoaderClient::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_VasiliLoaderClient.data,
    qt_meta_data_VasiliLoaderClient,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *VasiliLoaderClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VasiliLoaderClient::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VasiliLoaderClient.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int VasiliLoaderClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void VasiliLoaderClient::dumpFilenameChanged(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void VasiliLoaderClient::statusChanged(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void VasiliLoaderClient::textChanged(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void VasiliLoaderClient::stopButtonSetEnable(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void VasiliLoaderClient::workFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
