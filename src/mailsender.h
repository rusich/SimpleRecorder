#ifndef MAILSENDER_H
#define MAILSENDER_H

#include <QObject>

class MailSender : public QObject
{
    Q_OBJECT
public:
    explicit MailSender(QString sendingFilesPath, QObject *parent = nullptr);

signals:

public slots:
    void start();
private:
    QString _sendingFilesPath;
};

#endif // MAILSENDER_H
