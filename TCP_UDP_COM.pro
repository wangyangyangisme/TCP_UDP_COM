#-------------------------------------------------
#
# Project created by QtCreator 2012-01-09T11:10:15
#
#-------------------------------------------------

QT       += core gui
QT       +=network

TARGET = TCP_UDP_COM
TEMPLATE = app

win32 {
     SOURCES += qextserial/qextserialport_win.cpp
}

unix {
     SOURCES += qextserial/qextserialport_unix.cpp
}

SOURCES += main.cpp\
        mainwindow.cpp \
    udp.cpp \
    com.cpp \
    tcpclient.cpp \
    tcpserver.cpp \
    tcpudpcombase.cpp \
    mystyle.cpp \
    sysutils.cpp \
    qextserial/qextserialport.cpp

HEADERS  += mainwindow.h \
    udp.h \
    com.h \
    tcpclient.h \
    tcpserver.h \
    tcpudpcombase.h \
    mystyle.h \
    sysutils.h \
    qextserial/qextserialport_p.h \
    qextserial/qextserialport_global.h \
    qextserial/qextserialport.h

FORMS    += mainwindow.ui \
    udp.ui \
    com.ui \
    tcpclient.ui \
    tcpserver.ui

RESOURCES += \
    images.qrc

OTHER_FILES += \
    myapp.rc

RC_FILE += myapp.rc




























