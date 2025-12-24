#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "NetworkManager.h" // 引用新头文件

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<NetworkManager>("we", 1, 0, "NetworkManager");

    QQmlApplicationEngine engine;
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
                     &app, []() { QCoreApplication::exit(-1); },
                     Qt::QueuedConnection);
    engine.loadFromModule("we", "Main");

    return app.exec();
}
