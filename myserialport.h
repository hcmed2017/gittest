//By realfan   2015.1
#ifndef MYSERIALPORT_H
#define MYSERIALPORT_H

#include <QObject>
#include <QSemaphore>
#include <QMutex>
#include <QRegExp>
#include <SerialSettingsDialog.h>
class QThread;
class QSerialPort;
//class SerialSettingsDialog;

class MySerialPort : public QObject
{
    Q_OBJECT

public:
    static MySerialPort * GetInstance();

private:
    explicit MySerialPort();
    ~MySerialPort();

public:
    bool isOpen() const;
    bool setCOM(const QString strCOM = "", const int iBautRate = 9600, const int iDataBits = 8, const char chParity = 'N', const char chStopBits = 1);
    void write(const QByteArray &byteArray);
    qint64 write(const unsigned char *data, qint64 maxSize = -1);
    void close();
    void clear();
    //bool EchoCommand(const QString & strCmd, const QRegExp& rx = QRegExp(""), QString * pstrReply = 0);
    //bool EchoCommand(const QString &strCmd, const QString &strExpectReply = "", QString *pstrReply = 0);
    void updateSerialSetting(QSerialPort*);
    void printSerialSetting(QSerialPort*);
    //==================================================
protected:
    QString m_strComName;
    //========================
    QThread *m_pThread;
    QSerialPort *m_pCom;
    //=========================
    //write
    QMutex m_lockWrite;
    QSemaphore m_semWrite;
    QMutex m_lockWriteLen;
    qint64 m_iLen;
    //openCom
    QMutex m_lockSetCOM;
    QSemaphore m_semSetCOM;
    bool m_bOpen;
    //close
    QMutex m_lockClose;
    QSemaphore m_semClose;
    //clear
    QMutex m_lockClear;
    QSemaphore m_semClear;
    //=========================
    QMutex m_lockInBuffer;
    QString m_strInBuffer;
    QString m_strReply;

    QMutex m_lockExpect;
    QString m_strExpectReply;
    QRegExp m_rxExpectReply;

    QMutex m_lockEcho;
    QSemaphore m_semEcho;
    bool m_bEchoRegExp;

private:
    bool m_bEchoFlag;
    mutable QMutex m_lockEchoFlag;
    void setEchoFlag(bool b){
        QMutexLocker lk(&m_lockEchoFlag);
        m_bEchoFlag = b;
    }
    bool getEchoFlag() const{
        QMutexLocker lk(&m_lockEchoFlag);
        return m_bEchoFlag;
    }

signals:
    void sigDataReady(QByteArray);
    void sigResponseData(QString);
    void sigWrite(QByteArray);
    void sigSetCOM(const QString, const int, const int, const char, const char);
    void sigClose();
    void sigClear();

private slots:
    void slotDataReady();
    //-----------------
    //void slotWrite(const unsigned char *pch, qint64);
    void slotWrite(const QByteArray &data);
    void slotSetCOM(const QString strCOM, const int iBautRate, const int iDataBits, const char chParity, const char chStopBits);
    void slotClose();
    void slotClear();

private:
    SerialSettingsDialog *settingsDialog;
    SerialSettingsDialog::Settings currentSettings;

public:
    void updateSerialSetting();
};

#endif // MYSERIALPORT_H
