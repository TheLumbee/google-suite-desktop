######################################################################
# Automatically generated by qmake (3.1) Tue Feb 15 17:18:39 2022
######################################################################

TEMPLATE = app
TARGET = GCal-Desktop
INCLUDEPATH += .
QT += core network opengl webenginecore webenginewidgets widgets
CONFIG += debug_and_release

# You can make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# Please consult the documentation of the deprecated API in order to know
# how to port your code away from it.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
HEADERS += WebView/CalendarView.hpp

SOURCES += main.cpp \
           WebView/CalendarView.cpp


