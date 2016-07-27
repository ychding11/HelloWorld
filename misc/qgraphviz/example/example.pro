#-------------------------------------------------
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = example
TEMPLATE = app
CONFIG+=debug
CONFIG+=c++11
QMAKE_LFLAGS += "-Wl,-rpath,'\$\$ORIGIN'" 

DESTDIR = ../lib

#QGVCore librairie
LIBS += -L$$OUT_PWD/../lib -lQGraphviz
INCLUDEPATH += $$PWD/../QGraphviz
DEPENDPATH += $$PWD/../QGraphviz

#GraphViz librairie
!include(../QGraphviz/utils.pri) {
     error("fail open utils.pri")
 }

SOURCES   += main.cpp QGraphicsViewEc.cpp
HEADERS   += QGraphicsViewEc.h


#FORMS    += MainWindow.ui
#RESOURCES += ress.qrc
