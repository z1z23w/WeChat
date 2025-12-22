#include "NetworkManager.h"
#include <QJsonDocument>
#include <QDebug>

NetworkManager::NetworkManager(QObject *parent) : QObject(parent) {
    m_socket = new QTcpSocket(this);
    m_storage = new MessageStorage(this); // 初始化数据层

    connect(m_socket, &QTcpSocket::readyRead, this, &NetworkManager::onReadyRead);
    connect(m_socket, &QTcpSocket::connected, this, &NetworkManager::onConnected);
    connect(m_socket, &QTcpSocket::errorOccurred, this, &NetworkManager::onError);
}

void NetworkManager::connectToServer(QString ip) {
    if (m_socket->state() == QAbstractSocket::ConnectedState && m_socket->peerAddress().toString() == ip) return;
    m_socket->abort();
    m_socket->connectToHost(ip, 9999);
}

void NetworkManager::sendJson(const QJsonObject &json) {
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        QJsonDocument doc(json);
        m_socket->write(doc.toJson());
        m_socket->flush();
    } else {
        m_pendingData = json;
    }
}

void NetworkManager::onConnected() {
    qDebug() << "✅ 连接服务器成功";
    if (!m_pendingData.isEmpty()) {
        sendJson(m_pendingData);
        m_pendingData = QJsonObject();
    }
}

void NetworkManager::onError(QAbstractSocket::SocketError socketError) {
    emit operationResult("network", false, "网络错误: " + m_socket->errorString());
}

// --- 核心逻辑：刷新好友列表（合并红点） ---
void NetworkManager::refreshFriendList() {
    QJsonArray finalArray;
    for (const auto &item : m_rawFriendList) {
        QJsonObject friendObj = item.toObject();
        QString username = friendObj.value("username").toString();

        bool hasUnread = m_storage->hasUnread(username);
        friendObj["unread"] = hasUnread;

        finalArray.append(friendObj);
    }
    m_friendList = finalArray;
    emit friendListChanged();
}

void NetworkManager::sendMessage(QString toUser, QString content) {
    QJsonObject obj; obj["type"] = "msg"; obj["to"] = toUser; obj["content"] = content;
    sendJson(obj);

    QJsonObject record; record["sender"] = "me"; record["text"] = content;
    m_storage->addMessage(toUser, record);
}

void NetworkManager::clearUnread(QString friendName) {
    m_storage->setUnread(friendName, false);
    refreshFriendList(); // 强制刷新 UI
}

// --- 业务封装 ---
void NetworkManager::registerUser(QString u, QString p) {
    QJsonObject o; o["type"]="register"; o["username"]=u; o["password"]=p; sendJson(o);
}
void NetworkManager::login(QString u, QString p) {
    QJsonObject o; o["type"]="login"; o["username"]=u; o["password"]=p; sendJson(o);
}
void NetworkManager::searchUser(QString k) {
    QJsonObject o; o["type"]="search_user"; o["keyword"]=k; sendJson(o);
}
void NetworkManager::addFriend(QString f) {
    QJsonObject o; o["type"]="add_friend"; o["user_id"]=m_myUserId; o["friend_name"]=f; sendJson(o);
}
void NetworkManager::deleteFriend(QString f) {
    QJsonObject o; o["type"]="del_friend"; o["user_id"]=m_myUserId; o["friend_name"]=f; sendJson(o);
}
void NetworkManager::updateRemark(QString f, QString r) {
    QJsonObject o; o["type"]="update_friend"; o["user_id"]=m_myUserId; o["friend_name"]=f; o["remark"]=r; sendJson(o);
}

// --- 数据处理 ---
void NetworkManager::onReadyRead() {
    static QByteArray buffer;
    buffer.append(m_socket->readAll());
    while (!buffer.isEmpty()) {
        int open=0, close=0, end=-1;
        for(int i=0; i<buffer.size(); ++i) {
            if(buffer[i]=='{') open++; if(buffer[i]=='}') close++;
            if(open>0 && open==close) { end=i; break; }
        }
        if(end==-1) break;
        handleData(buffer.left(end+1));
        buffer = buffer.mid(end+1).trimmed();
    }
}

void NetworkManager::handleData(const QByteArray &data) {
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) return;
    QJsonObject obj = doc.object();
    QString type = obj.value("type").toString();

    if (type == "msg") {
        QString fromUser = obj.value("from").toString();
        QString content = obj.value("content").toString();

        QJsonObject record; record["sender"] = "other"; record["text"] = content;
        m_storage->addMessage(fromUser, record);
        m_storage->setUnread(fromUser, true);

        refreshFriendList(); // 核心：触发 UI 红点
        emit messageReceived(fromUser, content, false);
    }
    else if (type == "friend_list") {
        m_rawFriendList = obj.value("friends").toArray();
        refreshFriendList();
    }
    else if (type == "login_resp") {
        if (obj.value("success").toBool()) {
            m_myUserId = obj.value("user_id").toInt();
            m_isLoggedIn = true;
            emit isLoggedInChanged();
            emit myUserIdChanged();
        } else {
            emit registerResult(false, "登录失败");
        }
    }
    else if (type == "register_resp") emit registerResult(obj.value("success").toBool(), obj.value("message").toString());
    else if (type == "search_user_resp") emit searchResult(obj.value("found").toBool(), obj.value("found").toBool()?obj.value("user").toObject().value("username").toString():"", -1);
    else if (type == "op_resp") emit operationResult(obj.value("op").toString(), obj.value("success").toBool(), obj.value("message").toString());
}

QJsonArray NetworkManager::getChatHistory(QString friendName) {
    return m_storage->getHistory(friendName);
}
