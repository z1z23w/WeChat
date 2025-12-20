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

    // 【核心数据结构】
    // 1. 根据用户名找到 Socket (用于转发消息)
    QMap<QString, QTcpSocket*> m_onlineUsers;
    // 2. 根据 Socket 找到用户名 (用于断开连接时清理)
    QMap<QTcpSocket*, QString> m_socketToUser;

    void sendJson(QTcpSocket *socket, const QJsonObject &obj);
    void handleLogin(QTcpSocket *socket, const QJsonObject &obj);
    void handleMessage(QTcpSocket *socket, const QJsonObject &obj);
    void broadcastUserList(); // 广播好友列表给所有人
};

#endif // CHATSERVER_H
