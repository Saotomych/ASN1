/*
 * DeclareCommonTypes.h
 *
 *  Created on: 16 июня 2016 г.
 *      Author: alex
 */

#ifndef DECLARECOMMONTYPES_H_
#define DECLARECOMMONTYPES_H_

#include <QByteArray>
#include <QBitArray>
#include <QVector>

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


#endif /* DECLARECOMMONTYPES_H_ */
