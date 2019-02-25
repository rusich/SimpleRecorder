#ifndef SIMPLERECORDER_H
#define SIMPLERECORDER_H

#include <QObject>
#include <QSettings>

class SimpleRecorder : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int recordLenght READ getRecordLenght WRITE setRecordLenght)
public:
    explicit SimpleRecorder(QString PathToSaveRecords = "/tmp/SimpleRecorder", QObject *parent = nullptr);
    inline void setRecordLenght(int lenghtInMinutes) { _recordLenght = lenghtInMinutes; }
    inline int getRecordLenght() { return _recordLenght; }
    ~SimpleRecorder();

signals:

public slots:
private:
    QSettings settings;
    QString savePath;
    int _recordLenght;
};

#endif // SIMPLERECORDER_H
