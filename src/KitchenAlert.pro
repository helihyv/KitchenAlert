#-------------------------------------------------
#
# Project created by QtCreator 2010-05-25T15:17:43
#
#-------------------------------------------------

QT       += core gui
QT      += phonon

TARGET = KitchenAlert
TEMPLATE = app


SOURCES += main.cpp\
        kitchenalertmainwindow.cpp \
    createtimersequencedialog.cpp \
    timer.cpp \
    currentalertstablemodel.cpp \
    alertsound.cpp \
    selectsounddialog.cpp

HEADERS  += kitchenalertmainwindow.h \
    createtimersequencedialog.h \
    timer.h \
    currentalertstablemodel.h \
    alertsound.h \
    selectsounddialog.h

FORMS    += kitchenalertmainwindow.ui \
    createtimersequencedialog.ui \
    selectsounddialog.ui \
    kitchenalertmainwindow.ui

CONFIG += mobility
MOBILITY =



symbian {
    TARGET.UID3 = 0xe1020059
    # TARGET.CAPABILITY += 
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x020000 0x800000
}

RESOURCES += \
    kitchenalert.qrc
