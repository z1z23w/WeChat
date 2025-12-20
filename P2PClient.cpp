#include "P2PClient.h"
#include <QNetworkProxy>

P2PClient::P2PClient(QObject *parent)
    : QObject(parent), m_server(new QTcpServer(this)), m_socket(nullptr)
{
    setStatus("等待操作...");
}

QString P2PClient::localIp() const {
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress &address : QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost)
            return address.toString();
    }
    return "127.0.0.1";
}

bool P2PClient::isListening() const {
    return m_server->isListening();
}

void P2PClient::startListening(int port)
{
    if (m_server->isListening()) return;

    if (m_server->listen(QHostAddress::Any, port)) {
        connect(m_server, &QTcpServer::newConnection, this, &P2PClient::onNewConnection);
        setStatus("服务已开启! 监听端口: " + QString::number(port));
        emit isListeningChanged();
    } else {
        setStatus("开启失败: " + m_server->errorString());
    }
}

void P2PClient::connectToPeer(QString ip, int port)
{
    if (m_socket) {
        m_socket->abort();
        m_socket->deleteLater();
    }

    m_socket = new QTcpSocket(this);

    // 【关键修复】禁用代理，防止 VPN/加速器 拦截局域网连接
    m_socket->setProxy(QNetworkProxy::NoProxy);

    connect(m_socket, &QTcpSocket::connected, this, [this](){
        setStatus("成功连接到对方!");
    });
    connect(m_socket, &QTcpSocket::readyRead, this, &P2PClient::onReadyRead);
    connect(m_socket, &QTcpSocket::errorOccurred, this, &P2PClient::onSocketError);

    setStatus("正在尝试连接 " + ip + "...");
    m_socket->connectToHost(ip, port);
}

void P2PClient::sendMessage(QString msg)
{
    if (m_socket && m_socket->state() == QAbstractSocket::ConnectedState) {
        m_socket->write(msg.toUtf8());
    } else {
        setStatus("发送失败: 未连接");
    }
}

void P2PClient::onNewConnection()
{
    if (m_socket && m_socket->state() == QAbstractSocket::ConnectedState) return;

    m_socket = m_server->nextPendingConnection();
    connect(m_socket, &QTcpSocket::readyRead, this, &P2PClient::onReadyRead);
    connect(m_socket, &QTcpSocket::disconnected, m_socket, &QTcpSocket::deleteLater);
    connect(m_socket, &QTcpSocket::errorOccurred, this, &P2PClient::onSocketError);

    setStatus("对方已连接进来!");
}

void P2PClient::onReadyRead()
{
    QByteArray data = m_socket->readAll();
    emit messageReceived(QString::fromUtf8(data));
}

void P2PClient::onSocketError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);
    if(m_socket) setStatus("网络错误: " + m_socket->errorString());
}

void P2PClient::setStatus(QString newStatus)
{
    m_status = newStatus;
    emit statusChanged();
}
