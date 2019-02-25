#include "simplerecorder.h"
#include <QtDebug>
#include <QDir>
#include <QString>
#include <QStandardPaths>

SimpleRecorder::SimpleRecorder(QString PathToSaveRecords, QObject *parent) : QObject(parent), savePath(PathToSaveRecords)
{
    if(!QDir(savePath).exists())  {
        if(!QDir().mkdir(savePath)) qDebug("Cannot create directory to save records!");
    }

    _recordLenght = settings.value("recorder/RecordLenght", 5).toInt();
    qDebug()<<QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
}

SimpleRecorder::~SimpleRecorder()
{
    settings.setValue("recorder/RecordLenght", _recordLenght);
}

