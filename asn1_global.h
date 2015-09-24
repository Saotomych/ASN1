#ifndef ASN1_GLOBAL_H
#define ASN1_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QObject>
#include <QHostAddress>
#include <QMutex>
#include <QDebug>
#include <QLinkedList>
#include <QVector>
#include <QDebug>
#include <QDataStream>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QByteArray>

#include <exception>

#if defined(ASN1_LIBRARY)
#  define ASN1_EXPORT Q_DECL_EXPORT
#else
#  define ASN1_EXPORT Q_DECL_IMPORT
#endif

#endif // OSITRANSPORT_GLOBAL_H
