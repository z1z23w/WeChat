#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QString>
#include <QSqlDatabase>

class DBManager
{
public:
    // 获取单例实例
    static DBManager& instance();

    // 连接数据库
    bool connectToDb();

    // 业务函数：注册和登录
    bool registerUser(const QString &username, const QString &password);
    bool loginUser(const QString &username, const QString &password, int &userId);

    // 更新用户状态 (IP, Port, 在线状态)
    void updateUserStatus(int userId, bool isOnline, QString ip, int port);

private:
    DBManager(); // 私有构造函数
    QSqlDatabase m_db;
};

#endif // DBMANAGER_H
