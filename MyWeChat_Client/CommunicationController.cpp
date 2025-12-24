#include "CommunicationController.h"
#include <QJsonDocument>

CommunicationController::CommunicationController(QObject *parent) : QObject(parent) {
    m_socket = new QTcpSocket(this);
    m_currentUser = new Netizen(this);
    m_chatSession = new ChatSession(this);
    m_complianceCtrl = new ComplianceController(this);
    m_mediaCtrl = new MediaSessionController(this);

    // 绑定子控制器信号
    connect(m_mediaCtrl, &MediaSessionController::mediaStreamGenerated, this, &CommunicationController::onMediaData);

    // 绑定 Socket 信号
    connect(m_socket, &QTcpSocket::readyRead, this, &CommunicationController::onReadyRead);

    // 【修改】连接到我们自定义的 onConnected 槽函数
    connect(m_socket, &QTcpSocket::connected, this, &CommunicationController::onConnected);
}

bool CommunicationController::isConnected() const {
    return m_socket->state() == QTcpSocket::ConnectedState;
}

// 【新增】当 TCP 连接真正建立时
void CommunicationController::onConnected() {
    // 通知 UI 状态变化
    emit connectionStatusChanged();

    // 检查是否有还没发出去的数据（比如刚才点的登录请求）
    if (!m_pendingData.isEmpty()) {
        sendJson(m_pendingData);
        m_pendingData = QJsonObject(); // 清空缓存
    }
}

void CommunicationController::connectToServer(QString ip) {
    // 如果已经在连接或已连接，就不重复动作，但允许重连
    if (m_socket->state() == QTcpSocket::ConnectedState && m_socket->peerAddress().toString() == ip) {
        return;
    }
    m_socket->abort();
    m_socket->connectToHost(ip, 9999);
}

void CommunicationController::login(QString u, QString p) {
    QJsonObject o; o["type"]="login"; o["username"]=u; o["password"]=p;
    sendJson(o);
}

void CommunicationController::registerUser(QString u, QString p) {
    QJsonObject o; o["type"]="register"; o["username"]=u; o["password"]=p;
    sendJson(o);
}

void CommunicationController::sendMessage(QString content) {
    if (m_chatSession->currentTarget().isEmpty()) {
        emit notificationTriggered("错误", "未选择好友");
        return;
    }

    QString safeText = m_complianceCtrl->checkAndFilter(content);

    QJsonObject o; o["type"]="msg"; o["to"]=m_chatSession->currentTarget(); o["content"]=safeText;
    sendJson(o);
}

void CommunicationController::startMediaSession() {
    if (m_chatSession->currentTarget().isEmpty()) return;
    m_mediaCtrl->startSession();
}

void CommunicationController::endMediaSession() {
    m_mediaCtrl->endSession();
}

void CommunicationController::onMediaData(QByteArray data) {
    if (m_chatSession->currentTarget().isEmpty()) return;
    QJsonObject o; o["type"]="voice"; o["to"]=m_chatSession->currentTarget();
    o["content"]=QString::fromLatin1(data.toBase64());
    sendJson(o);
}

void CommunicationController::selectFriend(QString name) {
    m_chatSession->setCurrentTarget(name);
}

void CommunicationController::searchUser(QString keyword) {
    QJsonObject o; o["type"]="search_user"; o["keyword"]=keyword;
    sendJson(o);
}

void CommunicationController::addFriend(QString friendName) {
    QJsonObject o; o["type"]="add_friend"; o["friend_name"]=friendName;
    sendJson(o);
}

// 【修改】核心发送逻辑
void CommunicationController::sendJson(const QJsonObject &json) {
    if (isConnected()) {
        // 如果连接正常，直接发送
        m_socket->write(QJsonDocument(json).toJson());
        m_socket->flush();
    } else {
        // 如果没连接，先存起来
        m_pendingData = json;

        // 如果当前完全没动静（不是正在连接中），提示一下用户
        if (m_socket->state() == QTcpSocket::UnconnectedState) {
            emit notificationTriggered("提示", "正在连接服务器，请稍候...");
        }
        // 注意：这里我们不报错，因为 onConnected 会自动补发
    }
}

void CommunicationController::onReadyRead() {
    QByteArray data = m_socket->readAll();
    handleData(data);
}

void CommunicationController::handleData(const QByteArray &data) {
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) return;
    QJsonObject obj = doc.object();
    QString type = obj.value("type").toString();

    if (type == "login_resp") {
        if (obj.value("success").toBool()) {
            m_currentUser->setUsername(obj.value("username").toString());
            m_currentUser->setStatus("Online");
            emit notificationTriggered("系统", "登录成功");
        } else {
            emit notificationTriggered("系统", "登录失败");
        }
    }
    else if (type == "msg") {
        emit messageReceived(obj.value("from").toString(), obj.value("content").toString());
    }
    else if (type == "voice") {
        QByteArray d = QByteArray::fromBase64(obj.value("content").toString().toLatin1());
        m_mediaCtrl->playIncomingStream(d);
    }
    else if (type == "friend_list") {
        m_friendList = obj.value("friends").toArray();
        emit friendListChanged();
    }
    else if (type == "search_user_resp") {
        if (obj.value("found").toBool())
            emit notificationTriggered("搜索", "找到用户: " + obj.value("user").toObject().value("username").toString());
        else
            emit notificationTriggered("搜索", "用户不存在");
    }
}
