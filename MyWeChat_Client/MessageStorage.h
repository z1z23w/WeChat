#ifndef MESSAGESTORAGE_H
#define MESSAGESTORAGE_H

#include <QObject>
#include <QMap>
#include <QJsonArray>
#include <QJsonObject>

class MessageStorage : public QObject
{
    Q_OBJECT
public:
    explicit MessageStorage(QObject *parent = nullptr);

    // 存入一条新消息
    // friendName: 对方的名字
    void addMessage(const QString &friendName, const QJsonObject &msgObj);

    // 获取和某人的所有历史记录
    QJsonArray getHistory(const QString &friendName);

    // 设置/查询/清除
    void setUnread(const QString &friendName, bool hasUnread);
    bool hasUnread(const QString &friendName);

private:
    // 内存数据库：Key=好友用户名, Value=消息列表
    QMap<QString, QJsonArray> m_history;

    // 未读状态表：Key=好友用户名, Value=是否未读
    QMap<QString, bool> m_unreadMap;
};

#endif // MESSAGESTORAGE_H
