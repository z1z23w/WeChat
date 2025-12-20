#include "ChatServer.h"
#include "DBManager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

ChatServer::ChatServer(QObject *parent) : QObject(parent), m_server(new QTcpServer(this)) {}

bool ChatServer::start(int port) {
    if (!m_server->listen(QHostAddress::Any, port)) {
        qDebug() << "监听失败:" << m_server->errorString();
        return false;
    }
    connect(m_server, &QTcpServer::newConnection, this, &ChatServer::onNewConnection);
    qDebug() << "服务器监听中... 端口:" << port;
    return true;
}

void ChatServer::onNewConnection() {
    QTcpSocket *clientSocket = m_server->nextPendingConnection();
    connect(clientSocket, &QTcpSocket::readyRead, this, &ChatServer::onReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &ChatServer::onClientDisconnected);
    qDebug() << "新客户端连接:" << clientSocket->peerAddress().toString();
}

void ChatServer::onReadyRead() {
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    QByteArray data = socket->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) return;

    QJsonObject obj = doc.object();
    QString type = obj.value("type").toString();

    if (type == "login") {
        handleLogin(socket, obj);
    } else if (type == "msg") {
        handleMessage(socket, obj);
    }
}

// ChatServer.cpp

void ChatServer::handleLogin(QTcpSocket *socket, const QJsonObject &req) {
    QString user = req.value("username").toString();
    QString pass = req.value("password").toString();
    int userId = -1;

    // 1. 验证逻辑：先登录，失败则自动注册
    bool success = DBManager::instance().loginUser(user, pass, userId);
    if (!success) {
        // 尝试自动注册
        if (DBManager::instance().registerUser(user, pass)) {
            // 注册成功后再次登录获取 ID
            success = DBManager::instance().loginUser(user, pass, userId);
        }
    }

    // 2. 【关键第一步】不管成功失败，立刻发送登录结果给客户端
    // 必须先让客户端收到这个包，它才能从登录页跳转到主界面
    QJsonObject resp;
    resp["type"] = "login_resp";
    resp["success"] = success;
    resp["user_id"] = userId;

    sendJson(socket, resp); // 立即发送！

    // 3. 【关键第二步】登录成功后，才处理上线逻辑和广播
    if (success) {
        // 如果该用户已经在其他地方登录，先把旧连接踢掉（防止多端登录冲突）
        if (m_onlineUsers.contains(user)) {
            QTcpSocket* oldSocket = m_onlineUsers[user];
            if (oldSocket != socket) {
                oldSocket->disconnectFromHost();
            }
        }

        // 更新服务器内部的在线映射表
        m_onlineUsers[user] = socket;
        m_socketToUser[socket] = user;

        qDebug() << "用户上线成功:" << user << "ID:" << userId;

        // 最后：广播最新的好友列表给所有人（包括刚登录的这个人）
        // 此时客户端已经在主界面了，收到这个包正好刷新通讯录
        broadcastUserList();
    } else {
        qDebug() << "登录/自动注册失败:" << user;
    }
}

void ChatServer::handleMessage(QTcpSocket *senderSocket, const QJsonObject &req) {
    QString fromUser = m_socketToUser.value(senderSocket);
    QString toUser = req.value("to").toString();
    QString content = req.value("content").toString();

    qDebug() << "转发消息:" << fromUser << "->" << toUser << ":" << content;

    if (m_onlineUsers.contains(toUser)) {
        QTcpSocket *targetSocket = m_onlineUsers[toUser];

        QJsonObject msg;
        msg["type"] = "msg";
        msg["from"] = fromUser;
        msg["content"] = content;

        sendJson(targetSocket, msg);
    }
}

void ChatServer::broadcastUserList() {
    QJsonArray userList;
    for (auto user : m_onlineUsers.keys()) {
        userList.append(user);
    }

    QJsonObject pkg;
    pkg["type"] = "user_list";
    pkg["users"] = userList;

    for (auto socket : m_onlineUsers.values()) {
        sendJson(socket, pkg);
    }
}

// ChatServer.cpp

void ChatServer::onClientDisconnected() {
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    if (m_socketToUser.contains(socket)) {
        QString user = m_socketToUser[socket];

        // 【关键修复】只有当 map 里存的 socket 确实是当前断开的这个 socket 时，才移除用户
        // 防止：用户在新设备登录(Socket B)，旧设备(Socket A)断开，结果把 User1 从在线列表里删了
        if (m_onlineUsers.value(user) == socket) {
            qDebug() << "用户下线:" << user;
            m_onlineUsers.remove(user);
            broadcastUserList(); // 刷新列表
        } else {
            qDebug() << "旧连接断开 (用户已在新连接登录):" << user;
        }

        m_socketToUser.remove(socket);
    }
    socket->deleteLater();
}

void ChatServer::sendJson(QTcpSocket *socket, const QJsonObject &obj) {
    if (socket && socket->isOpen()) {
        QJsonDocument doc(obj);
        socket->write(doc.toJson());
        socket->flush(); // 【必须加这句】强制发送
    }
}
