/****************************************************************************
** Meta object code from reading C++ file 'ImageViewer.h'
**
** Created: Wed Apr 10 16:23:28 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/QTPro/ImageViewer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ImageViewer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ImageViewer[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x08,
      24,   12,   12,   12, 0x08,
      35,   12,   12,   12, 0x08,
      42,   12,   12,   12, 0x08,
      51,   12,   12,   12, 0x08,
      61,   12,   12,   12, 0x08,
      74,   12,   12,   12, 0x08,
      88,   12,   12,   12, 0x08,
      96,   12,   12,   12, 0x08,
     113,   12,   12,   12, 0x08,
     132,   12,   12,   12, 0x08,
     154,  146,   12,   12, 0x08,
     182,   12,   12,   12, 0x08,
     200,   12,   12,   12, 0x08,
     225,   12,   12,   12, 0x08,
     241,   12,   12,   12, 0x08,
     266,   12,   12,   12, 0x08,
     284,   12,   12,   12, 0x08,
     305,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ImageViewer[] = {
    "ImageViewer\0\0openFile()\0newStage()\0"
    "save()\0zoomIn()\0zoomOut()\0normalSize()\0"
    "fitToWindow()\0about()\0toolBarVisible()\0"
    "statusBarVisible()\0dockVisible()\0"
    "visible\0dockVisibleFromDocker(bool)\0"
    "regionDetection()\0morphologicalOperation()\0"
    "inpaintMethod()\0textureSynthesisMethod()\0"
    "decomposeMethod()\0medianFilterMethod()\0"
    "closeApp()\0"
};

const QMetaObject ImageViewer::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_ImageViewer,
      qt_meta_data_ImageViewer, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ImageViewer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ImageViewer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ImageViewer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ImageViewer))
        return static_cast<void*>(const_cast< ImageViewer*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int ImageViewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: openFile(); break;
        case 1: newStage(); break;
        case 2: save(); break;
        case 3: zoomIn(); break;
        case 4: zoomOut(); break;
        case 5: normalSize(); break;
        case 6: fitToWindow(); break;
        case 7: about(); break;
        case 8: toolBarVisible(); break;
        case 9: statusBarVisible(); break;
        case 10: dockVisible(); break;
        case 11: dockVisibleFromDocker((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: regionDetection(); break;
        case 13: morphologicalOperation(); break;
        case 14: inpaintMethod(); break;
        case 15: textureSynthesisMethod(); break;
        case 16: decomposeMethod(); break;
        case 17: medianFilterMethod(); break;
        case 18: closeApp(); break;
        default: ;
        }
        _id -= 19;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
