/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Wed Sep 20 22:00:51 2017
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../resourcemanager_server/mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      41,   32,   11,   11, 0x0a,
      75,   11,   11,   11, 0x0a,
      99,   95,   11,   11, 0x0a,
     130,  123,   11,   11, 0x0a,
     154,   11,   11,   11, 0x08,
     178,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0procClientMessage()\0"
    "numBytes\0updateWriteClientProgress(qint64)\0"
    "readClientMessage()\0log\0logsappendShow(QString)\0"
    "cltmsg\0readfromremote(QString)\0"
    "on_pushButton_clicked()\0"
    "on_pushButton_flushipaddr_clicked()\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: procClientMessage(); break;
        case 1: updateWriteClientProgress((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 2: readClientMessage(); break;
        case 3: logsappendShow((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: readfromremote((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: on_pushButton_clicked(); break;
        case 6: on_pushButton_flushipaddr_clicked(); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
