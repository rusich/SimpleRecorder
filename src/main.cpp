#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>
#include <QThread>
#include "simplerecorder.h"
#include "mailsender.h"
#ifdef Q_OS_ANDROID
#include <QStringList>
#include <QString>
#include <QtAndroid>
#include <QAndroidJniObject>
#endif //Q_OS_ANDROID

void checkDirs(QString PathToSaveRecords) {
    if(!QDir(PathToSaveRecords).exists())  {
        if(!QDir().mkdir(PathToSaveRecords)) qDebug("Cannot create directory to save records!");
    }
    QString to_send = PathToSaveRecords + QString("/to_send");
    if(!QDir(to_send).exists())  {
        if(!QDir().mkdir(to_send)) qDebug("Cannot create directory 'to_send'!");
    }
    QString sent = PathToSaveRecords + QString("/sent");
    if(!QDir(sent).exists())  {
        if(!QDir().mkdir(sent)) qDebug("Cannot create directory 'send'!");
    }
}

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

    checkDirs(sr.getSavePath());
    QThread *thread = new QThread(&sr);
    MailSender ms(sr.getSavePath()+"/"+"to_send");
    ms.moveToThread(thread);
    QThread::connect(thread,SIGNAL(started()),&ms,SLOT(start()));
    thread->start();

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("recorder",&sr);
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
