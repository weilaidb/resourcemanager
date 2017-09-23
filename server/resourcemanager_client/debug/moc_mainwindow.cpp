/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[28];
    char stringdata[298];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 11),
QT_MOC_LITERAL(2, 23, 0),
QT_MOC_LITERAL(3, 24, 14),
QT_MOC_LITERAL(4, 39, 6),
QT_MOC_LITERAL(5, 46, 10),
QT_MOC_LITERAL(6, 57, 6),
QT_MOC_LITERAL(7, 64, 9),
QT_MOC_LITERAL(8, 74, 16),
QT_MOC_LITERAL(9, 91, 12),
QT_MOC_LITERAL(10, 104, 7),
QT_MOC_LITERAL(11, 112, 5),
QT_MOC_LITERAL(12, 118, 5),
QT_MOC_LITERAL(13, 124, 4),
QT_MOC_LITERAL(14, 129, 9),
QT_MOC_LITERAL(15, 139, 3),
QT_MOC_LITERAL(16, 143, 8),
QT_MOC_LITERAL(17, 152, 6),
QT_MOC_LITERAL(18, 159, 19),
QT_MOC_LITERAL(19, 179, 4),
QT_MOC_LITERAL(20, 184, 20),
QT_MOC_LITERAL(21, 205, 13),
QT_MOC_LITERAL(22, 219, 19),
QT_MOC_LITERAL(23, 239, 14),
QT_MOC_LITERAL(24, 254, 1),
QT_MOC_LITERAL(25, 256, 16),
QT_MOC_LITERAL(26, 273, 2),
QT_MOC_LITERAL(27, 276, 21)
    },
    "MainWindow\0hellosocket\0\0readfromremote\0"
    "cltmsg\0procErrMsg\0errmsg\0showTitle\0"
    "deleteBeforeShow\0showOneRowUI\0devname\0"
    "devip\0netip\0time\0timeindex\0usr\0usrindex\0"
    "notice\0Proc_RequestSrcItem\0text\0"
    "test_showoneresource\0showResources\0"
    "T_ResourceUse_Print\0T_ResourceUse*\0p\0"
    "setPushBtnEnable\0st\0on_pushButton_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x0a /* Public */,
       3,    2,   75,    2, 0x0a /* Public */,
       5,    2,   80,    2, 0x0a /* Public */,
       7,    0,   85,    2, 0x0a /* Public */,
       8,    0,   86,    2, 0x0a /* Public */,
       9,    8,   87,    2, 0x0a /* Public */,
      18,    1,  104,    2, 0x0a /* Public */,
      20,    1,  107,    2, 0x0a /* Public */,
      21,    1,  110,    2, 0x0a /* Public */,
      22,    1,  113,    2, 0x0a /* Public */,
      25,    1,  116,    2, 0x0a /* Public */,
      27,    0,  119,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::VoidStar,    4,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::VoidStar,    6,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QStringList, QMetaType::QString, QMetaType::QStringList, QMetaType::QString, QMetaType::QString,   10,   11,   12,   13,   14,   15,   16,   17,
    QMetaType::Void, QMetaType::QString,   19,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, 0x80000000 | 23,   24,
    QMetaType::Void, QMetaType::QString,   26,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->hellosocket(); break;
        case 1: _t->readfromremote((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< void*(*)>(_a[2]))); break;
        case 2: _t->procErrMsg((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< void*(*)>(_a[2]))); break;
        case 3: _t->showTitle(); break;
        case 4: _t->deleteBeforeShow(); break;
        case 5: _t->showOneRowUI((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QStringList(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])),(*reinterpret_cast< QStringList(*)>(_a[6])),(*reinterpret_cast< QString(*)>(_a[7])),(*reinterpret_cast< QString(*)>(_a[8]))); break;
        case 6: _t->Proc_RequestSrcItem((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->test_showoneresource((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->showResources((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->T_ResourceUse_Print((*reinterpret_cast< T_ResourceUse*(*)>(_a[1]))); break;
        case 10: _t->setPushBtnEnable((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: _t->on_pushButton_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, 0, 0}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_END_MOC_NAMESPACE
