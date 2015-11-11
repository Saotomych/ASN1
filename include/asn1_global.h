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
#include <QScopedPointer>
#include <QSharedPointer>
#include <QByteArray>
#include <QBitArray>
#include <QtCore/qmath.h>
#include <QMetaObject>
#include <QMetaProperty>
#include <QVariant>

#include <exception>
#include <stdexcept>
#include <limits>

#if defined(ASN1_LIBRARY)
#  define ASN1_SHAREDEXPORT Q_DECL_EXPORT
#else
#  define ASN1_SHAREDEXPORT Q_DECL_IMPORT
#endif

Q_DECLARE_METATYPE(QByteArray*)
Q_DECLARE_METATYPE(qint64*)
Q_DECLARE_METATYPE(QVector<qint32>*)
Q_DECLARE_METATYPE(double*)
Q_DECLARE_METATYPE(QBitArray*)
Q_DECLARE_METATYPE(bool*)

namespace PtrMetaTypes
{
	static quint32 s_QByteArrayPtrMetaType  =  qRegisterMetaType<QByteArray*>();
	static quint32 s_qint64PtrMetaType = qRegisterMetaType<qint64*>();
	static quint32 s_QVectorQint32PtrMetaType = qRegisterMetaType<QVector<qint32>*>();
	static quint32 s_doublePtrMetaType = qRegisterMetaType<double*>();
	static quint32 s_QBitArrayPtrMetaType = qRegisterMetaType<QBitArray*>();
	static quint32 s_boolPtrMetaType = qRegisterMetaType<bool*>();
}


#endif // OSITRANSPORT_GLOBAL_H
