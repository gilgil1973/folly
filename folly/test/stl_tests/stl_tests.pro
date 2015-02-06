#-------------------------------------------------
#
# Project created by QtCreator 2015-02-06T15:48:21
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = stl_tests
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app

include(/root/proj/fb/proxygen/proxygen/fbthrift/thrift/folly/folly.pri)

LIBS += -lgflags -lgtest

SOURCES += \
    StlVectorTest.cpp


