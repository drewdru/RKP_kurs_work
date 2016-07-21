#-------------------------------------------------
#
# Project created by QtCreator 2015-03-17T17:15:07
#
#-------------------------------------------------

QT += core gui
QT += sql
QT += printsupport
QT += webkit webkitwidgets
QT += concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gkh
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Admin.cpp \
    Cashier.cpp \
    FindUserCBox.cpp \
    Payment.cpp \
    FindAccoundCBox.cpp \
    AddNewFlatHouse.cpp \
    EmergencyService.cpp \
    FindEmergencyCBox.cpp \
    Accountant.cpp \
    webpageparser.cpp \
    EmergencyBid.cpp \
    OperatorBD.cpp \
    qcustomplot.cpp \
    admingraph.cpp \
    accountantgraph.cpp \
    operatorgraph.cpp \
    emergencyservicegraph.cpp

HEADERS  += mainwindow.h \
    Admin.h \
    Cashier.h \
    FindUserCBox.h \
    Payment.h \
    FindAccoundCBox.h \
    AddNewHouse.h \
    EmergencyService.h \
    FindEmergencyCBox.h \
    Accountant.h \
    webpageparser.h \
    EmergencyBid.h \
    OperatorBD.h \
    qcustomplot.h

FORMS    += mainwindow.ui \
    Admin.ui \
    Cashier.ui \
    FindUserCBox.ui \
    Payment.ui \
    FindAccoundCBox.ui \
    AddNewHouse.ui \
    EmergencyService.ui \
    FindEmergencyCBox.ui \
    Accountant.ui \
    EmergencyBid.ui \
    OperatorBD.ui
#win32:TARGET = ../MyForm
