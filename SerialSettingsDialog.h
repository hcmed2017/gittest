#ifndef SERIALSETTINGSDIALOG_H
#define SERIALSETTINGSDIALOG_H

#include <QDialog>
#include <QtSerialPort/QSerialPort>
#include "DataProcessThread.h"

namespace Ui {
class SerialSettingsDialog;
}
class QIntValidator;
class MySerialPort;


class SerialSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    struct Settings {
        QString name;
        qint32 baudRate;
        QString stringBaudRate;
        QSerialPort::DataBits dataBits;
        QString stringDataBits;
        QSerialPort::Parity parity;
        QString stringParity;
        QSerialPort::StopBits stopBits;
        QString stringStopBits;
        QSerialPort::FlowControl flowControl;
        QString stringFlowControl;
        bool localEchoEnabled;
    };

    explicit SerialSettingsDialog(QWidget *parent = 0);
    ~SerialSettingsDialog();

    Settings getCurrentSettings() const;
    void setControlsEnabled(bool enable);
    int getWaitResponseTime();

signals:
    //void sendCommand(const QString &s);
    //void sendCommand(const QByteArray &byteArray);
    void sendCommand(const char *, qint64);
    void sigUpdateSettings();

private slots:
    void showPortInfo(int idx);
    void checkCustomBaudRatePolicy(int idx);
    void checkCustomDevicePathPolicy(int idx);

    void transaction();
    void showResponse(const QString &s);
    void processError(const QString &s);
    void processTimeout(const QString &s);

    void on_sendButton_clicked();
    void on_applyButton_clicked();

    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_10_clicked();
    void on_pushButton_11_clicked();

private:
    void fillPortsParameters();
    void fillPortsInfo();
    void updateSettings();

private:
    Ui::SerialSettingsDialog *ui;
    int transactionCount;
    Settings currentSettings;
    QIntValidator *intValidator;
};

#endif // SERIALSETTINGSDIALOG_H
