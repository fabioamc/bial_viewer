#-------------------------------------------------
#
# Project created by QtCreator 2015-09-12T11:52:54
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = bialgui
TEMPLATE = app

QMAKE_CXXFLAGS += -O0 -Wno-unused-function -Wno-unused-parameter \
-I$$PWD/../bial/inc -I$$PWD/../bial/src -I$$PWD/../bial/lsh/inc \
-I$$PWD/../bial/cpplex/inc -I$$PWD/../bial/SLIC/inc -I$$PWD/../bial/zlib -DREAL_FLOAT -DBIAL_$(basename $(notdir $(@)))

QMAKE_CXXFLAGS += -DBIAL_EXPLICIT_LIB
QMAKE_CXXFLAGS_DEBUG -= -pipe
QMAKE_CXXFLAGS_RELEASE -= -pipe
QMAKE_CXXFLAGS_DEBUG += -DBIAL_DEBUG=1

win32{
QMAKE_CXXFLAGS += -fopenmp
QMAKE_LFLAGS += -fopenmp

Release:DESTDIR = $$PWD/../build/win/release/
Release:OBJECTS_DIR = $$PWD/../build/win/release/obj

Debug:DESTDIR = $$PWD/../build/win/debug/
Debug:OBJECTS_DIR = $$PWD/../build/win/debug/obj
}

unix: !macx{
QMAKE_CXXFLAGS += -fopenmp -std=c++11
QMAKE_LFLAGS += -std=c++11 -fopenmp
}

macx{
QMAKE_CXXFLAGS += -stdlib=libc++ -std=c++11
QMAKE_LFLAGS += -stdlib=libc++

Release:DESTDIR = $$PWD/../build/mac/release/
Release:OBJECTS_DIR = $$PWD/../build/mac/release/obj

Debug:DESTDIR = $$PWD/../build/mac/debug/
Debug:OBJECTS_DIR = $$PWD/../build/mac/debug/obj
}

unix{

CONFIG(release, debug|release):DESTDIR = $$PWD/../build/linux/release/
CONFIG(release, debug|release):OBJECTS_DIR = $$PWD/../build/linux/release/obj

CONFIG(debug, debug|release):DESTDIR = $$PWD/../build/linux/debug/
CONFIG(debug, debug|release):OBJECTS_DIR = $$PWD/../build/linux/debug/obj
}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build/win/release/lib -lbial
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build/win/debug/lib -lbial
else:unix: CONFIG(release, debug|release): LIBS += -L$$PWD/../build/linux/release/lib -lbial
else:unix: CONFIG(debug, debug|release): LIBS += -L$$PWD/../build/linux/debug/lib -lbial


unix: CONFIG(release, debug|release): QMAKE_RPATHDIR += $$PWD/../build/linux/release/lib
else:unix: CONFIG(debug, debug|release): QMAKE_RPATHDIR += $$PWD/../build/linux/debug/lib


INCLUDEPATH += $$PWD/../bial/inc
DEPENDPATH += $$PWD/../bial/inc

CONFIG += c++11

LIBS += -lz

win32{
LIBS += -fopenmp
}

unix: !macx{
LIBS += -fopenmp
}

RESOURCES += \
    qrs/resources.qrc

FORMS += \
    ui/mainwindow.ui \
    ui/controlswidget.ui \
    ui/imagewidget.ui \
    ui/labelswidget.ui \
    ui/aboutdialog.ui

HEADERS += \
    src/mainwindow.h \
    src/controlswidget.h \
    src/controller.h \
    src/guiimage.h \
    src/gdcm.h \
    src/tool.h \
    src/graphicsscene.h \
    src/imageviewer.h \
    src/imagewidget.h \
    src/graphicsview.h \
    src/navigationtool.h \
    src/labelswidget.h \
    ui/aboutdialog.h

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/controlswidget.cpp \
    src/controller.cpp \
    src/guiimage.cpp \
    src/gdcm.cpp \
    src/tool.cpp \
    src/graphicsscene.cpp \
    src/imageviewer.cpp \
    src/imagewidget.cpp \
    src/graphicsview.cpp \
    src/navigationtool.cpp \
    src/labelswidget.cpp \
    ui/aboutdialog.cpp

DISTFILES += \
    uncrustify.cfg

#LIBS += -lopencv_core -lopencv_ml
#QMAKE_CXXFLAGS += -DOPENCV
