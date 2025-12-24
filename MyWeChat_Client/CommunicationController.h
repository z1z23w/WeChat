#ifndef COMMUNICATIONCONTROLLER_H
#define COMMUNICATIONCONTROLLER_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonArray>
#include <QJsonObject>
#include "Netizen.h"
#include "ChatSession.h"
#include "ComplianceController.h"
#include "MediaSessionController.h"

class CommunicationController : public QObject {
    Q_OBJECT
    Q_PROPERTY(Netizen* currentUser READ currentUser CONSTANT)
    Q_PROPERTY(ChatSession* currentSession READ currentSession CONSTANT)
    Q_PROPERTY(QJsonArray friendList READ friendList NOTIFY friendListChanged)
    Q_PROPERTY(bool isConnected READ isConnected NOTIFY connectionStatusChanged)

public:
    explicit CommunicationController(QObject *parent = nullptr);

    // --- UI 调用接口 ---
    Q_INVOKABLE void connectToServer(QString ip);
    Q_INVOKABLE void login(QString u, QString p);
    Q_INVOKABLE void registerUser(QString u, QString p);
    Q_INVOKABLE void sendMessage(QString content);
    Q_INVOKABLE void startMediaSession();
    Q_INVOKABLE void endMediaSession();
    Q_INVOKABLE void selectFriend(QString name);
    Q_INVOKABLE void searchUser(QString keyword);
    Q_INVOKABLE void addFriend(QString friendName);

    // Getters
    Netizen* currentUser() const { return m_currentUser; }
    ChatSession* currentSession() const { return m_chatSession; }
    QJsonArray friendList() const { return m_friendList; }
    bool isConnected() const;

signals:
    void friendListChanged();
    void connectionStatusChanged();
    void messageReceived(QString from, QString content);
    void notificationTriggered(QString title, QString msg);

private slots:
    void onReadyRead();
    void onMediaData(QByteArray data);

    // 【新增】连接建立后的回调
    void onConnected();

private:
    QTcpSocket *m_socket;
    Netizen *m_currentUser;
    ChatSession *m_chatSession;
    ComplianceController *m_complianceCtrl;
    MediaSessionController *m_mediaCtrl;

    QJsonArray m_friendList;

    // 【新增】暂存未发送的数据（解决异步连接问题）
    QJsonObject m_pendingData;

    void sendJson(const QJsonObject &json);
    void handleData(const QByteArray &data);
};

#endif // COMMUNICATIONCONTROLLER_H
