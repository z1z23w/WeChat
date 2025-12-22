#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonArray>
#include <QJsonObject>
#include "MessageStorage.h"

// 【类名修改】NetworkManager：负责网络通信与业务调度
class NetworkManager : public QObject
{
    Q_OBJECT
    // 暴露给 QML 的属性
    Q_PROPERTY(bool isLoggedIn READ isLoggedIn NOTIFY isLoggedInChanged)
    Q_PROPERTY(QJsonArray friendList READ friendList NOTIFY friendListChanged)
    Q_PROPERTY(int myUserId READ myUserId NOTIFY myUserIdChanged)

public:
    explicit NetworkManager(QObject *parent = nullptr);

    // --- 网络连接 ---
    Q_INVOKABLE void connectToServer(QString ip);
    Q_INVOKABLE void registerUser(QString username, QString password);
    Q_INVOKABLE void login(QString username, QString password);

    // --- 消息发送 ---
    Q_INVOKABLE void sendMessage(QString toUser, QString content);

    // --- 好友管理 ---
    Q_INVOKABLE void searchUser(QString keyword);
    Q_INVOKABLE void addFriend(QString friendName);
    Q_INVOKABLE void deleteFriend(QString friendName);
    Q_INVOKABLE void updateRemark(QString friendName, QString newRemark);

    // --- 数据接口 (透传给 MessageStorage) ---
    Q_INVOKABLE QJsonArray getChatHistory(QString friendName);
    Q_INVOKABLE void clearUnread(QString friendName);

    // Getters
    bool isLoggedIn() const { return m_isLoggedIn; }
    int myUserId() const { return m_myUserId; }
    QJsonArray friendList() const { return m_friendList; }

signals:
    void isLoggedInChanged();
    void myUserIdChanged();
    void friendListChanged();

    void messageReceived(QString friendName, QString content, bool isMe);
    void registerResult(bool success, QString msg);
    void searchResult(bool found, QString username, int id);
    void operationResult(QString op, bool success, QString msg);

private slots:
    void onReadyRead();
    void onConnected();
    void onError(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket *m_socket;
    MessageStorage *m_storage; // 核心：持有数据存储对象

    bool m_isLoggedIn = false;
    int m_myUserId = -1;

    QJsonArray m_friendList;      // UI 显示用的列表
    QJsonArray m_rawFriendList;   // 原始数据缓存
    QJsonObject m_pendingData;    // 离线发送缓存

    void handleData(const QByteArray &data);
    void sendJson(const QJsonObject &json);
    void refreshFriendList();     // 核心：合并红点状态
};

#endif // NETWORKMANAGER_H
