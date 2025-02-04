#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "InputHandler.h"
#include <QQmlContext>
#include <QDebug>
#include <cstdlib>
#include <ctime>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
                         if (!obj && url == objUrl)
                             QCoreApplication::exit(-1);
                     }, Qt::QueuedConnection);

    InputHandler inputHandler;
    engine.rootContext()->setContextProperty("inputHandler", &inputHandler);
    engine.load(url);

    inputHandler.gameStarted();
    return app.exec();
}
