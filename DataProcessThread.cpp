#include "DataProcessThread.h"

#include <QtSerialPort/QSerialPort>
#include <QTime>
#include <QDebug>
#include "SerialSettingsDialog.h"
#include "SerialCommunication.h"


DataProcessThread::DataProcessThread(QObject *parent):
    QThread(parent),
    waitTimeout(0),
    quit(false),
    currentPortSettingsChanged(false)
{
}

DataProcessThread::~DataProcessThread()
{
    mutex.lock();
    quit = true;
    cond.wakeOne();
    mutex.unlock();
    wait();
}

void DataProcessThread::transaction(const QString &portName, int waitTimeout, const QString &request)
{
    QMutexLocker locker(&mutex);
    this->portName = portName;
    this->waitTimeout = waitTimeout;
    this->request = request;

    if (!isRunning())
    {
        start();
    }
    else
    {
        cond.wakeOne();
    }
}

void DataProcessThread::run()
{
    bool currentPortNameChanged = false;

    mutex.lock();

    if (currentPortName != portName)
    {
        currentPortName = portName;
        currentPortNameChanged = true;
    }

    int currentWaitTimeout = waitTimeout;
    QString currentRequest = request;
    mutex.unlock();

    QSerialPort serial;
    updateSerialSettings(serial); // this place use reference of serial, whether it is safe?

    while (!quit)
    {
        if(currentPortSettingsChanged)
        {
            updateSerialSettings(serial);
            currentPortSettingsChanged = false;
            printSerialSettings(serial);
        }

        if (currentPortNameChanged)
        {
            serial.close();

            if (!serial.open(QIODevice::ReadWrite))
            {
                emit error(tr("Can't open %1, error code %2")
                           .arg(portName).arg(serial.error()));
                return;
            }
        }

        // write request
        QByteArray requestData = currentRequest.toLocal8Bit();
        serial.write(requestData);
        if (serial.waitForBytesWritten(waitTimeout))
        {
            // read response
            if (serial.waitForReadyRead(currentWaitTimeout))
            {
                QByteArray responseData = serial.readAll();
                while (serial.waitForReadyRead(10))
                    responseData += serial.readAll();

                //qDebug()<<"QByteArray type responseData:"<<responseData;
                QString response(responseData);

                emit this->response(response);

            }
            else
            {
                emit timeout(tr("wait read response timeout %1")
                             .arg(QTime::currentTime().toString()));
            }

        }
        else
        {
            emit timeout(tr("wait write request timeout %1")
                         .arg(QTime::currentTime().toString()));
        }

        mutex.lock();
        cond.wait(&mutex);
        if (currentPortName != portName)
        {
            currentPortName = portName;
            currentPortNameChanged = true;
        }
        else
        {
            currentPortNameChanged = false;
        }
        currentWaitTimeout = waitTimeout;
        currentRequest = request;
        mutex.unlock();
    }

}

void DataProcessThread::setCurrentPortChangeFlag(bool val)
{
    currentPortSettingsChanged = val;
}

void DataProcessThread::updateSerialSettings(QSerialPort &serial)
{
    qDebug()<<"update serial settings";
    serial.setPortName(currentPortName);
    serial.setPortName(serialSettings.name);
    serial.setBaudRate(serialSettings.baudRate);
    serial.setDataBits(serialSettings.dataBits);
    serial.setParity(serialSettings.parity);
    serial.setStopBits(serialSettings.stopBits);
    serial.setFlowControl(serialSettings.flowControl);
}

void DataProcessThread::printSerialSettings(QSerialPort &serial)
{
    qDebug()<< serial.portName();
    qDebug()<< serial.baudRate();
    qDebug()<< serial.dataBits();
    qDebug()<< serial.parity();
    qDebug()<< serial.stopBits();
    qDebug()<< serial.flowControl();
}

