#-------------------------------------------------
#
# Project created by QtCreator 2013-04-17T09:06:06
#
#-------------------------------------------------

QT       += core

greaterThan(QT_MAJOR_VERSION, 4){
    QT += widgets
}
lessThan(QT_MAJOR_VERSION, 5) {
    QT += gui
}

TARGET = QGraphviz
TEMPLATE = lib
CONFIG += shared
CONFIG += debug
CONFIG += c++11
QMAKE_CXXFLAGS += -g3

DESTDIR = ../lib
DLLDESTDIR = ../bin

#GraphViz librairie
!include(utils.pri) {
     error("fail open GraphViz.pri")
 }


#QGVEdge.cpp \
#QGVSubGraph.cpp \

SOURCES += QGVScene.cpp \
    QGVNode.cpp \
    QGVEdge.cpp \
	utils/QGVCore.cpp \

HEADERS  += QGVScene.h \
    QGVNode.h \
    QGVEdge.h \
    QGVSubGraph.h \
	utils/QGVCore.h \
    qgv.h
