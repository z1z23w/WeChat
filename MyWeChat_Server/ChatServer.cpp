#include "ChatServer.h"
#include "DBManager.h"
#include <QJsonDocument>
#include <QDebug>

ChatServer::ChatServer(QObject *parent) : QObject(parent), m_server(new QTcpServer(this)) {}

bool ChatServer::start(int port) {
    if (!m_server->listen(QHostAddress::Any, port)) {
        qDebug() << "监听失败:" << m_server->errorString();
        return false;
    }
    qDebug() << "服务器监听中... 端口:" << port;
    connect(m_server, &QTcpServer::newConnection, this, &ChatServer::onNewConnection);
    return true;
}

void ChatServer::onNewConnection() {
    QTcpSocket *clientSocket = m_server->nextPendingConnection();
    connect(clientSocket, &QTcpSocket::readyRead, this, &ChatServer::onReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &ChatServer::onClientDisconnected);
    qDebug() << "新连接:" << clientSocket->peerAddress().toString();
}

void ChatServer::onReadyRead() {
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    QByteArray data = socket->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) return;

    QJsonObject obj = doc.object();
    QString type = obj.value("type").toString();

    // 分发请求
    if (type == "register") handleRegister(socket, obj);
    else if (type == "login") handleLogin(socket, obj);
    else if (type == "msg") handleMessage(socket, obj);
    else if (type == "search_user") handleSearchUser(socket, obj);
    else if (type == "add_friend") handleAddFriend(socket, obj);
    else if (type == "del_friend") handleDeleteFriend(socket, obj);
    else if (type == "update_friend") handleUpdateFriend(socket, obj);
}

void ChatServer::handleRegister(QTcpSocket *socket, const QJsonObject &req) {
    QString user = req.value("username").toString();
    QString pass = req.value("password").toString();

    bool ok = DBManager::instance().registerUser(user, pass);

    QJsonObject resp;
    resp["type"] = "register_resp";
    resp["success"] = ok;
    resp["message"] = ok ? "注册成功" : "用户名已存在";
    sendJson(socket, resp);
}

void ChatServer::handleLogin(QTcpSocket *socket, const QJsonObject &req) {
    QString user = req.value("username").toString();
    QString pass = req.value("password").toString();
    int userId = -1;

    bool success = DBManager::instance().loginUser(user, pass, userId);

    QJsonObject resp;
    resp["type"] = "login_resp";
    resp["success"] = success;
    resp["user_id"] = userId;
    resp["username"] = user;
    sendJson(socket, resp);

    if (success) {
        // 踢掉旧连接
        if (m_onlineUsers.contains(user)) {
            QTcpSocket* old = m_onlineUsers[user];
            if (old != socket) old->disconnectFromHost();
        }
        m_onlineUsers[user] = socket;
        m_socketToUser[socket] = user;

        qDebug() << "用户上线:" << user;
        sendFriendList(socket, userId); // 登录成功发好友列表
    }
}

void ChatServer::handleMessage(QTcpSocket *senderSocket, const QJsonObject &req) {
    QString fromUser = m_socketToUser.value(senderSocket);
    QString toUser = req.value("to").toString();
    QString content = req.value("content").toString();

    // 转发给目标
    if (m_onlineUsers.contains(toUser)) {
        QJsonObject msg;
        msg["type"] = "msg";
        msg["from"] = fromUser;
        msg["content"] = content;
        sendJson(m_onlineUsers[toUser], msg);
    }
}

// --- CRUD Handlers ---

void ChatServer::handleSearchUser(QTcpSocket *socket, const QJsonObject &req) {
    QString keyword = req.value("keyword").toString();
    QJsonObject result;
    bool found = DBManager::instance().searchUser(keyword, result);

    QJsonObject resp;
    resp["type"] = "search_user_resp";
    resp["found"] = found;
    if(found) resp["user"] = result;
    sendJson(socket, resp);
}


void ChatServer::handleAddFriend(QTcpSocket *socket, const QJsonObject &req) {
    int myId = req.value("user_id").toInt();
    QString friendName = req.value("friend_name").toString();
    QString myName = m_socketToUser.value(socket); // 获取我自己的名字

    // 1. 执行双向添加
    bool ok = DBManager::instance().addFriend(myId, friendName);

    // 2. 回复给“发起者”
    QJsonObject resp;
    resp["type"] = "op_resp";
    resp["op"] = "add";
    resp["success"] = ok;
    resp["message"] = ok ? "添加成功" : "失败 (用户不存在或已添加)";
    sendJson(socket, resp);

    if (ok) {
        // 3. 刷新“我”的好友列表
        sendFriendList(socket, myId);

        if (m_onlineUsers.contains(friendName)) {
            QTcpSocket* friendSocket = m_onlineUsers[friendName];

            // 查出对方的 ID
            int friendId = DBManager::instance().getUserId(friendName);

            // 给对方发送最新的好友列表
            sendFriendList(friendSocket, friendId);

            // 可选：给对方发个系统通知消息
            QJsonObject notify;
            notify["type"] = "msg";
            notify["from"] = "系统通知";
            notify["content"] = myName + " 已添加你为好友";
            sendJson(friendSocket, notify);
        }
    }
}

void ChatServer::handleDeleteFriend(QTcpSocket *socket, const QJsonObject &req) {
    int myId = req.value("user_id").toInt();
    QString friendName = req.value("friend_name").toString();

    bool ok = DBManager::instance().deleteFriend(myId, friendName);
    if (ok) sendFriendList(socket, myId);
}

void ChatServer::handleUpdateFriend(QTcpSocket *socket, const QJsonObject &req) {
    int myId = req.value("user_id").toInt();
    QString friendName = req.value("friend_name").toString();
    QString remark = req.value("remark").toString();

    bool ok = DBManager::instance().updateFriendRemark(myId, friendName, remark);
    if (ok) sendFriendList(socket, myId);
}

void ChatServer::sendFriendList(QTcpSocket *socket, int userId) {
    QJsonArray friends = DBManager::instance().getFriendList(userId);

    // 附加在线状态
    QJsonArray finalArr;
    for(auto v : friends) {
        QJsonObject f = v.toObject();
        QString originalName = f["username"].toString();
        f["online"] = m_onlineUsers.contains(originalName);
        finalArr.append(f);
    }

    QJsonObject pkg;
    pkg["type"] = "friend_list";
    pkg["friends"] = finalArr;
    sendJson(socket, pkg);
}

void ChatServer::onClientDisconnected() {
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (socket && m_socketToUser.contains(socket)) {
        QString user = m_socketToUser[socket];
        m_onlineUsers.remove(user);
        m_socketToUser.remove(socket);
        qDebug() << "用户下线:" << user;
    }
    socket->deleteLater();
}

void ChatServer::sendJson(QTcpSocket *socket, const QJsonObject &obj) {
    QJsonDocument doc(obj);
    socket->write(doc.toJson());
    socket->flush();
}
