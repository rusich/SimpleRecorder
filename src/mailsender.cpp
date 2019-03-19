#include "mailsender.h"
#include <QDebug>
#include <QThread>
#include <QDir>


MailSender::MailSender(QString sendingFilesPath, QObject *parent) :
    QObject(parent),
    _sendingFilesPath(sendingFilesPath)
{

}

void MailSender::start()
{
    while(true){
        QDir dir(_sendingFilesPath);
        dir.setFilter(QDir::Files);
        if(dir.count()==0) {
            qDebug()<<"dir empty";
            QThread::sleep(5);
            continue;
        }

        QString file;
        foreach(file, dir.entryList()) {
            qDebug()<<"File: "<<file;
        }
        QThread::sleep(5);
    }
}
