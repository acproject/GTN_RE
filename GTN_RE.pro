#-------------------------------------------------
#
# Project created by QtCreator 2020-11-24T15:52:51
#
#-------------------------------------------------

QT       += widgets declarative network sql script scripttools svg webengine webenginewidgets webkit xml

TARGET = GTN_RE
TEMPLATE = lib

DEFINES += GTN_RE_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        gtn_re.cpp \
    graph.cpp \
    autograd.cpp

HEADERS += \
        gtn_re.h \
        gtn_re_global.h \ 
    graph.h \
    autograd.h

unix {
    target.path = /usr/local/lib
    INSTALLS += target
}
