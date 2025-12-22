#include "NetworkManager.h"
#include <QJsonDocument>
#include <QDebug>

NetworkManager::NetworkManager(QObject *parent) : QObject(parent) {
    m_socket = new QTcpSocket(this);
    m_storage = new MessageStorage(this);

    // 【初始化音频模块】
    m_audio = new AudioManager(this);
    // 连接信号：音频模块录到数据了 -> NetworkManager 负责发出去
    connect(m_audio, &AudioManager::audioDataReady, this, &NetworkManager::onAudioReady);

    // TCP 信号连接
    connect(m_socket, &QTcpSocket::readyRead, this, &NetworkManager::onReadyRead);
    connect(m_socket, &QTcpSocket::connected, this, &NetworkManager::onConnected);
    connect(m_socket, &QTcpSocket::errorOccurred, this, &NetworkManager::onError);
}

// --- 语音核心逻辑 ---

void NetworkManager::startVoice(QString targetUser) {
    // 【新增日志】检查目标用户是否传进来了
    qDebug() << "👉 [NetworkManager] 按下按钮，目标用户:" << targetUser;

    m_currentVoiceTarget = targetUser;

    if (targetUser.isEmpty()) {
        qDebug() << "❌ [错误] 目标用户为空！语音将无法发送。请先点开一个好友的聊天框。";
    }

    m_audio->startRecording();
}

void NetworkManager::onAudioReady(QByteArray data) {
    // 【新增日志】检查是否因为没有目标而被拦截
    if (m_currentVoiceTarget.isEmpty()) {
        qDebug() << "⛔ [NetworkManager] 拦截：没有发送目标，丢弃语音数据";
        return;
    }

    QString base64Str = QString::fromLatin1(data.toBase64());

    // 【新增日志】准备发送
    qDebug() << "🚀 [NetworkManager] 正在发送语音包 ->" << m_currentVoiceTarget << "大小:" << base64Str.size();

    QJsonObject obj;
    obj["type"] = "voice";
    obj["to"] = m_currentVoiceTarget;
    obj["content"] = base64Str;

    sendJson(obj);
}

void NetworkManager::stopVoice() {
    m_audio->stopRecording(); // 指挥 AudioManager 停工
    m_currentVoiceTarget = "";
}


// --- 数据处理逻辑 ---

void NetworkManager::handleData(const QByteArray &data) {
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) return;
    QJsonObject obj = doc.object();
    QString type = obj.value("type").toString();

    // 1. 普通文字消息
    if (type == "msg") {
        QString fromUser = obj.value("from").toString();
        QString content = obj.value("content").toString();

        QJsonObject record; record["sender"] = "other"; record["text"] = content;
        m_storage->addMessage(fromUser, record);

        // 1. 设置未读
        m_storage->setUnread(fromUser, true);

        // 2. 【核心】必须调用这个函数，红点才会出来！
        refreshFriendList();

        emit messageReceived(fromUser, content, false);
    }
    // 2. 【新增】语音消息处理
    else if (type == "voice") {
        QString content = obj.value("content").toString();

        // Base64 解码
        QByteArray audioData = QByteArray::fromBase64(content.toLatin1());

        // 指挥 AudioManager 播放
        m_audio->playAudioChunk(audioData);
    }
    // 3. 好友列表更新
    else if (type == "friend_list") {
        m_rawFriendList = obj.value("friends").toArray();
        refreshFriendList();
    }
    // 4. 登录结果
    else if (type == "login_resp") {
        if (obj.value("success").toBool()) {
            m_myUserId = obj.value("user_id").toInt();
            m_isLoggedIn = true;
            emit isLoggedInChanged();
            emit myUserIdChanged();
        } else emit registerResult(false, "登录失败");
    }
    // 5. 其他响应
    else if (type == "register_resp") emit registerResult(obj.value("success").toBool(), obj.value("message").toString());
    else if (type == "search_user_resp") emit searchResult(obj.value("found").toBool(), obj.value("found").toBool()?obj.value("user").toObject().value("username").toString():"", -1);
    else if (type == "op_resp") emit operationResult(obj.value("op").toString(), obj.value("success").toBool(), obj.value("message").toString());
}

// --- 标准网络函数 ---

void NetworkManager::connectToServer(QString ip) {
    if (m_socket->state() == QAbstractSocket::ConnectedState && m_socket->peerAddress().toString() == ip) return;
    m_socket->abort(); m_socket->connectToHost(ip, 9999);
}

void NetworkManager::sendJson(const QJsonObject &json) {
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        QJsonDocument doc(json); m_socket->write(doc.toJson()); m_socket->flush();
    } else m_pendingData = json;
}

void NetworkManager::onConnected() {
    if (!m_pendingData.isEmpty()) { sendJson(m_pendingData); m_pendingData = QJsonObject(); }
}

void NetworkManager::onError(QAbstractSocket::SocketError) {
    emit operationResult("network", false, m_socket->errorString());
}

// --- 业务辅助函数 ---

// NetworkManager.cpp

void NetworkManager::refreshFriendList() {
    QJsonArray finalArray;

    // 遍历原始的好友列表缓存
    for (const auto &item : m_rawFriendList) {
        QJsonObject friendObj = item.toObject();
        QString username = friendObj.value("username").toString();

        // 【核心】去 Storage 查一下有没有未读，然后塞到 JSON 里
        bool hasUnread = m_storage->hasUnread(username);
        friendObj["unread"] = hasUnread; // <--- 这行代码决定了红点亮不亮

        finalArray.append(friendObj);
    }

    m_friendList = finalArray;
    emit friendListChanged(); // 通知 QML 刷新
}

void NetworkManager::sendMessage(QString toUser, QString content) {
    QJsonObject obj; obj["type"] = "msg"; obj["to"] = toUser; obj["content"] = content; sendJson(obj);
    QJsonObject record; record["sender"] = "me"; record["text"] = content; m_storage->addMessage(toUser, record);
}

void NetworkManager::clearUnread(QString friendName) {
    m_storage->setUnread(friendName, false); refreshFriendList();
}

QJsonArray NetworkManager::getChatHistory(QString friendName) {
    return m_storage->getHistory(friendName);
}

void NetworkManager::registerUser(QString u, QString p) { QJsonObject o; o["type"]="register"; o["username"]=u; o["password"]=p; sendJson(o); }
void NetworkManager::login(QString u, QString p) { QJsonObject o; o["type"]="login"; o["username"]=u; o["password"]=p; sendJson(o); }
void NetworkManager::searchUser(QString k) { QJsonObject o; o["type"]="search_user"; o["keyword"]=k; sendJson(o); }
void NetworkManager::addFriend(QString f) { QJsonObject o; o["type"]="add_friend"; o["user_id"]=m_myUserId; o["friend_name"]=f; sendJson(o); }
void NetworkManager::deleteFriend(QString f) { QJsonObject o; o["type"]="del_friend"; o["user_id"]=m_myUserId; o["friend_name"]=f; sendJson(o); }
void NetworkManager::updateRemark(QString f, QString r) { QJsonObject o; o["type"]="update_friend"; o["user_id"]=m_myUserId; o["friend_name"]=f; o["remark"]=r; sendJson(o); }

void NetworkManager::onReadyRead() {
    static QByteArray buffer; buffer.append(m_socket->readAll());
    while (!buffer.isEmpty()) {
        int open=0, close=0, end=-1;
        for(int i=0; i<buffer.size(); ++i) { if(buffer[i]=='{') open++; if(buffer[i]=='}') close++; if(open>0 && open==close) { end=i; break; } }
        if(end==-1) break; handleData(buffer.left(end+1)); buffer = buffer.mid(end+1).trimmed();
    }
}
