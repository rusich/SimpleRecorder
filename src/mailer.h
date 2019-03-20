#ifndef MAILSENDER_H
#define MAILSENDER_H

#include <QObject>
#include <QSettings>

class Mailer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString server MEMBER _server)
    Q_PROPERTY(int port MEMBER _port)
    Q_PROPERTY(QString senderName MEMBER _senderName)
    Q_PROPERTY(QString recipientEmail MEMBER _recipientEmail)
    Q_PROPERTY(QString login MEMBER _login)
    Q_PROPERTY(QString password MEMBER _password)
public:
    explicit Mailer(QString recordsPath, QObject *parent = nullptr);
    ~Mailer();

signals:

public slots:
     void start();
private:
    QString _recordsPath;
    QSettings settings;
    QString _server;
    int _port;
    QString _senderName;
    QString _recipientEmail;
    QString _login;
    QString _password;
};

#endif // MAILSENDER_H
