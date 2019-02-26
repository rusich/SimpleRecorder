#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>
#include "simplerecorder.h"
#ifdef Q_OS_ANDROID
#include <QStringList>
#include <QString>
#include <QtAndroid>
#endif //Q_OS_ANDROID


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setOrganizationName("rusich");
    QGuiApplication::setApplicationName("Simple Recorder");

#ifdef  Q_OS_ANDROID
    QStringList permissions;
    permissions <<"android.permission.WRITE_EXTERNAL_STORAGE";
    permissions <<"android.permission.RECORD_AUDIO";
    QtAndroid::requestPermissionsSync(permissions);
    SimpleRecorder sr("/storage/emulated/0/SimpleRecorder");
#ifdef DEBUG
    QtAndroid::PermissionResult pr = QtAndroid::checkPermission(QString("android.permission.WRITE_EXTERNAL_STORAGE"));
    switch(pr) {
    case QtAndroid::PermissionResult::Granted:
        qDebug()<<"Permission granted";
        break;
    default:
        qDebug()<<"Permission denied!!!";
        break;
    }
#endif //Debug
#else // Q_OS_ANDROID
    SimpleRecorder sr("/tmp/SimpleRecorder");
#endif // Q_OS_ANDROID
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("recorder",&sr);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
