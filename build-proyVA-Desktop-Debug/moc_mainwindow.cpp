/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../P1/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[18];
    char stringdata0[166];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 7), // "compute"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 18), // "start_stop_capture"
QT_MOC_LITERAL(4, 39, 5), // "start"
QT_MOC_LITERAL(5, 45, 17), // "change_color_gray"
QT_MOC_LITERAL(6, 63, 5), // "color"
QT_MOC_LITERAL(7, 69, 12), // "selectWindow"
QT_MOC_LITERAL(8, 82, 1), // "p"
QT_MOC_LITERAL(9, 84, 1), // "w"
QT_MOC_LITERAL(10, 86, 1), // "h"
QT_MOC_LITERAL(11, 88, 14), // "deselectWindow"
QT_MOC_LITERAL(12, 103, 8), // "loadFile"
QT_MOC_LITERAL(13, 112, 8), // "saveFile"
QT_MOC_LITERAL(14, 121, 8), // "copyFile"
QT_MOC_LITERAL(15, 130, 10), // "resizeFile"
QT_MOC_LITERAL(16, 141, 11), // "enlargeFile"
QT_MOC_LITERAL(17, 153, 12) // "warpZoomFile"

    },
    "MainWindow\0compute\0\0start_stop_capture\0"
    "start\0change_color_gray\0color\0"
    "selectWindow\0p\0w\0h\0deselectWindow\0"
    "loadFile\0saveFile\0copyFile\0resizeFile\0"
    "enlargeFile\0warpZoomFile"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x0a /* Public */,
       3,    1,   70,    2, 0x0a /* Public */,
       5,    1,   73,    2, 0x0a /* Public */,
       7,    3,   76,    2, 0x0a /* Public */,
      11,    0,   83,    2, 0x0a /* Public */,
      12,    0,   84,    2, 0x0a /* Public */,
      13,    0,   85,    2, 0x0a /* Public */,
      14,    0,   86,    2, 0x0a /* Public */,
      15,    0,   87,    2, 0x0a /* Public */,
      16,    0,   88,    2, 0x0a /* Public */,
      17,    0,   89,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void, QMetaType::QPointF, QMetaType::Int, QMetaType::Int,    8,    9,   10,
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
        case 0: _t->compute(); break;
        case 1: _t->start_stop_capture((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->change_color_gray((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->selectWindow((*reinterpret_cast< QPointF(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 4: _t->deselectWindow(); break;
        case 5: _t->loadFile(); break;
        case 6: _t->saveFile(); break;
        case 7: _t->copyFile(); break;
        case 8: _t->resizeFile(); break;
        case 9: _t->enlargeFile(); break;
        case 10: _t->warpZoomFile(); break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
