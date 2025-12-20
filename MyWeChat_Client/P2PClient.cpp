#include "P2PClient.h"
#include <QNetworkProxy>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

P2PClient::P2PClient(QObject *parent)
    : QObject(parent), m_socket(nullptr)
{
    setStatus("请登录...");
}

void P2PClient::loginToServer(QString username, QString password, QString serverIp)
{
    if (m_socket) {
        m_socket->abort();
        m_socket->deleteLater();
    }
    m_socket = new QTcpSocket(this);
    m_socket->setProxy(QNetworkProxy::NoProxy);

    connect(m_socket, &QTcpSocket::connected, this, [=](){
        setStatus("连接成功，正在验证...");
        QJsonObject json;
        json["type"] = "login";
        json["username"] = username;
        json["password"] = password;
        QJsonDocument doc(json);
        m_socket->write(doc.toJson());
    });

    connect(m_socket, &QTcpSocket::readyRead, this, &P2PClient::onReadyRead);
    connect(m_socket, &QTcpSocket::errorOccurred, this, &P2PClient::onSocketError);

    setStatus("正在连接 " + serverIp + "...");
    m_socket->connectToHost(serverIp, 9999);
}

void P2PClient::sendMessageTo(QString targetUser, QString msg)
{
    if (!m_socket) return;
    QJsonObject json;
    json["type"] = "msg";
    json["to"] = targetUser;     // 发给谁
    json["content"] = msg;       // 内容
    QJsonDocument doc(json);
    m_socket->write(doc.toJson());
}

// P2PClient.cpp -> onReadyRead 函数

// 注意：需要在 P2PClient.h 的 private 里加一个变量：QByteArray m_buffer;
// 如果不想改头文件，可以用下面的 static 变量偷懒方法（仅限单 Socket 场景）

void P2PClient::onReadyRead()
{
    // 使用 static 缓存来处理分包/粘包 (注意：如果有多个连接需移到类成员变量 m_buffer)
    static QByteArray buffer;

    // 1. 把新收到的数据追加到缓存屁股后面
    buffer.append(m_socket->readAll());

    // 2. 循环处理缓存里的数据
    while (!buffer.isEmpty()) {

        // 简单的花括号计数法来寻找一个完整的 JSON 对象
        int openBrace = 0;
        int closeBrace = 0;
        int endIndex = -1;

        for (int i = 0; i < buffer.size(); i++) {
            if (buffer[i] == '{') openBrace++;
            if (buffer[i] == '}') closeBrace++;

            // 如果左右括号数量相等，且不为0，说明找到了一个完整的闭环 { ... }
            if (openBrace > 0 && openBrace == closeBrace) {
                endIndex = i;
                break;
            }
        }

        // 如果没找到完整的包，说明包还没收全，等待下一次数据来
        if (endIndex == -1) {
            break;
        }

        // 3. 截取这一个完整的 JSON 包
        QByteArray packet = buffer.left(endIndex + 1);

        // 4. 把已经处理的包从缓存中切掉，保留剩下的（如果有的话）
        buffer = buffer.mid(endIndex + 1).trimmed();

        // 5. 打印调试并处理
        qDebug() << "处理独立包:" << packet;
        handleServerMessage(packet);
    }
}

void P2PClient::handleServerMessage(const QByteArray &data)
{
    // 这里处理粘包问题比较复杂，暂用最简单方式处理单条 JSON
    // 如果数据量大可能需要分割，但在 Demo 中通常没问题
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if(doc.isNull()) return;

    QJsonObject obj = doc.object();
    QString type = obj.value("type").toString();

    if (type == "login_resp") {
        bool success = obj.value("success").toBool();
        if (success) {
            m_isLoggedIn = true;
            setStatus("登录成功!");
            emit isLoggedInChanged();
        } else {
            setStatus("登录失败: 账号或密码错误");
        }
    }
    else if (type == "user_list") {
        // 【核心】更新好友列表
        m_onlineUsersList.clear();
        QJsonArray arr = obj.value("users").toArray();
        for (auto val : arr) {
            m_onlineUsersList.append(val.toString());
        }
        emit onlineUsersChanged();
    }
    else if (type == "msg") {
        // 【核心】收到消息
        QString sender = obj.value("from").toString();
        QString content = obj.value("content").toString();
        // 格式化显示
        emit messageReceived(sender + ": " + content);
    }
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
