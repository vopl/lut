#-------------------------------------------------
#
# Project created by QtCreator 2014-01-27T15:10:42
#
#-------------------------------------------------

QT       -= core

QT       -= gui

TARGET = untitled2534
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

HEADERS += pool_c.hpp pool_c_st.hpp FSBAllocator.hh

LIBS += -lboost_system

QMAKE_CXXFLAGS += -std=c++11 -g
QMAKE_LFLAGS += -std=c++11 -g

QMAKE_CXXFLAGS_RELEASE += -O2 -DNDEBUG -static-libgcc
QMAKE_LFLAGS_RELEASE += -static-libgcc
