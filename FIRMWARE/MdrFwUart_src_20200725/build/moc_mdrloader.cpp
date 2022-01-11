/****************************************************************************
** Meta object code from reading C++ file 'mdrloader.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mdrloader.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mdrloader.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MdrLoader_t {
    QByteArrayData data[18];
    char stringdata0[167];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MdrLoader_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MdrLoader_t qt_meta_stringdata_MdrLoader = {
    {
QT_MOC_LITERAL(0, 0, 9), // "MdrLoader"
QT_MOC_LITERAL(1, 10, 19), // "dumpFilenameChanged"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 6), // "dumpfn"
QT_MOC_LITERAL(4, 38, 13), // "statusChanged"
QT_MOC_LITERAL(5, 52, 1), // "s"
QT_MOC_LITERAL(6, 54, 11), // "textChanged"
QT_MOC_LITERAL(7, 66, 19), // "stopButtonSetEnable"
QT_MOC_LITERAL(8, 86, 12), // "workFinished"
QT_MOC_LITERAL(9, 99, 8), // "stopWork"
QT_MOC_LITERAL(10, 108, 8), // "WorkType"
QT_MOC_LITERAL(11, 117, 12), // "WorkTypeFlag"
QT_MOC_LITERAL(12, 130, 6), // "NoWork"
QT_MOC_LITERAL(13, 137, 4), // "Load"
QT_MOC_LITERAL(14, 142, 5), // "Erase"
QT_MOC_LITERAL(15, 148, 7), // "Program"
QT_MOC_LITERAL(16, 156, 6), // "Verify"
QT_MOC_LITERAL(17, 163, 3) // "Run"

    },
    "MdrLoader\0dumpFilenameChanged\0\0dumpfn\0"
    "statusChanged\0s\0textChanged\0"
    "stopButtonSetEnable\0workFinished\0"
    "stopWork\0WorkType\0WorkTypeFlag\0NoWork\0"
    "Load\0Erase\0Program\0Verify\0Run"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MdrLoader[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       1,   58, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       4,    1,   47,    2, 0x06 /* Public */,
       6,    1,   50,    2, 0x06 /* Public */,
       7,    1,   53,    2, 0x06 /* Public */,
       8,    0,   56,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,   57,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,

 // enums: name, alias, flags, count, data
      10,   11, 0x1,    6,   63,

 // enum data: key, value
      12, uint(MdrLoader::NoWork),
      13, uint(MdrLoader::Load),
      14, uint(MdrLoader::Erase),
      15, uint(MdrLoader::Program),
      16, uint(MdrLoader::Verify),
      17, uint(MdrLoader::Run),

       0        // eod
};

void MdrLoader::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MdrLoader *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->dumpFilenameChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->statusChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->textChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->stopButtonSetEnable((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->workFinished(); break;
        case 5: _t->stopWork(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MdrLoader::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MdrLoader::dumpFilenameChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MdrLoader::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MdrLoader::statusChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MdrLoader::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MdrLoader::textChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (MdrLoader::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MdrLoader::stopButtonSetEnable)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (MdrLoader::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MdrLoader::workFinished)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MdrLoader::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_MdrLoader.data,
    qt_meta_data_MdrLoader,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MdrLoader::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MdrLoader::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MdrLoader.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int MdrLoader::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void MdrLoader::dumpFilenameChanged(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MdrLoader::statusChanged(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MdrLoader::textChanged(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MdrLoader::stopButtonSetEnable(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void MdrLoader::workFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
