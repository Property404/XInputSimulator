TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

TARGET = XInputSimulatorMain

SOURCES += main.cpp \
    xinputsimulatorimpl.cpp \
    xinputsimulator.cpp \
    xinputsimulatorimplunix.cpp \
    notimplementedexception.cpp \
    xinputsimulatorimplmacos.cpp \
    xinputsimulatorimplwin.cpp

HEADERS += \
    xinputsimulator.h \
    xinputsimulatorimpl.h \
    xinputsimulatorimplunix.h \
    notimplementedexception.h \
    xinputsimulatorimplmacos.h \
    xinputsimulatorimplwin.h

macx {
# mac only
    LIBS += -framework ApplicationServices
}
unix:!macx{
# unix only
    LIBS += -lX11 -lXtst
}
win32 {
# windows only
    LIBS += "C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib\User32.Lib"
}
