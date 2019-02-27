#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>
#include "simplerecorder.h"
#ifdef Q_OS_ANDROID
#include <QStringList>
#include <QString>
#include <QtAndroid>
#include <QAndroidJniObject>
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
    permissions <<"android.permission.WAKE_LOCK";
    QtAndroid::requestPermissionsSync(permissions);
#ifdef DEBUG
    QtAndroid::PermissionResult pr = QtAndroid::checkPermission(QString("android.permission.WAKE_LOCK"));
    switch(pr) {
    case QtAndroid::PermissionResult::Granted:
        qDebug()<<"Permission granted";
        break;
    default:
        qDebug()<<"Permission denied!!!";
        break;
    }
#endif //Debug
    QAndroidJniObject activity = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative", "activity", "()Landroid/app/Activity;");    //activity is valid
    if ( activity.isValid() )
    {
        QAndroidJniObject wakeLockObject("my/utl/WakeLock","(Landroid/app/Activity;)V",activity.object<jobject>());
        if ( wakeLockObject.isValid() )
        {
            jint res = wakeLockObject.callMethod<jint>("configure","()I");
            assert( res == 42 ); // check Java code was executed...
        }
        else
        {
            assert( false );
        }
    }
    else
    {
        assert( false );
    }
   //
    SimpleRecorder sr("/storage/emulated/0/SimpleRecorder");
#else // Q_OS_ANDROID
    SimpleRecorder sr("/tmp/SimpleRecorder");
#endif // Q_OS_ANDROID
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("recorder",&sr);
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
