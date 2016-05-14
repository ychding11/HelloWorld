QT += widgets

HEADERS       = mainwindow.h \
                renderthread.h
SOURCES       = main.cpp \
                mainwindow.cpp \
                renderthread.cpp

unix:!mac:!vxworks:!integrity:!haiku:LIBS += -lm

# install
target.path = $$[QT_INSTALL_EXAMPLES]/corelib/threads/mandelbrot
INSTALLS += target
