#-------------------------------------------------
#
# Project created by QtCreator 2018-10-19T01:58:49
#
#-------------------------------------------------

QT       += core gui
win32:RC_ICONS += icon.ico


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = quarto
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    1.jpg \
    images/enabled/1.png \
    images/enabled/10.png \
    images/enabled/11.png \
    images/enabled/12.png \
    images/enabled/13.png \
    images/enabled/14.png \
    images/enabled/15.png \
    images/enabled/16.png \
    images/enabled/2.png \
    images/enabled/3.png \
    images/enabled/4.png \
    images/enabled/5.png \
    images/enabled/6.png \
    images/enabled/7.png \
    images/enabled/8.png \
    images/enabled/9.png

RESOURCES += \
    images.qrc


