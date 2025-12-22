#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QMap>
#include <QJsonObject>

class ChatServer : public QObject
{
    Q_OBJECT
public:
    explicit ChatServer(QObject *parent = nullptr);
    bool start(int port);

private slots:
    void onNewConnection();
    void onReadyRead();
    void onClientDisconnected();

private:
    QTcpServer *m_server;

    // 在线用户映射表
    QMap<QString, QTcpSocket*> m_onlineUsers;
    QMap<QTcpSocket*, QString> m_socketToUser;

    void sendJson(QTcpSocket *socket, const QJsonObject &obj);

    // --- 业务处理函数群 (Handler) ---
    void handleRegister(QTcpSocket *socket, const QJsonObject &req);
    void handleLogin(QTcpSocket *socket, const QJsonObject &req);
    void handleMessage(QTcpSocket *socket, const QJsonObject &req);

    // 好友 CRUD Handler
    void handleSearchUser(QTcpSocket *socket, const QJsonObject &req);
    void handleAddFriend(QTcpSocket *socket, const QJsonObject &req);
    void handleDeleteFriend(QTcpSocket *socket, const QJsonObject &req);
    void handleUpdateFriend(QTcpSocket *socket, const QJsonObject &req);

    void sendFriendList(QTcpSocket *socket, int userId);
};

#endif // CHATSERVER_H
