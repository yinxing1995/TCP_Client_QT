TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

QT+ = network
LIBS += -lpthread libwsock32 libws2_32
