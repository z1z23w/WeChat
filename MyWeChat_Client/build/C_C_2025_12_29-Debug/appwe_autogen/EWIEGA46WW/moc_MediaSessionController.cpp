/****************************************************************************
** Meta object code from reading C++ file 'MediaSessionController.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../MediaSessionController.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MediaSessionController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN22MediaSessionControllerE_t {};
} // unnamed namespace

template <> constexpr inline auto MediaSessionController::qt_create_metaobjectdata<qt_meta_tag_ZN22MediaSessionControllerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "MediaSessionController",
        "mediaStreamGenerated",
        "",
        "data",
        "onAudioData"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'mediaStreamGenerated'
        QtMocHelpers::SignalData<void(QByteArray)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QByteArray, 3 },
        }}),
        // Slot 'onAudioData'
        QtMocHelpers::SlotData<void(QByteArray)>(4, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QByteArray, 3 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<MediaSessionController, qt_meta_tag_ZN22MediaSessionControllerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject MediaSessionController::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN22MediaSessionControllerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN22MediaSessionControllerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN22MediaSessionControllerE_t>.metaTypes,
    nullptr
} };

void MediaSessionController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MediaSessionController *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->mediaStreamGenerated((*reinterpret_cast<std::add_pointer_t<QByteArray>>(_a[1]))); break;
        case 1: _t->onAudioData((*reinterpret_cast<std::add_pointer_t<QByteArray>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (MediaSessionController::*)(QByteArray )>(_a, &MediaSessionController::mediaStreamGenerated, 0))
            return;
    }
}

const QMetaObject *MediaSessionController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MediaSessionController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN22MediaSessionControllerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int MediaSessionController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void MediaSessionController::mediaStreamGenerated(QByteArray _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}
QT_WARNING_POP
