#include <QApplication>
#include "SerialSettingsDialog.h"
#include "SerialCommunication.h"
#include "myserialport.h"
//SerialCommunication *w = NULL;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MySerialPort *serial = MySerialPort::GetInstance();
    //serial.settingsDialog->show();
    //SerialCommunication *w = SerialCommunication::GetInstance();
    //w.show();

    return a.exec();
}
