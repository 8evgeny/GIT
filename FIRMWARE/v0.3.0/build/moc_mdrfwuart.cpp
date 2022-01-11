/****************************************************************************
** Meta object code from reading C++ file 'mdrfwuart.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mdrfwuart.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mdrfwuart.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MdrFwUart_t {
    QByteArrayData data[17];
    char stringdata0[258];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MdrFwUart_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MdrFwUart_t qt_meta_stringdata_MdrFwUart = {
    {
QT_MOC_LITERAL(0, 0, 9), // "MdrFwUart"
QT_MOC_LITERAL(1, 10, 7), // "operate"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 28), // "VasiliLoaderClient::WorkType"
QT_MOC_LITERAL(4, 48, 4), // "work"
QT_MOC_LITERAL(5, 53, 21), // "on_openFilePB_clicked"
QT_MOC_LITERAL(6, 75, 22), // "on_startStopPB_clicked"
QT_MOC_LITERAL(7, 98, 17), // "on_dumpCB_clicked"
QT_MOC_LITERAL(8, 116, 7), // "checked"
QT_MOC_LITERAL(9, 124, 19), // "on_dumpfnPB_clicked"
QT_MOC_LITERAL(10, 144, 14), // "loaderFinished"
QT_MOC_LITERAL(11, 159, 22), // "on_flasherfnPB_clicked"
QT_MOC_LITERAL(12, 182, 22), // "on_action_Qt_triggered"
QT_MOC_LITERAL(13, 205, 18), // "on_about_triggered"
QT_MOC_LITERAL(14, 224, 7), // "logText"
QT_MOC_LITERAL(15, 232, 9), // "logStatus"
QT_MOC_LITERAL(16, 242, 15) // "updateComPortCB"

    },
    "MdrFwUart\0operate\0\0VasiliLoaderClient::WorkType\0"
    "work\0on_openFilePB_clicked\0"
    "on_startStopPB_clicked\0on_dumpCB_clicked\0"
    "checked\0on_dumpfnPB_clicked\0loaderFinished\0"
    "on_flasherfnPB_clicked\0on_action_Qt_triggered\0"
    "on_about_triggered\0logText\0logStatus\0"
    "updateComPortCB"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MdrFwUart[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   77,    2, 0x08 /* Private */,
       6,    0,   78,    2, 0x08 /* Private */,
       7,    1,   79,    2, 0x08 /* Private */,
       9,    0,   82,    2, 0x08 /* Private */,
      10,    0,   83,    2, 0x08 /* Private */,
      11,    0,   84,    2, 0x08 /* Private */,
      12,    0,   85,    2, 0x08 /* Private */,
      13,    0,   86,    2, 0x08 /* Private */,
      14,    1,   87,    2, 0x08 /* Private */,
      15,    1,   90,    2, 0x08 /* Private */,
      16,    0,   93,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,

       0        // eod
};

void MdrFwUart::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MdrFwUart *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->operate((*reinterpret_cast< VasiliLoaderClient::WorkType(*)>(_a[1]))); break;
        case 1: _t->on_openFilePB_clicked(); break;
        case 2: _t->on_startStopPB_clicked(); break;
        case 3: _t->on_dumpCB_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->on_dumpfnPB_clicked(); break;
        case 5: _t->loaderFinished(); break;
        case 6: _t->on_flasherfnPB_clicked(); break;
        case 7: _t->on_action_Qt_triggered(); break;
        case 8: _t->on_about_triggered(); break;
        case 9: _t->logText((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: _t->logStatus((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: _t->updateComPortCB(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MdrFwUart::*)(VasiliLoaderClient::WorkType );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MdrFwUart::operate)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MdrFwUart::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_MdrFwUart.data,
    qt_meta_data_MdrFwUart,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MdrFwUart::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MdrFwUart::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MdrFwUart.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MdrFwUart::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void MdrFwUart::operate(VasiliLoaderClient::WorkType _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
