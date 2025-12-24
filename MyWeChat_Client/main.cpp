#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "CommunicationController.h"

// 【修复点1】引用 StringLiterals 命名空间
using namespace Qt::StringLiterals;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<CommunicationController>("we", 1, 0, "CommunicationController");

    QQmlApplicationEngine engine;

    // 【修复点2】把 _qs 改成 _s
    const QUrl url(u"qrc:/we/NetizenChatUI.qml"_s);

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
                     &app, []() { QCoreApplication::exit(-1); },
                     Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
