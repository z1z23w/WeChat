#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QString>
#include <QSqlDatabase>
#include <QJsonArray>
#include <QJsonObject>

class DBManager
{
public:
    static DBManager& instance();
    bool connectToDb();

    // --- 用户账户相关 ---
    bool isUserExists(const QString &username);
    bool registerUser(const QString &username, const QString &password);
    bool loginUser(const QString &username, const QString &password, int &userId);
    int getUserId(const QString &username);
    QString getUsername(int userId);

    // --- 好友管理相关 (增删改查) ---
    // 查：获取好友列表 (包含备注)
    QJsonArray getFriendList(int userId);
    // 增：添加好友
    bool addFriend(int userId, const QString &friendName);
    // 删：删除好友
    bool deleteFriend(int userId, const QString &friendName);
    // 改：修改好友备注
    bool updateFriendRemark(int userId, const QString &friendName, const QString &remark);
    // 搜：搜索用户 (用于添加好友前检查)
    bool searchUser(const QString &keyword, QJsonObject &result);

    // 更新状态
    void updateUserStatus(int userId, bool isOnline, QString ip, int port);

private:
    DBManager();
    QSqlDatabase m_db;
};

#endif // DBMANAGER_H
