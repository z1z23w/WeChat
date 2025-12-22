#include "MessageStorage.h"

MessageStorage::MessageStorage(QObject *parent) : QObject(parent) {}

void MessageStorage::addMessage(const QString &friendName, const QJsonObject &msgObj) {
    // 1. 取出旧记录
    QJsonArray chatLog = m_history.value(friendName);
    // 2. 追加新消息
    chatLog.append(msgObj);
    // 3. 存回去
    m_history.insert(friendName, chatLog);
}

QJsonArray MessageStorage::getHistory(const QString &friendName) {
    return m_history.value(friendName);
}

void MessageStorage::setUnread(const QString &friendName, bool hasUnread) {
    m_unreadMap.insert(friendName, hasUnread);
}

bool MessageStorage::hasUnread(const QString &friendName) {
    return m_unreadMap.value(friendName, false);
}
