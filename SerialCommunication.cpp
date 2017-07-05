#include "SerialCommunication.h"
#include "DataProcessThread.h"
#include "SerialSettingsDialog.h"
#include <QDebug>
#include <QByteArray>
#include "myserialport.h"

SerialCommunication* SerialCommunication::pSerialInstance = NULL;

SerialCommunication::SerialCommunication(QWidget *parent):
    QObject(parent),
    transactionCount(0)
{
    mySerial = MySerialPort::GetInstance();

    settingsDialog = new SerialSettingsDialog();
    settingsDialog->show();

    //updateSettings();

//    connect(&thread, SIGNAL(response(QString)),
//            this, SLOT(handleResponseData(QString)));

    /***********For Serial Debug in Setting Dialog begin***********/
    //dialog send
//    connect(settingsDialog, SIGNAL(sendCommand(QString)),
//            this, SLOT(sendRequestData(QString)));
    // dialog receive
//    connect(&thread, SIGNAL(response(QString)),
//            settingsDialog, SLOT(showResponse(QString)));
    // dialog update settings
//    connect(settingsDialog, SIGNAL(sigUpdateSettings()),
//            this, SLOT(updateSettings()));

    //dialog error hint
//    connect(&thread, SIGNAL(error(QString)),
//            settingsDialog, SLOT(processError(QString)));
    //dialog timeout hint
//    connect(&thread, SIGNAL(timeout(QString)),
//            settingsDialog, SLOT(processTimeout(QString)));
    /***********For Serial Debug in Setting Dialog end***********/
}

SerialCommunication::~SerialCommunication()
{
    //delete mySerial;
    delete settingsDialog;
}

//void SerialCommunication::handleResponseData(const QString &str)
//{
//    //setControlsEnabled(true);
//    qDebug()<<"QString type response data :"<<str;

//    QByteArray responseByteData = str.toLocal8Bit();
//    qDebug()<<"QByteArray type response data :"<<responseByteData;
//    qDebug()<<"QByteArray type （to Hex）response data :"<<responseByteData.toHex();

//    char buff[8096];
//    memset(buff, 0, 8096);
//    strncpy(buff, responseByteData.data(), responseByteData.size());
//    qDebug()<<"buff:"<<buff;
//}

//void SerialCommunication::sendRequestData(const QString &str)
//{
//    thread.transaction(settingsDialog->getCurrentSettings().name,
//                       settingsDialog->getWaitResponseTime(),
//                       str);
//}

//void SerialCommunication::updateSettings()
//{
//    thread.serialSettings.name = settingsDialog->getCurrentSettings().name;
//    thread.serialSettings.baudRate = settingsDialog->getCurrentSettings().baudRate;
//    thread.serialSettings.dataBits = settingsDialog->getCurrentSettings().dataBits;
//    thread.serialSettings.parity = settingsDialog->getCurrentSettings().parity;
//    thread.serialSettings.stopBits = settingsDialog->getCurrentSettings().stopBits;
//    thread.serialSettings.flowControl = settingsDialog->getCurrentSettings().flowControl;
//    thread.setCurrentPortChangeFlag(true);
//}

