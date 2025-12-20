#ifndef P2PCLIENT_H
#define P2PCLIENT_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QQmlEngine>
#include <QNetworkInterface>

class P2PClient : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    // 暴露给 QML 的属性
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)
    Q_PROPERTY(QString localIp READ localIp CONSTANT)
    Q_PROPERTY(bool isListening READ isListening NOTIFY isListeningChanged)

public:
    explicit P2PClient(QObject *parent = nullptr);

    Q_INVOKABLE void startListening(int port);
    Q_INVOKABLE void connectToPeer(QString ip, int port);
    Q_INVOKABLE void sendMessage(QString msg);

    QString status() const { return m_status; }
    QString localIp() const;
    bool isListening() const;

signals:
    void statusChanged();
    void messageReceived(QString msg);
    void isListeningChanged();

private slots:
    void onNewConnection();
    void onReadyRead();
    void onSocketError(QAbstractSocket::SocketError socketError);

private:
    QTcpServer *m_server;
    QTcpSocket *m_socket;
    QString m_status;

    void setStatus(QString newStatus);
};

#endif // P2PCLIENT_H
