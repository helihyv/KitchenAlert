#-------------------------------------------------
#
# Project created by QtCreator 2010-05-25T15:17:43
#
#-------------------------------------------------

QT       += core gui

TARGET = KitchenAlert
TEMPLATE = app


SOURCES += main.cpp\
        kitchenalertmainwindow.cpp \
    choosetimersequencedialog.cpp \
    createtimersequencedialog.cpp \
    timer.cpp \
    currentalertstablemodel.cpp

HEADERS  += kitchenalertmainwindow.h \
    choosetimersequencedialog.h \
    createtimersequencedialog.h \
    timer.h \
    currentalertstablemodel.h

FORMS    += kitchenalertmainwindow.ui \
    choosetimersequencedialog.ui \
    createtimersequencedialog.ui

CONFIG += mobility
MOBILITY = 

symbian {
    TARGET.UID3 = 0xe1020059
    # TARGET.CAPABILITY += 
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x020000 0x800000
}
