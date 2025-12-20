/****************************************************************************
** Meta object code from reading C++ file 'P2PClient.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../P2PClient.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'P2PClient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.3. It"
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
struct qt_meta_tag_ZN9P2PClientE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN9P2PClientE = QtMocHelpers::stringData(
    "P2PClient",
    "QML.Element",
    "auto",
    "statusChanged",
    "",
    "messageReceived",
    "msg",
    "isLoggedInChanged",
    "onlineUsersChanged",
    "onReadyRead",
    "onSocketError",
    "QAbstractSocket::SocketError",
    "socketError",
    "loginToServer",
    "username",
    "password",
    "serverIp",
    "sendMessageTo",
    "targetUser",
    "status",
    "isLoggedIn",
    "onlineUsers"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN9P2PClientE[] = {

 // content:
      12,       // revision
       0,       // classname
       1,   14, // classinfo
       8,   16, // methods
       3,   86, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // classinfo: key, value
       1,    2,

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       3,    0,   64,    4, 0x06,    4 /* Public */,
       5,    1,   65,    4, 0x06,    5 /* Public */,
       7,    0,   68,    4, 0x06,    7 /* Public */,
       8,    0,   69,    4, 0x06,    8 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       9,    0,   70,    4, 0x08,    9 /* Private */,
      10,    1,   71,    4, 0x08,   10 /* Private */,

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
      13,    3,   74,    4, 0x02,   12 /* Public */,
      17,    2,   81,    4, 0x02,   16 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 11,   12,

 // methods: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,   14,   15,   16,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   18,    6,

 // properties: name, type, flags, notifyId, revision
      19, QMetaType::QString, 0x00015001, uint(0), 0,
      20, QMetaType::Bool, 0x00015001, uint(2), 0,
      21, QMetaType::QStringList, 0x00015001, uint(3), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject P2PClient::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ZN9P2PClientE.offsetsAndSizes,
    qt_meta_data_ZN9P2PClientE,
    qt_static_metacall,
    nullptr,
    qt_metaTypeArray<
        // property 'status'
        QString,
        // property 'isLoggedIn'
        bool,
        // property 'onlineUsers'
        QStringList,
        // Q_OBJECT / Q_GADGET
        P2PClient,
        // method 'statusChanged'
        void,
        // method 'messageReceived'
        void,
        QString,
        // method 'isLoggedInChanged'
        void,
        // method 'onlineUsersChanged'
        void,
        // method 'onReadyRead'
        void,
        // method 'onSocketError'
        void,
        QAbstractSocket::SocketError,
        // method 'loginToServer'
        void,
        QString,
        QString,
        QString,
        // method 'sendMessageTo'
        void,
        QString,
        QString
    >,
    nullptr
} };

void P2PClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<P2PClient *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->statusChanged(); break;
        case 1: _t->messageReceived((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->isLoggedInChanged(); break;
        case 3: _t->onlineUsersChanged(); break;
        case 4: _t->onReadyRead(); break;
        case 5: _t->onSocketError((*reinterpret_cast< std::add_pointer_t<QAbstractSocket::SocketError>>(_a[1]))); break;
        case 6: _t->loginToServer((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3]))); break;
        case 7: _t->sendMessageTo((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _q_method_type = void (P2PClient::*)();
            if (_q_method_type _q_method = &P2PClient::statusChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _q_method_type = void (P2PClient::*)(QString );
            if (_q_method_type _q_method = &P2PClient::messageReceived; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _q_method_type = void (P2PClient::*)();
            if (_q_method_type _q_method = &P2PClient::isLoggedInChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _q_method_type = void (P2PClient::*)();
            if (_q_method_type _q_method = &P2PClient::onlineUsersChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->status(); break;
        case 1: *reinterpret_cast< bool*>(_v) = _t->isLoggedIn(); break;
        case 2: *reinterpret_cast< QStringList*>(_v) = _t->onlineUsers(); break;
        default: break;
        }
    }
}

const QMetaObject *P2PClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *P2PClient::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN9P2PClientE.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int P2PClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void P2PClient::statusChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void P2PClient::messageReceived(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void P2PClient::isLoggedInChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void P2PClient::onlineUsersChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
