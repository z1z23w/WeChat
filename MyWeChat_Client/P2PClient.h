#ifndef P2PCLIENT_H
#define P2PCLIENT_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QQmlEngine>
#include <QNetworkInterface>
#include <QJsonObject>

class P2PClient : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)
    Q_PROPERTY(bool isLoggedIn READ isLoggedIn NOTIFY isLoggedInChanged)
    // 【新增】暴露好友列表给 QML
    Q_PROPERTY(QStringList onlineUsers READ onlineUsers NOTIFY onlineUsersChanged)

public:
    explicit P2PClient(QObject *parent = nullptr);

    Q_INVOKABLE void loginToServer(QString username, QString password, QString serverIp);

    // 【修改】发送消息增加目标用户
    Q_INVOKABLE void sendMessageTo(QString targetUser, QString msg);

    QString status() const { return m_status; }
    bool isLoggedIn() const { return m_isLoggedIn; }
    QStringList onlineUsers() const { return m_onlineUsersList; }

signals:
    void statusChanged();
    void messageReceived(QString msg);
    void isLoggedInChanged();
    void onlineUsersChanged(); // 列表变动信号

private slots:
    void onReadyRead();
    void onSocketError(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket *m_socket;
    QString m_status;
    bool m_isLoggedIn = false;
    QStringList m_onlineUsersList; // 本地存储好友列表

    void setStatus(QString newStatus);
    void handleServerMessage(const QByteArray &data);
};

#endif // P2PCLIENT_H
