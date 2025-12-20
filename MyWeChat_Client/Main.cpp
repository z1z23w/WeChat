#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle> // 必须引入这个

int main(int argc, char *argv[])
{
    // 设置漂亮的 Material 风格
    QQuickStyle::setStyle("Material");

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.loadFromModule("we", "Main");

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
                     &app, []() { QCoreApplication::exit(-1); },
                     Qt::QueuedConnection);

    return app.exec();
}
