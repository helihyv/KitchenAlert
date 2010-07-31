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


symbian {
    TARGET.UID3 = 0xe1020059
    # TARGET.CAPABILITY += 
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x020000 0x800000
}


unix {
  #VARIABLES
  isEmpty(PREFIX) {
    PREFIX = /usr/local
  }
  BINDIR = $$PREFIX/bin
  DATADIR =$$PREFIX/share

  DEFINES += DATADIR=\\\"$$DATADIR\\\" PKGDATADIR=\\\"$$PKGDATADIR\\\"

  #MAKE INSTALL

  INSTALLS += target desktop service iconxpm icon26 icon48 icon64

  target.path =$$BINDIR

  desktop.path = $$DATADIR/applications/hildon
  desktop.files += $${TARGET}.desktop

  service.path = $$DATADIR/dbus-1/services
  service.files += $${TARGET}.service

  iconxpm.path = $$DATADIR/pixmap
  iconxpm.files += ../data/maemo/$${TARGET}.xpm

  icon26.path = $$DATADIR/icons/hicolor/26x26/apps
  icon26.files += ../data/26x26/$${TARGET}.png

  icon48.path = $$DATADIR/icons/hicolor/48x48/apps
  icon48.files += ../data/48x48/$${TARGET}.png

  icon64.path = $$DATADIR/icons/hicolor/64x64/apps
  icon64.files += ../data/64x64/$${TARGET}.png
}
