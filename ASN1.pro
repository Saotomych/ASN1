#-------------------------------------------------
#
# Project created by QtCreator 2015-09-24T16:52:21
#
#-------------------------------------------------

QT       += network

QT       -= gui

TEMPLATE = lib
CONFIG += staticlib

DESTDIR = bin
VERSION = 0.0.0.0
CONFIG += build_all
CONFIG += c++11

DEFINES += ASN1_LIBRARY

SOURCES += berByteArrayOutputStream.cpp \
	berIdentifier.cpp \
    berLength.cpp 

HEADERS += asn1_global.h \
    berByteArrayOutputStream.h \
	berIdentifier.h \
    berLength.h 
    
unix {
    CONFIG (debug, debug|release) {
        TARGET = $$qtLibraryTarget(asn1d)
    }else{
        TARGET = $$qtLibraryTarget(asn1)
    }
    target.path = /usr/lib
    INSTALLS += target
}else{
    TARGET = $$qtLibraryTarget(asn1)
}

CONFIG += debug_and_release build_all
INCLUDEPATH += include/ASN1

CONFIG (debug, debug|release){
    OBJECTS_DIR = build/debug
    DEFINES += DEBUG
} else {
    OBJECTS_DIR = build/release
}

