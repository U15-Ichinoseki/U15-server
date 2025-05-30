#-------------------------------------------------
#
# Project created by QtCreator 2015-03-20T02:08:40
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += multimedia
#QMAKE_CXXFLAGS += -std=c++11

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = U15-Server
TEMPLATE = app


SOURCES += main.cpp\
    CustomRadioButton.cpp \
    ScoreLabelStyle.cpp \
    animation.cpp \
    dynamicallylabel.cpp \
    game.cpp \
        mainwindow.cpp \
    startupdialog.cpp \
    GameSystem.cpp \
    TcpClient.cpp \
    BaseClient.cpp \
    ComClient.cpp \
    GameBoard.cpp \
    ManualClientDialog.cpp \
    ManualClient.cpp \
    MapEditerDialog.cpp \
    SettingDialog.cpp \
    ClientSettingForm.cpp \
    StableLog.cpp

HEADERS  += mainwindow.h \
    CustomRadioButton.h \
    animation.h \
    dynamicallylabel.h \
    game.h \
    scorelabelstyle.h \
    startupdialog.h \
    GameSystem.h \
    TcpClient.h \
    BaseClient.h \
    ComClient.h \
    GameBoard.h \
    ManualClientDialog.h \
    ManualClient.h \
    MapEditerDialog.h \
    SettingDialog.h \
    ClientSettingForm.h \
    Definition.h \
    StableLog.h

FORMS    += mainwindow.ui \
    CustomRadioButton.ui \
    dynamicallyLabel.ui \
    startupdialog.ui \
    GameBoard.ui \
    ManualClientDialog.ui \
    MapEditerDialog.ui \
    SettingDialog.ui \
    ClientSettingForm.ui

RESOURCES += \
    resource.qrc

win32:{
    RC_FILE += icon.rc
}
mac:{
    ICON += icon.icns
}
