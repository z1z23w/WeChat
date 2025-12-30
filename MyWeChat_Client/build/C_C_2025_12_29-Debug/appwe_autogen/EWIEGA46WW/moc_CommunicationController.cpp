/****************************************************************************
** Meta object code from reading C++ file 'CommunicationController.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../CommunicationController.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CommunicationController.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN23CommunicationControllerE_t {};
} // unnamed namespace

template <> constexpr inline auto CommunicationController::qt_create_metaobjectdata<qt_meta_tag_ZN23CommunicationControllerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "CommunicationController",
        "friendListChanged",
        "",
        "connectionStatusChanged",
        "messageReceived",
        "from",
        "content",
        "notificationTriggered",
        "title",
        "msg",
        "onReadyRead",
        "onMediaData",
        "data",
        "onConnected",
        "connectToServer",
        "ip",
        "login",
        "u",
        "p",
        "registerUser",
        "sendMessage",
        "startMediaSession",
        "endMediaSession",
        "selectFriend",
        "name",
        "searchUser",
        "keyword",
        "addFriend",
        "friendName",
        "currentUser",
        "Netizen*",
        "currentSession",
        "ChatSession*",
        "friendList",
        "QJsonArray",
        "isConnected"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'friendListChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'connectionStatusChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'messageReceived'
        QtMocHelpers::SignalData<void(QString, QString)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 5 }, { QMetaType::QString, 6 },
        }}),
        // Signal 'notificationTriggered'
        QtMocHelpers::SignalData<void(QString, QString)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 8 }, { QMetaType::QString, 9 },
        }}),
        // Slot 'onReadyRead'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onMediaData'
        QtMocHelpers::SlotData<void(QByteArray)>(11, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QByteArray, 12 },
        }}),
        // Slot 'onConnected'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Method 'connectToServer'
        QtMocHelpers::MethodData<void(QString)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 15 },
        }}),
        // Method 'login'
        QtMocHelpers::MethodData<void(QString, QString)>(16, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 17 }, { QMetaType::QString, 18 },
        }}),
        // Method 'registerUser'
        QtMocHelpers::MethodData<void(QString, QString)>(19, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 17 }, { QMetaType::QString, 18 },
        }}),
        // Method 'sendMessage'
        QtMocHelpers::MethodData<void(QString)>(20, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 6 },
        }}),
        // Method 'startMediaSession'
        QtMocHelpers::MethodData<void()>(21, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'endMediaSession'
        QtMocHelpers::MethodData<void()>(22, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'selectFriend'
        QtMocHelpers::MethodData<void(QString)>(23, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 24 },
        }}),
        // Method 'searchUser'
        QtMocHelpers::MethodData<void(QString)>(25, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 26 },
        }}),
        // Method 'addFriend'
        QtMocHelpers::MethodData<void(QString)>(27, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 28 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'currentUser'
        QtMocHelpers::PropertyData<Netizen*>(29, 0x80000000 | 30, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'currentSession'
        QtMocHelpers::PropertyData<ChatSession*>(31, 0x80000000 | 32, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'friendList'
        QtMocHelpers::PropertyData<QJsonArray>(33, 0x80000000 | 34, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'isConnected'
        QtMocHelpers::PropertyData<bool>(35, QMetaType::Bool, QMC::DefaultPropertyFlags, 1),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<CommunicationController, qt_meta_tag_ZN23CommunicationControllerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject CommunicationController::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN23CommunicationControllerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN23CommunicationControllerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN23CommunicationControllerE_t>.metaTypes,
    nullptr
} };

void CommunicationController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<CommunicationController *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->friendListChanged(); break;
        case 1: _t->connectionStatusChanged(); break;
        case 2: _t->messageReceived((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 3: _t->notificationTriggered((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 4: _t->onReadyRead(); break;
        case 5: _t->onMediaData((*reinterpret_cast<std::add_pointer_t<QByteArray>>(_a[1]))); break;
        case 6: _t->onConnected(); break;
        case 7: _t->connectToServer((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 8: _t->login((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 9: _t->registerUser((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 10: _t->sendMessage((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 11: _t->startMediaSession(); break;
        case 12: _t->endMediaSession(); break;
        case 13: _t->selectFriend((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 14: _t->searchUser((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 15: _t->addFriend((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (CommunicationController::*)()>(_a, &CommunicationController::friendListChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (CommunicationController::*)()>(_a, &CommunicationController::connectionStatusChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (CommunicationController::*)(QString , QString )>(_a, &CommunicationController::messageReceived, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (CommunicationController::*)(QString , QString )>(_a, &CommunicationController::notificationTriggered, 3))
            return;
    }
    if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< ChatSession* >(); break;
        case 0:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Netizen* >(); break;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<Netizen**>(_v) = _t->currentUser(); break;
        case 1: *reinterpret_cast<ChatSession**>(_v) = _t->currentSession(); break;
        case 2: *reinterpret_cast<QJsonArray*>(_v) = _t->friendList(); break;
        case 3: *reinterpret_cast<bool*>(_v) = _t->isConnected(); break;
        default: break;
        }
    }
}

const QMetaObject *CommunicationController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CommunicationController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN23CommunicationControllerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CommunicationController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 16;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void CommunicationController::friendListChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void CommunicationController::connectionStatusChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void CommunicationController::messageReceived(QString _t1, QString _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2);
}

// SIGNAL 3
void CommunicationController::notificationTriggered(QString _t1, QString _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1, _t2);
}
QT_WARNING_POP
