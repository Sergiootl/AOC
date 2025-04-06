FORMS += mainForm.ui 
HEADERS += pracaoc.h \
           painter.h 
SOURCES += pracaoc.cpp \
           main.cpp \
           painter.cpp 

TEMPLATE = app
QT += core gui opengl


QMAKE_CXXFLAGS_DEBUG += -fno-omit-frame-pointer
QMAKE_CXXFLAGS_RELEASE += -fno-omit-frame-pointer
QMAKE_CFLAGS_DEBUG += -fno-omit-frame-pointer
QMAKE_CFLAGS_RELEASE += -fno-omit-frame-pointer

QMAKE_LFLAGS += -no-pie


TARGET = pracAOC

