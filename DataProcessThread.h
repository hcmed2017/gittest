#ifndef DATAPROCESSTHREAD_H
#define DATAPROCESSTHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QtSerialPort/QSerialPort>
//#include "SerialSettingsDialog.h"
class SerialSettingsDialog;

class DataProcessThread : public QThread
{
    Q_OBJECT

public:
    struct SerialSettings {
        QString name;
        qint32 baudRate;
        QSerialPort::DataBits dataBits;
        QSerialPort::Parity parity;
        QSerialPort::StopBits stopBits;
        QSerialPort::FlowControl flowControl;
        bool localEchoEnabled;
    };

    DataProcessThread(QObject *parent = 0);
    ~DataProcessThread();
    void transaction(const QString &portName, int waitTimeout, const QString &request);
    void run();
    void setCurrentPortChangeFlag(bool);
    void updateSerialSettings(QSerialPort &);
    void printSerialSettings(QSerialPort &);

    SerialSettings serialSettings;

signals:
    void response(const QString &s);
    void error(const QString &s);
    void timeout(const QString &s);

private:
    QString currentPortName;
    QString portName;
    QString request;
    int waitTimeout;
    QMutex mutex;
    QWaitCondition cond;
    bool quit;
    bool currentPortSettingsChanged;
};

#endif // SERIALTHREAD_H
