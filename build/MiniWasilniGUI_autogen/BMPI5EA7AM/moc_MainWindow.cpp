/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.16)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../include/gui/MainWindow.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.16. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[23];
    char stringdata0[282];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 9), // "saveGraph"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 9), // "loadGraph"
QT_MOC_LITERAL(4, 32, 11), // "exportToSVG"
QT_MOC_LITERAL(5, 44, 7), // "addCity"
QT_MOC_LITERAL(6, 52, 10), // "deleteCity"
QT_MOC_LITERAL(7, 63, 7), // "addEdge"
QT_MOC_LITERAL(8, 71, 10), // "deleteEdge"
QT_MOC_LITERAL(9, 82, 10), // "performDFS"
QT_MOC_LITERAL(10, 93, 10), // "performBFS"
QT_MOC_LITERAL(11, 104, 21), // "visualizeSpanningTree"
QT_MOC_LITERAL(12, 126, 14), // "vector<string>"
QT_MOC_LITERAL(13, 141, 4), // "path"
QT_MOC_LITERAL(14, 146, 28), // "vector<pair<string,string> >"
QT_MOC_LITERAL(15, 175, 9), // "treeEdges"
QT_MOC_LITERAL(16, 185, 5), // "isDFS"
QT_MOC_LITERAL(17, 191, 16), // "findDijkstraPath"
QT_MOC_LITERAL(18, 208, 13), // "findAStarPath"
QT_MOC_LITERAL(19, 222, 22), // "handleNodeDragFinished"
QT_MOC_LITERAL(20, 245, 17), // "performAutoLayout"
QT_MOC_LITERAL(21, 263, 13), // "applyDarkMode"
QT_MOC_LITERAL(22, 277, 4) // "dark"

    },
    "MainWindow\0saveGraph\0\0loadGraph\0"
    "exportToSVG\0addCity\0deleteCity\0addEdge\0"
    "deleteEdge\0performDFS\0performBFS\0"
    "visualizeSpanningTree\0vector<string>\0"
    "path\0vector<pair<string,string> >\0"
    "treeEdges\0isDFS\0findDijkstraPath\0"
    "findAStarPath\0handleNodeDragFinished\0"
    "performAutoLayout\0applyDarkMode\0dark"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   89,    2, 0x08 /* Private */,
       3,    0,   90,    2, 0x08 /* Private */,
       4,    0,   91,    2, 0x08 /* Private */,
       5,    0,   92,    2, 0x08 /* Private */,
       6,    0,   93,    2, 0x08 /* Private */,
       7,    0,   94,    2, 0x08 /* Private */,
       8,    0,   95,    2, 0x08 /* Private */,
       9,    0,   96,    2, 0x08 /* Private */,
      10,    0,   97,    2, 0x08 /* Private */,
      11,    3,   98,    2, 0x08 /* Private */,
      17,    0,  105,    2, 0x08 /* Private */,
      18,    0,  106,    2, 0x08 /* Private */,
      19,    0,  107,    2, 0x08 /* Private */,
      20,    0,  108,    2, 0x08 /* Private */,
      21,    1,  109,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 12, 0x80000000 | 14, QMetaType::Bool,   13,   15,   16,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   22,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->saveGraph(); break;
        case 1: _t->loadGraph(); break;
        case 2: _t->exportToSVG(); break;
        case 3: _t->addCity(); break;
        case 4: _t->deleteCity(); break;
        case 5: _t->addEdge(); break;
        case 6: _t->deleteEdge(); break;
        case 7: _t->performDFS(); break;
        case 8: _t->performBFS(); break;
        case 9: _t->visualizeSpanningTree((*reinterpret_cast< const vector<string>(*)>(_a[1])),(*reinterpret_cast< const vector<pair<string,string> >(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 10: _t->findDijkstraPath(); break;
        case 11: _t->findAStarPath(); break;
        case 12: _t->handleNodeDragFinished(); break;
        case 13: _t->performAutoLayout(); break;
        case 14: _t->applyDarkMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


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
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
