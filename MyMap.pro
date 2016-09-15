#-------------------------------------------------
#
# Project created by QtCreator 2015-12-09T17:22:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyMap
TEMPLATE = app


SOURCES += main.cpp\
    coord.cpp \
    draw.cpp \
    dataload.cpp \
    graph.cpp \
    mainwindow.cpp \
    spath.cpp \
    kdtree.cpp \
    segtree.cpp \
    top-k.cpp \
    taxidata.cpp \
    findbyname.cpp \
    mysignal.cpp

HEADERS  += \
    coord.h \
    data.h \
    dataload.h \
    draw.h \
    pugiconfig.hpp \
    pugixml.hpp \
    graph.h \
    mainwindow.h \
    spath.h \
    kdtree.h \
    segtree.h \
    top-k.h \
    taxidata.h \
    findbyname.h \
    mysignal.h

INCLUDEPATH += /usr/local/include/opencv /usr/local/include /usr/local/include/boost

LIBS += -L/usr/local/lib -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_nonfree -lopencv_objdetect -lopencv_ocl -lopencv_photo -lopencv_stitching -lopencv_superres -lopencv_ts -lopencv_video -lopencv_videostab

LIBS += -lboost_system-mt #-lboost_filesystem-mt

QMAKE_MAC_SDK = macosx10.11

QMAKE_CXXFLAGS += -O2

FORMS += \
    mainwindow.ui
