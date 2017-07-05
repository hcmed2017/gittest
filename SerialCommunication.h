#ifndef SERIALCOMMUNICATION_H
#define SERIALCOMMUNICATION_H

#include <QDialog>
#include <QtSerialPort/QSerialPort>
#include "SerialCommunication.h"
#include "SerialSettingsDialog.h"

//class SerialSettingsDialog;
//class DataProcessThread;
class MySerialPort;

class SerialCommunication : public QObject
{
    Q_OBJECT

public:
    static SerialCommunication * GetInstance()
        {
            if(pSerialInstance == NULL)  //判断是否第一次调用
                pSerialInstance = new SerialCommunication();
            return pSerialInstance;
        }

private:
    SerialCommunication(QWidget *parent = 0);
    ~SerialCommunication();

    static SerialCommunication *pSerialInstance;

    class CGarbo   //它的唯一工作就是在析构函数中删除CSingleton的实例
        {
        public:
            ~CGarbo(){
                if(SerialCommunication::pSerialInstance)
                    delete SerialCommunication::pSerialInstance;
            }
        };
        static CGarbo Garbo;  //定义一个静态成员变量，程序结束时，系统会自动调用它的析构函数

private slots:
//    void sendRequestData(const QString &str);
//    void handleResponseData(const QString &str);
//    void updateSettings();

private:
    int transactionCount;
    SerialSettingsDialog *settingsDialog;
    MySerialPort *mySerial;
};



#endif // SERIALSETTINSDIALOG_H
