#-------------------------------------------------
#
# Project created by QtCreator 2018-02-20T17:21:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = image_scissor
TEMPLATE = app

INCLUDEPATH += /opt/ros/kinetic/include/opencv-3.3.1/ \
                /opt/ros/kinetic/include/opencv-3.3.1/opencv \
                /opt/ros/kinetic/include/opencv-3.3.1/opencv2

LIBS += /opt/ros/kinetic/lib/libopencv_*.so


SOURCES += main.cpp \
            mainwindow.cpp \
            image_scissor.cpp

HEADERS  += mainwindow.h \
            image_scissor.h

FORMS    += mainwindow.ui
