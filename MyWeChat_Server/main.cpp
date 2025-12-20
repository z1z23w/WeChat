#include <QCoreApplication>
#include "ChatServer.h"
#include "DBManager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // 【第一步】连接数据库
    if (!DBManager::instance().connectToDb()) {
        qDebug() << "致命错误：数据库无法启动，程序退出。";
        return -1;
    }

    // 【第二步】启动服务器监听
    ChatServer server;
    if (!server.start(9999)) {
        return -1;
    }

    return a.exec();
}
