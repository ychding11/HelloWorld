#GraphViz librairie
DEFINES += WITH_CGRAPH
INCLUDEPATH += utils
QMAKE_CXXFLAGS += -DQGVCORE_LIB

unix {
 CONFIG += link_pkgconfig
 PKGCONFIG += libcdt libgvc libcgraph
}
win32 {
 #Configure Windows GraphViz path here :
 GRAPHVIZ_PATH = "D:/Program Files (x86)/Graphviz2.36/"
 DEFINES += WIN32_DLL
 DEFINES += GVDLL
 INCLUDEPATH += $$GRAPHVIZ_PATH/include/graphviz
 LIBS += -L$$GRAPHVIZ_PATH/lib/release/lib -lgvc -lcgraph -lgraph -lcdt
}
