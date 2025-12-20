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
    // 1. 设置数据库类型为 SQLite
    m_db = QSqlDatabase::addDatabase("QSQLITE");

    // 2. 设置数据库文件名 (放在运行目录下)
    QString dbPath = QCoreApplication::applicationDirPath() + "/wechat.db";
    m_db.setDatabaseName(dbPath);

    // 3. 打开连接
    if (!m_db.open()) {
        qDebug() << "❌ 数据库连接失败:" << m_db.lastError().text();
        return false;
    }

    qDebug() << "✅ 数据库连接成功! 文件路径:" << dbPath;

    // 4. 自动建表：如果 users 表不存在，就创建它
    QSqlQuery query;
    QString sql = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT UNIQUE NOT NULL,
            password TEXT NOT NULL,
            is_online INTEGER DEFAULT 0,
            ip TEXT,
            port INTEGER
        )
    )";

    if (!query.exec(sql)) {
        qDebug() << "❌ 建表失败:" << query.lastError().text();
        return false;
    } else {
        qDebug() << "✅ 数据表检查完毕 (users表已就绪)";
    }

    return true;
}

bool DBManager::registerUser(const QString &username, const QString &password) {
    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password) VALUES (:u, :p)");
    query.bindValue(":u", username);
    query.bindValue(":p", password);

    if (query.exec()) {
        return true;
    } else {
        qDebug() << "注册失败 (可能是用户名重复):" << query.lastError().text();
        return false;
    }
}

bool DBManager::loginUser(const QString &username, const QString &password, int &userId) {
    QSqlQuery query;
    query.prepare("SELECT id FROM users WHERE username = :u AND password = :p");
    query.bindValue(":u", username);
    query.bindValue(":p", password);

    if (query.exec() && query.next()) {
        userId = query.value(0).toInt(); // 获取查到的 ID
        return true;
    }
    return false;
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
