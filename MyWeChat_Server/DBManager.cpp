#include "DBManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QCoreApplication>
#include <QDir>

DBManager::DBManager() {}

DBManager& DBManager::instance() {
    static DBManager instance;
    return instance;
}

bool DBManager::connectToDb() {
    m_db = QSqlDatabase::addDatabase("QSQLITE");

    QString dbPath = QDir::cleanPath(QCoreApplication::applicationDirPath() + "/../../../wechat.db");

    m_db.setDatabaseName(dbPath);

    if (!m_db.open()) {
        qDebug() << "❌ 数据库连接失败:" << m_db.lastError().text();
        return false;
    }

    qDebug() << "✅ 数据库连接成功! 文件路径:" << dbPath;

    QSqlQuery query;
    // 1. 用户表
    query.exec(R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT UNIQUE NOT NULL,
            password TEXT NOT NULL,
            is_online INTEGER DEFAULT 0,
            ip TEXT,
            port INTEGER
        )
    )");

    // 2. 好友关系表 (本次新增)
    query.exec(R"(
        CREATE TABLE IF NOT EXISTS friends (
            user_id INTEGER,
            friend_id INTEGER,
            remark TEXT,
            create_time DATETIME DEFAULT CURRENT_TIMESTAMP,
            PRIMARY KEY (user_id, friend_id)
        )
    )");

    return true;
}

bool DBManager::isUserExists(const QString &username) {
    return getUserId(username) != -1;
}

bool DBManager::registerUser(const QString &username, const QString &password) {
    if (isUserExists(username)) return false;
    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password) VALUES (:u, :p)");
    query.bindValue(":u", username);
    query.bindValue(":p", password);
    return query.exec();
}

bool DBManager::loginUser(const QString &username, const QString &password, int &userId) {
    QSqlQuery query;
    query.prepare("SELECT id FROM users WHERE username = :u AND password = :p");
    query.bindValue(":u", username);
    query.bindValue(":p", password);

    if (query.exec() && query.next()) {
        userId = query.value(0).toInt();
        return true;
    }
    return false;
}

int DBManager::getUserId(const QString &username) {
    QSqlQuery query;
    query.prepare("SELECT id FROM users WHERE username = :u");
    query.bindValue(":u", username);
    if (query.exec() && query.next()) return query.value(0).toInt();
    return -1;
}

QString DBManager::getUsername(int userId) {
    QSqlQuery query;
    query.prepare("SELECT username FROM users WHERE id = :id");
    query.bindValue(":id", userId);
    if (query.exec() && query.next()) return query.value(0).toString();
    return "";
}

void DBManager::updateUserStatus(int userId, bool isOnline, QString ip, int port) {
    QSqlQuery query;
    query.prepare("UPDATE users SET is_online = :o, ip = :ip, port = :p WHERE id = :id");
    query.bindValue(":o", isOnline ? 1 : 0);
    query.bindValue(":ip", ip);
    query.bindValue(":p", port);
    query.bindValue(":id", userId);
    query.exec();
}


QJsonArray DBManager::getFriendList(int userId) {
    QJsonArray list;
    QSqlQuery query;
    query.prepare("SELECT u.username, f.remark FROM users u "
                  "INNER JOIN friends f ON u.id = f.friend_id "
                  "WHERE f.user_id = :uid");
    query.bindValue(":uid", userId);

    if (query.exec()) {
        while (query.next()) {
            QJsonObject friendObj;
            QString originalName = query.value(0).toString();
            QString remark = query.value(1).toString();

            friendObj["username"] = originalName; // 原始账号名
            friendObj["displayName"] = remark.isEmpty() ? originalName : remark; // 有备注显示备注，没备注显示原名
            friendObj["remark"] = remark;
            list.append(friendObj);
        }
    }
    return list;
}

bool DBManager::addFriend(int userId, const QString &friendName) {
    int friendId = getUserId(friendName);
    // 1. 基本检查：用户不存在，或者试图加自己
    if (friendId == -1 || friendId == userId) return false;

    QSqlQuery query;
    m_db.transaction(); // 开启事务

    // 2. 插入 A -> B
    query.prepare("INSERT OR IGNORE INTO friends (user_id, friend_id) VALUES (:u, :f)");
    query.bindValue(":u", userId);
    query.bindValue(":f", friendId);
    bool ok1 = query.exec();

    // 3. 插入 B -> A (实现互为好友)
    query.prepare("INSERT OR IGNORE INTO friends (user_id, friend_id) VALUES (:u, :f)");
    query.bindValue(":u", friendId); // 注意这里反过来了
    query.bindValue(":f", userId);
    bool ok2 = query.exec();

    if (ok1 && ok2) {
        m_db.commit();
        return true;
    } else {
        m_db.rollback();
        return false;
    }
}

bool DBManager::deleteFriend(int userId, const QString &friendName) {
    int friendId = getUserId(friendName);
    if (friendId == -1) return false;

    QSqlQuery query;
    query.prepare("DELETE FROM friends WHERE user_id = :u AND friend_id = :f");
    query.bindValue(":u", userId);
    query.bindValue(":f", friendId);
    return query.exec();
}

bool DBManager::updateFriendRemark(int userId, const QString &friendName, const QString &remark) {
    int friendId = getUserId(friendName);
    if (friendId == -1) return false;

    QSqlQuery query;
    query.prepare("UPDATE friends SET remark = :r WHERE user_id = :u AND friend_id = :f");
    query.bindValue(":r", remark);
    query.bindValue(":u", userId);
    query.bindValue(":f", friendId);
    return query.exec();
}

bool DBManager::searchUser(const QString &keyword, QJsonObject &result) {
    int uid = getUserId(keyword);
    if (uid != -1) {
        result["exists"] = true;
        result["username"] = keyword;
        result["id"] = uid;
        return true;
    }
    return false;
}
