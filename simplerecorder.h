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
    Q_PROPERTY(qint64 duration READ getDuration)
public:
    explicit SimpleRecorder(QString PathToSaveRecords = "/tmp/SimpleRecorder", QObject *parent = nullptr);
    inline int getRecordLenght() { return _recordLenght; }
    inline void setRecordLenght(int lenghtInMinutes) { _recordLenght = lenghtInMinutes; }
    inline QString getRecordName() { return _recordName; }
    inline void setRecordName(QString newRecordName) { _recordName = newRecordName; }
    QString getFileName();
    inline bool getRecording() { return  _recording; }
    Q_INVOKABLE void startRecord();
    Q_INVOKABLE void stopRecord();
    Q_INVOKABLE void stopRecordRotation();
    inline qint64 getDuration() { return _recording? audioRecorder->duration(): 0; }
    ~SimpleRecorder();

signals:
    void recordingChanged();
    void filePathChanged();
public slots:
    void rotateRecordFile();
private:
    QSettings settings;
    QTimer fileRotateTimer;
    QString savePath;
    int _recordLenght;
    QString _recordName;
    QAudioEncoderSettings audioSettings;
    QAudioRecorder *audioRecorder;
    bool _recording;
    QString _filePath;
};

#endif // SIMPLERECORDER_H
