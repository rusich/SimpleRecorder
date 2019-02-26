#include "simplerecorder.h"
#include <QtDebug>
#include <QDir>
#include <QString>
#include <QStandardPaths>

SimpleRecorder::SimpleRecorder(QString PathToSaveRecords, QObject *parent) : QObject(parent),
    savePath(PathToSaveRecords), _recording(false)
{
    fileRotateTimer.setSingleShot(false);
    connect(&fileRotateTimer, SIGNAL(timeout()), this, SLOT(rotateRecordFile()));
    if(!QDir(savePath).exists())  {
        if(!QDir().mkdir(savePath)) qDebug("Cannot create directory to save records!");
    }

    _recordLenght = settings.value("recorder/RecordLenght", 5).toInt();
    _recordName = settings.value("recorder/RecordName", "Default").toString();
}

QString SimpleRecorder::getFileName()
{
    QStringList pieces = _filePath.split( "/" );
    return pieces.value( pieces.length() - 1 );
}

void SimpleRecorder::startRecord()
{
    if(_recording)
        return;

    //    qDebug()<<audioRecorder->supportedAudioCodecs();
    //    qDebug()<<audioRecorder->supportedContainers();
    _filePath = savePath + "/" + _recordName.replace(" ","_") + QDateTime::currentDateTime().toString("_dd-MM-yyyy_HH:mm:ss");
    audioRecorder = new QAudioRecorder();
    audioSettings.setEncodingMode(QMultimedia::TwoPassEncoding);
    audioSettings.setQuality(QMultimedia::HighQuality);
    QString container;
#ifndef ANDROID
    audioSettings.setCodec("audio/x-vorbis");
    _filePath += ".ogg";
    container = "audio/ogg";
#endif // ANDROID
    audioRecorder->setEncodingSettings(audioSettings,QVideoEncoderSettings(),container);
    audioRecorder->setOutputLocation(QUrl::fromLocalFile(_filePath));
    emit filePathChanged();
    audioRecorder->record();
    qDebug()<<_filePath;
    _recording = true;
    fileRotateTimer.setInterval(_recordLenght*1000);
    fileRotateTimer.start();

    emit recordingChanged();
}

void SimpleRecorder::stopRecord()
{
    audioRecorder->stop();
    delete audioRecorder;
    _recording = false;
    emit recordingChanged();
}

void SimpleRecorder::stopRecordRotation()
{
   fileRotateTimer.stop();
   stopRecord();
}

SimpleRecorder::~SimpleRecorder()
{
    settings.setValue("recorder/RecordLenght", _recordLenght);
    settings.setValue("recorder/RecordName", _recordName);
}


void SimpleRecorder::rotateRecordFile()
{
    stopRecord();
    startRecord();
}

