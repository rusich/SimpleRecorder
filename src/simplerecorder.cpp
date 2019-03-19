#include "simplerecorder.h"
#include <QtDebug>
#include <QDir>
#include <QString>
#include <QStandardPaths>

SimpleRecorder::SimpleRecorder(QString PathToSaveRecords, QObject *parent) :
    QObject(parent), _savePath(PathToSaveRecords), _recording(false)
{
    fileRotateTimer.setSingleShot(false);
    connect(&fileRotateTimer, SIGNAL(timeout()), this, SLOT(rotateRecordFile()));
    durationStringTimer.setSingleShot(false);
    durationStringTimer.setInterval(10);
    connect(&durationStringTimer, SIGNAL(timeout()), this, SLOT(updateDurationString()));


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
    _filePath = _savePath + "/" + _recordName.replace(" ","_") +
            QDateTime::currentDateTime().toString("_dd-MM-yyyy_HHmmss");
    audioRecorder = new QAudioRecorder();
//    audioSettings.setEncodingMode(QMultimedia::TwoPassEncoding);
    audioSettings.setQuality(QMultimedia::VeryHighQuality);
    QString container;
#ifndef Q_OS_ANDROID
    audioSettings.setCodec("audio/x-vorbis");
    _filePath += ".ogg";
    container = "audio/ogg";
#endif // Q_OSANDROID
    audioRecorder->setEncodingSettings(audioSettings,QVideoEncoderSettings(),container);
    audioRecorder->setOutputLocation(QUrl::fromLocalFile(_filePath));
    connect(audioRecorder, SIGNAL(mutedChanged(bool)), this, SLOT(inputMuted(bool)));

    emit filePathChanged();
    audioRecorder->record();
    _recordStartTime.restart();
    _recording = true;
    _recordStartTime = QTime::currentTime();
    fileRotateTimer.setInterval(_recordLenght*1000*60);
    fileRotateTimer.start();
    emit recordingChanged();
    durationStringTimer.start();
}

void SimpleRecorder::stopRecord()
{
    audioRecorder->stop();
    auto outputFilePath = audioRecorder->outputLocation().path();
    auto outputFileName = audioRecorder->outputLocation().fileName();

    delete audioRecorder;
    _recording = false;
    emit recordingChanged();
    durationStringTimer.stop();
    emit durationStringUpdated();
    // move recorded file to $RECORDING_PATH/to_send/
    QFile f(outputFilePath);
    f.rename(_savePath+"/to_send/"+outputFileName); //TODO: do something if false
    f.close();
}

void SimpleRecorder::stopRecordRotation()
{
   fileRotateTimer.stop();
   stopRecord();
}

QString SimpleRecorder::getDurationgString()
{
    if(!_recording)
        return QString("00:00.00");
    return _durationString;
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

void SimpleRecorder::updateDurationString()
{
    int ts = _recordStartTime.elapsed();
    short msec = ts%1000/10;
    short sec = ts/1000%60;
    long min = ts/1000/60;
    _durationString = QString("%1:%2.%3")
            .arg(min,2,10,QLatin1Char('0'))
            .arg(sec,2,10,QLatin1Char('0'))
            .arg(msec,2,10,QLatin1Char('0'));

    emit durationStringUpdated();
}

void SimpleRecorder::inputMuted(bool muted)
{
   qDebug()<<"Muted: "<<muted;
   emit this->muted(muted);
}

