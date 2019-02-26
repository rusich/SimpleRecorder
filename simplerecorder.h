#ifndef SIMPLERECORDER_H
#define SIMPLERECORDER_H

#include <QObject>
#include <QSettings>
#include <QAudioRecorder>
#include <QtMultimedia>

class SimpleRecorder : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int recordLenght READ getRecordLenght WRITE setRecordLenght)
    Q_PROPERTY(QString recordName READ getRecordName WRITE setRecordName)
    Q_PROPERTY(bool isRecording READ getRecording NOTIFY recordingChanged)
public:
    explicit SimpleRecorder(QString PathToSaveRecords = "/tmp/SimpleRecorder", QObject *parent = nullptr);
    inline int getRecordLenght() { return _recordLenght; }
    inline void setRecordLenght(int lenghtInMinutes) { _recordLenght = lenghtInMinutes; }
    inline QString getRecordName() { return _recordName; }
    inline void setRecordName(QString newRecordName) { _recordName = newRecordName; }
    inline bool getRecording() { return  _recording; }
    Q_INVOKABLE void startRecord();
    Q_INVOKABLE void stopRecord();
    ~SimpleRecorder();

signals:
    void recordingChanged();
public slots:
    void printAudioRecorderError();
private:
    QSettings settings;
    QString savePath;
    int _recordLenght;
    QString _recordName;
    QAudioEncoderSettings audioSettings;
    QAudioRecorder audioRecorder;
    bool _recording;
};

#endif // SIMPLERECORDER_H
