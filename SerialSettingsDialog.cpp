#include "SerialSettingsDialog.h"
#include "ui_SerialSettingsDialog.h"
#include "DataProcessThread.h"

#include <QtSerialPort/QSerialPortInfo>
#include <QIntValidator>
#include <QLineEdit>
#include <QDebug>
#include "myserialport.h"
#include <qglobal.h>
#include <QByteArray>

static const char blankString[] = QT_TRANSLATE_NOOP("SerialSettingsDialog", "N/A");


SerialSettingsDialog::SerialSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SerialSettingsDialog),
    transactionCount(0)
{
    ui->setupUi(this);

    ui->waitResponseSpinBox->setRange(0, 10000);
    ui->waitResponseSpinBox->setValue(1000);

    intValidator = new QIntValidator(0, 4000000, this);

    ui->baudRateBox->setInsertPolicy(QComboBox::NoInsert);

    connect(ui->applyButton, SIGNAL(clicked()),
            this, SLOT(on_applyButton_clicked()));
    connect(ui->serialPortInfoListBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(showPortInfo(int)));
    connect(ui->baudRateBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(checkCustomBaudRatePolicy(int)));
    connect(ui->serialPortInfoListBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(checkCustomDevicePathPolicy(int)));

    fillPortsParameters();
    fillPortsInfo();

    updateSettings();

    transaction();


    //ui->sendLineEdit->setText("abc");
}

SerialSettingsDialog::~SerialSettingsDialog()
{
    delete ui;
}

SerialSettingsDialog::Settings SerialSettingsDialog::getCurrentSettings() const
{
    return currentSettings;
}

void SerialSettingsDialog::showPortInfo(int idx)
{
    if (idx == -1)
        return;

    QStringList list = ui->serialPortInfoListBox->itemData(idx).toStringList();
    ui->descriptionLabel->setText(tr("Description: %1").arg(list.count() > 1 ? list.at(1) : tr(blankString)));
    ui->manufacturerLabel->setText(tr("Manufacturer: %1").arg(list.count() > 2 ? list.at(2) : tr(blankString)));
    ui->serialNumberLabel->setText(tr("Serial number: %1").arg(list.count() > 3 ? list.at(3) : tr(blankString)));
    ui->locationLabel->setText(tr("Location: %1").arg(list.count() > 4 ? list.at(4) : tr(blankString)));
    ui->vidLabel->setText(tr("Vendor Identifier: %1").arg(list.count() > 5 ? list.at(5) : tr(blankString)));
    ui->pidLabel->setText(tr("Product Identifier: %1").arg(list.count() > 6 ? list.at(6) : tr(blankString)));
}

void SerialSettingsDialog::checkCustomBaudRatePolicy(int idx)
{
    bool isCustomBaudRate = !ui->baudRateBox->itemData(idx).isValid();
    ui->baudRateBox->setEditable(isCustomBaudRate);
    if (isCustomBaudRate) {
        ui->baudRateBox->clearEditText();
        QLineEdit *edit = ui->baudRateBox->lineEdit();
        edit->setValidator(intValidator);
    }
}

void SerialSettingsDialog::checkCustomDevicePathPolicy(int idx)
{
    bool isCustomPath = !ui->serialPortInfoListBox->itemData(idx).isValid();
    ui->serialPortInfoListBox->setEditable(isCustomPath);
    if (isCustomPath)
        ui->serialPortInfoListBox->clearEditText();
}

void SerialSettingsDialog::transaction()
{
    //setControlsEnabled(false);
    ui->statusLabel->setText(tr("Status: Running, connected to port %1.")
                         .arg(ui->serialPortInfoListBox->currentText()));

    //emit sendCommand(ui->sendLineEdit->text());
//    thread.transaction(ui->serialPortInfoListBox->currentText(),
//                       ui->waitResponseSpinBox->value(),
//                       ui->sendLineEdit->text());
}

void SerialSettingsDialog::showResponse(const QString &s)
{
    setControlsEnabled(true);
    ui->trafficLabel->setText(tr("Traffic, transaction #%1:"
                             "\n\r-request: %2"
                             "\n\r-response: %3")
                          .arg(++transactionCount).arg(ui->sendLineEdit->text()).arg(s));
    ui->recLineEdit->setText(s);
}

void SerialSettingsDialog::processError(const QString &s)
{
    setControlsEnabled(true);
    ui->statusLabel->setText(tr("Status: Not running, %1.").arg(s));
    ui->trafficLabel->setText(tr("No traffic."));

    qDebug()<<tr("Status: Not running, %1.").arg(s);
}

void SerialSettingsDialog::processTimeout(const QString &s)
{
    setControlsEnabled(true);
    ui->statusLabel->setText(tr("Status: Running, %1.").arg(s));
    ui->trafficLabel->setText(tr("No traffic."));

    qDebug()<<tr("Status: Running, %1.").arg(s);
}

void SerialSettingsDialog::fillPortsParameters()
{
    ui->baudRateBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    ui->baudRateBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->baudRateBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->baudRateBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    ui->baudRateBox->addItem(tr("Custom"));
    ui->baudRateBox->setCurrentIndex(3); // Baud115200

    ui->dataBitsBox->addItem(QStringLiteral("5"), QSerialPort::Data5);
    ui->dataBitsBox->addItem(QStringLiteral("6"), QSerialPort::Data6);
    ui->dataBitsBox->addItem(QStringLiteral("7"), QSerialPort::Data7);
    ui->dataBitsBox->addItem(QStringLiteral("8"), QSerialPort::Data8);
    ui->dataBitsBox->setCurrentIndex(3); // Data8

    ui->parityBox->addItem(tr("None"), QSerialPort::NoParity);
    ui->parityBox->addItem(tr("Even"), QSerialPort::EvenParity);
    ui->parityBox->addItem(tr("Odd"), QSerialPort::OddParity);
    ui->parityBox->addItem(tr("Mark"), QSerialPort::MarkParity);
    ui->parityBox->addItem(tr("Space"), QSerialPort::SpaceParity);

    ui->stopBitsBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    ui->stopBitsBox->addItem(tr("1.5"), QSerialPort::OneAndHalfStop);
#endif
    ui->stopBitsBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    ui->flowControlBox->addItem(tr("None"), QSerialPort::NoFlowControl);
    ui->flowControlBox->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
    ui->flowControlBox->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);
}

void SerialSettingsDialog::fillPortsInfo()
{
    ui->serialPortInfoListBox->clear();
    QString description;
    QString manufacturer;
    QString serialNumber;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        QStringList list;
        description = info.description();
        manufacturer = info.manufacturer();
        serialNumber = info.serialNumber();
        list << info.portName()
             << (!description.isEmpty() ? description : blankString)
             << (!manufacturer.isEmpty() ? manufacturer : blankString)
             << (!serialNumber.isEmpty() ? serialNumber : blankString)
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString);

        ui->serialPortInfoListBox->addItem(list.first(), list);
    }

    ui->serialPortInfoListBox->addItem(tr("Custom"));
}

void SerialSettingsDialog::updateSettings()
{
    currentSettings.name = ui->serialPortInfoListBox->currentText();

    if (ui->baudRateBox->currentIndex() == 4) {
        currentSettings.baudRate = ui->baudRateBox->currentText().toInt();
    } else {
        currentSettings.baudRate = static_cast<QSerialPort::BaudRate>(
                    ui->baudRateBox->itemData(ui->baudRateBox->currentIndex()).toInt());
    }
    currentSettings.stringBaudRate = QString::number(currentSettings.baudRate);

    currentSettings.dataBits = static_cast<QSerialPort::DataBits>(
                ui->dataBitsBox->itemData(ui->dataBitsBox->currentIndex()).toInt());
    currentSettings.stringDataBits = ui->dataBitsBox->currentText();

    currentSettings.parity = static_cast<QSerialPort::Parity>(
                ui->parityBox->itemData(ui->parityBox->currentIndex()).toInt());
    currentSettings.stringParity = ui->parityBox->currentText();

    currentSettings.stopBits = static_cast<QSerialPort::StopBits>(
                ui->stopBitsBox->itemData(ui->stopBitsBox->currentIndex()).toInt());
    currentSettings.stringStopBits = ui->stopBitsBox->currentText();

    currentSettings.flowControl = static_cast<QSerialPort::FlowControl>(
                ui->flowControlBox->itemData(ui->flowControlBox->currentIndex()).toInt());
    currentSettings.stringFlowControl = ui->flowControlBox->currentText();

    currentSettings.localEchoEnabled = ui->localEchoCheckBox->isChecked();
}

void SerialSettingsDialog::setControlsEnabled(bool enable)
{
    ui->sendButton->setEnabled(enable);
    ui->serialPortInfoListBox->setEnabled(enable);
    ui->waitResponseSpinBox->setEnabled(enable);
    ui->sendLineEdit->setEnabled(enable);
}

int SerialSettingsDialog::getWaitResponseTime()
{
    return ui->waitResponseSpinBox->value();
}

void SerialSettingsDialog::on_sendButton_clicked()
{
     static char buf[] = "abcdef";

//    const char *temp = buf;
//    while(*temp)
//    {
//        qDebug()<<*temp;
//        temp++;
//    }

//    QByteArray ba("12345abc");
//    emit sendCommand(ba);
    emit sendCommand(buf, sizeof(buf));
    //emit sendCommand(ui->sendLineEdit->text());
}

void SerialSettingsDialog::on_applyButton_clicked()
{
    updateSettings();
    emit sigUpdateSettings();

    ui->statusLabel->setText(tr("Status: Update Serial Settings."));
    ui->trafficLabel->setText(tr("No traffic."));
}


void SerialSettingsDialog::on_pushButton_clicked()
{
    ui->sendLineEdit->insert("1");
}

void SerialSettingsDialog::on_pushButton_2_clicked()
{
    ui->sendLineEdit->insert("2");
}

void SerialSettingsDialog::on_pushButton_3_clicked()
{
    ui->sendLineEdit->insert("3");
}

void SerialSettingsDialog::on_pushButton_4_clicked()
{
    ui->sendLineEdit->insert("4");
}

void SerialSettingsDialog::on_pushButton_5_clicked()
{
    ui->sendLineEdit->insert("5");
}

void SerialSettingsDialog::on_pushButton_6_clicked()
{
    ui->sendLineEdit->insert("6");
}

void SerialSettingsDialog::on_pushButton_7_clicked()
{
    ui->sendLineEdit->insert("7");
}

void SerialSettingsDialog::on_pushButton_8_clicked()
{
    ui->sendLineEdit->insert("8");
}

void SerialSettingsDialog::on_pushButton_9_clicked()
{
    ui->sendLineEdit->insert("9");
}

void SerialSettingsDialog::on_pushButton_10_clicked()
{
    ui->sendLineEdit->insert("0");
}

void SerialSettingsDialog::on_pushButton_11_clicked()
{
    ui->sendLineEdit->clear();
}


