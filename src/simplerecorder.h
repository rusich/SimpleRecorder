#ifndef SIMPLERECORDER_H
#define SIMPLERECORDER_H

#include <QObject>
#include <QSettings>
#include <QAudioRecorder>
#include <QtMultimedia>
#include <QTimer>

class SimpleRecorder : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int recordLenght READ getRecordLenght WRITE setRecordLenght)
    Q_PROPERTY(QString recordName READ getRecordName WRITE setRecordName)
    Q_PROPERTY(QString fileName READ getFileName NOTIFY filePathChanged)
    Q_PROPERTY(bool isRecording READ getRecording NOTIFY recordingChanged)
    Q_PROPERTY(int duration READ getDuration)
    Q_PROPERTY(QString savePath READ getSavePath MEMBER _savePath)
    Q_PROPERTY(QString durationString READ getDurationgString NOTIFY durationStringUpdated)
public:
    explicit SimpleRecorder(QString PathToSaveRecords , QObject *parent = nullptr);
    inline int getRecordLenght() { return _recordLenght; }
    inline void setRecordLenght(int lenghtInMinutes) { _recordLenght = lenghtInMinutes; }
    inline QString getRecordName() { return _recordName; }
    inline QString getSavePath() { return _savePath; }
    inline void setRecordName(QString newRecordName) { _recordName = newRecordName; }
    QString getFileName();
    inline bool getRecording() { return  _recording; }
    Q_INVOKABLE void startRecord();
    Q_INVOKABLE void stopRecord();
    Q_INVOKABLE void stopRecordRotation();
    inline int getDuration() { return _recordStartTime.elapsed(); }
    QString getDurationgString();
    Q_INVOKABLE void saveSettings();

    
signals:
    void recordingChanged();
    void filePathChanged();
    void durationStringUpdated();
    void muted(bool muted);
private slots:
    void rotateRecordFile();
    void updateDurationString();
    void inputMuted(bool muted);
private:
    QSettings settings;
    QString _durationString;
    QTimer fileRotateTimer;
    QTimer durationStringTimer;
    QString _savePath;
    int _recordLenght;
    QString _recordName;
    QAudioEncoderSettings audioSettings;
    QAudioRecorder *audioRecorder;
    bool _recording;
    QString _filePath;
    QTime _recordStartTime;
};

#endif // SIMPLERECORDER_H
