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
    src/berByteArrayInputStream.cpp \
    src/InitIdentifiers.cpp \
    src/berIdentifier.cpp \
    src/berLength.cpp \
    src/berBase.cpp \
    src/types/berBaseOctetString.cpp \
    src/types/berBitString.cpp \
    src/types/berBoolean.cpp \
    src/types/berInteger.cpp \
    src/types/berNull.cpp \
    src/types/berObjectIdentifier.cpp \
    src/types/berReal.cpp \
    src/types/berVisibleString.cpp

HEADERS += include/asn1_global.h \
    include/berByteArrayOutputStream.h \
    include/berByteArrayInputStream.h \
	include/berIdentifier.h \
    include/berLength.h \
    include/berBase.h \
    include/berBaseOctetString.h \
    include/berAny.h \
    include/berBitString.h \
    include/berBoolean.h \
    include/berEnum.h \
    include/berGeneralizedTime.h \
    include/berInteger.h \
    include/berNull.h \
    include/berObjectIdentifier.h \
    include/berOctetString.h \
    include/berReal.h \
    include/berBMPString.h \
    include/berGeneralString.h \
    include/berGraphicString.h \
    include/berIA5String.h \
    include/berNumericString.h \
    include/berPrintableString.h \
    include/berTeletexString.h \
    include/berUniversalString.h \
    include/berUTF8String.h \
    include/berVideotexString.h \
    include/berVisibleString.h
    
unix {
    CONFIG (debug, debug|release) {
        TARGET = $$qtLibraryTarget(asn1d)
	    OBJECTS_DIR = build/debug
	    DEFINES += DEBUG
		LIBS += -lgcov

	    QMAKE_CXXFLAGS_RELEASE -= -O
		QMAKE_CXXFLAGS_RELEASE -= -O1
		QMAKE_CXXFLAGS_RELEASE -= -O2
		QMAKE_CXXFLAGS += -O0 -fprofile-arcs -ftest-coverage
    }else{
        TARGET = $$qtLibraryTarget(asn1)
	    OBJECTS_DIR = build/release
    }
    target.path = /usr/lib
    INSTALLS += target
   
}else{
    TARGET = $$qtLibraryTarget(asn1-notunix)
}

CONFIG += debug_and_release build_all
INCLUDEPATH += src include

