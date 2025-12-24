#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonArray>
#include <QJsonObject>
#include "MessageStorage.h"
#include "AudioManager.h" // 【引入音频类】

class NetworkManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isLoggedIn READ isLoggedIn NOTIFY isLoggedInChanged)
    Q_PROPERTY(QJsonArray friendList READ friendList NOTIFY friendListChanged)
    Q_PROPERTY(int myUserId READ myUserId NOTIFY myUserIdChanged)

public:
    explicit NetworkManager(QObject *parent = nullptr);

    // 基础网络接口
    Q_INVOKABLE void connectToServer(QString ip);
    Q_INVOKABLE void registerUser(QString username, QString password);
    Q_INVOKABLE void login(QString username, QString password);
    Q_INVOKABLE void sendMessage(QString toUser, QString content);

    // 好友接口
    Q_INVOKABLE void searchUser(QString keyword);
    Q_INVOKABLE void addFriend(QString friendName);
    Q_INVOKABLE void deleteFriend(QString friendName);
    Q_INVOKABLE void updateRemark(QString friendName, QString newRemark);

    // 数据接口
    Q_INVOKABLE QJsonArray getChatHistory(QString friendName);
    Q_INVOKABLE void clearUnread(QString friendName);

    // 语音接口
    Q_INVOKABLE void startVoice(QString targetUser); // 按下
    Q_INVOKABLE void stopVoice();                    // 松开

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

    // 处理音频数据
    void onAudioReady(QByteArray data);

private:
    QTcpSocket *m_socket;
    MessageStorage *m_storage;

    AudioManager *m_audio;
    QString m_currentVoiceTarget; // 记录语音

    bool m_isLoggedIn = false;
    int m_myUserId = -1;
    QJsonArray m_friendList;
    QJsonArray m_rawFriendList;
    QJsonObject m_pendingData;

    void handleData(const QByteArray &data);
    void sendJson(const QJsonObject &json);
    void refreshFriendList();
};

#endif // NETWORKMANAGER_H
