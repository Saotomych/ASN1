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

SOURCES += src/berByteArrayOutputStream.cpp \
	src/berIdentifier.cpp \
    src/berLength.cpp \
    src/berBase.cpp \
    src/types/berBitString.cpp \
    src/types/berBoolean.cpp \
    src/types/berInteger.cpp \
    src/types/berNull.cpp \
    src/types/berObjectIdentifier.cpp \
    src/types/berOctetString.cpp \
    src/types/berReal.cpp \
    src/types/strings/berVisibleString.cpp

HEADERS += src/asn1_global.h \
    src/berByteArrayOutputStream.h \
	src/berIdentifier.h \
    src/berLength.h \
    src/berBase.h \
    src/types/berAny.h \
    src/types/berBitString.h \
    src/types/berBoolean.h \
    src/types/berEnum.h \
    src/types/berGeneralizedTime.h \
    src/types/berInteger.h \
    src/types/berNull.h \
    src/types/berObjectIdentifier.h \
    src/types/berOctetString.h \
    src/types/berReal.h \
    src/types/strings/berBMPString.h \
    src/types/strings/berGeneralString.h \
    src/types/strings/berGraphicString.h \
    src/types/strings/berIA5String.h \
    src/types/strings/berNumericString.h \
    src/types/strings/berPrintableString.h \
    src/types/strings/berTeletexString.h \
    src/types/strings/berUniversalString.h \
    src/types/strings/berUTF8String.h \
    src/types/strings/berVideotexString.h \
    src/types/strings/berVisibleString.h
    
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
INCLUDEPATH += src src/types src/types/strings

CONFIG (debug, debug|release){
    OBJECTS_DIR = build/debug
    DEFINES += DEBUG
} else {
    OBJECTS_DIR = build/release
}

