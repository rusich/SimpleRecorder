#include "simplerecorder.h"
#include <QtDebug>
#include <QDir>
#include <QString>
#include <QStandardPaths>

SimpleRecorder::SimpleRecorder(QString PathToSaveRecords, QObject *parent) : QObject(parent), savePath(PathToSaveRecords)
{
    _recording = false;
    if(!QDir(savePath).exists())  {
        if(!QDir().mkdir(savePath)) qDebug("Cannot create directory to save records!");
    }

    _recordLenght = settings.value("recorder/RecordLenght", 5).toInt();
    _recordName = settings.value("recorder/RecordName", "Default").toString();
    qDebug()<<audioRecorder.supportedAudioCodecs();
    qDebug()<<audioRecorder.supportedContainers();
    //    audioSettings.setCodec("audio/x-vorbis");
    audioSettings.setEncodingMode(QMultimedia::TwoPassEncoding);
    qDebug()<<audioSettings.codec();
    audioSettings.setQuality(QMultimedia::HighQuality);


    //    audioRecorder.setEncodingSettings(audioSettings, QVideoEncoderSettings(),"audio/ogg");

}

void SimpleRecorder::startRecord()
{
    if(audioRecorder.state()!= QMediaRecorder::RecordingState)
    {
        QString fileName = savePath + "/" + _recordName + QDateTime::currentDateTime().toString("_dd-MM-yyyy_HH:mm:ss") ;
        qDebug()<<fileName;
        audioRecorder.setOutputLocation(QUrl(fileName));
        audioRecorder.record();
        _recording = true;
        emit recordingChanged();
    }
}

void SimpleRecorder::stopRecord()
{
    if(audioRecorder.state()!= QMediaRecorder::StoppedState)
    {
        audioRecorder.stop();
        audioRecorder.setOutputLocation(QUrl(""));
        _recording = false;
        emit recordingChanged();
    }
}

SimpleRecorder::~SimpleRecorder()
{
    settings.setValue("recorder/RecordLenght", _recordLenght);
    settings.setValue("recorder/RecordName", _recordName);
}

void SimpleRecorder::printAudioRecorderError()
{

}

