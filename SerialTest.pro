#-------------------------------------------------
#
# Project created by QtCreator 2017-06-10T17:11:43
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SerialTest
TEMPLATE = app
UI_DIR=./UI

SOURCES += main.cpp\
    DataProcessThread.cpp \
    SerialCommunication.cpp \
    SerialSettingsDialog.cpp \
    myserialport.cpp

HEADERS  += \
    DataProcessThread.h \
    SerialCommunication.h \
    SerialSettingsDialog.h \
    myserialport.h

FORMS    += \
    SerialSettingsDialog.ui
