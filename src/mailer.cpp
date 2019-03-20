#include "mailer.h"
#include <QDebug>
#include <QThread>
#include <QDir>
#include <SmtpMime>


Mailer::Mailer(QString recordsPath, QObject *parent) :
    QObject(parent),
    _recordsPath(recordsPath)
{
    _server = settings.value("mailer/Server","smtp.mail.ru").toString();
    _port = settings.value("mailer/Port",465).toInt();
    _senderName = settings.value("mailer/SenderName","SR Mailer").toString();
    _recipientEmail = settings.value("mailer/RecipientEmail","recipient@example.com").toString();
    _login = settings.value("mailer/Login","simprecmailer@mail.ru").toString();
    _password = settings.value("mailer/Password","123qwe234wer").toString();

}

Mailer::~Mailer()
{
    settings.setValue("mailer/Server", _server);
    settings.setValue("mailer/Port", _port);
    settings.setValue("mailer/SenderName", _senderName);
    settings.setValue("mailer/Login", _login);
    settings.setValue("mailer/RecipientEmail", _recipientEmail);
    settings.setValue("mailer/Password", _password);
}

void  Mailer::start()
{
    ulong timeout = 60;
    while(true){
        if(_recipientEmail=="recipient@example.com") {
            qDebug()<<"Recipient not set. Waiting...";
            QThread::sleep(timeout);
            continue;
        }

        QDir dir(_recordsPath+"/to_send");
        dir.setFilter(QDir::Files);
        if(dir.count()==0) {
            qDebug()<<"dir empty";
            QThread::sleep(timeout);
            continue;
        }

        QString file;
        foreach(file, dir.entryList()) {
            qDebug()<<"Sending File: "<<file;

            SmtpClient smtp(_server, 465, SmtpClient::SslConnection);
            smtp.setUser(_login);
            smtp.setPassword(_password);

            MimeMessage message;
            message.setSender(new EmailAddress(_login, _senderName));
            message.addRecipient(new EmailAddress(_recipientEmail));
            message.setSubject(file);
            QFile *fileToSend = new QFile(dir.path()+"/"+ file);
            MimeAttachment attachment(fileToSend);

            message.addPart(&attachment);
            smtp.connectToHost();
            smtp.login();
            bool ok = smtp.sendMail(message);
            smtp.quit();
            if(ok) {
                qDebug()<<"SEND: OK!";
                fileToSend->rename(_recordsPath+"/sent/"+file);
            }
            else {
                qDebug()<<"SEND: ERROR!";
            }
        }
    }
}
