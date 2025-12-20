#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle> // 新增头文件

int main(int argc, char *argv[])
{

    QQuickStyle::setStyle("Material");

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.loadFromModule("we", "Main");

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
                     &app, []() { QCoreApplication::exit(-1); },
                     Qt::QueuedConnection);

    return app.exec();
}
