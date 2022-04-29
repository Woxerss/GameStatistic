#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "WindowTracker.h"
#include "statistic/StatisticCollector.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    // Подключаем C++ к QML
    qmlRegisterType<WindowTracker>("WindowTracker", 1, 0, "WindowTracker");
    qmlRegisterType<StatisticCollector>("StatisticCollector", 1, 0, "StatisticCollector");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
