/*
 * berIntegerStorage.cpp
 *
 *  Created on: 3 нояб. 2015 г.
 *      Author: alexey
 */

#include "berIntegerStorage.h"

qint32* CBerIntegerStorage::ptrValue(QObject* obj, quint32 idx)
{
	QVariant var = obj->metaObject()->property(idx).read(obj);
	qDebug() << var.typeName() << "; " << var.userType() << "; ";
	return const_cast<qint32*> (reinterpret_cast<const qint32*> (var.constData()));
}

quint32 CBerIntegerStorage::serialize(CBerByteArrayOutputStream& berOStream, QObject* obj, bool explct)
{
	quint32 codeLength = 1;

	qint32* pInteger = ptrValue(obj, 3);

	while ( ( (*pInteger > (pow(2, 8 * codeLength - 1) - 1)) || (*pInteger < pow(-2, 8 * codeLength - 1)) ) && (codeLength < 8) )
		codeLength++;

	quint64 val = *pInteger;
	for (quint32 i = 0; i < codeLength; ++i)
	{
		berOStream.write( (quint8) (val & 0xFF) );
		val >>= 8;
	}

	codeLength += CBerLength::encodeLength(berOStream, codeLength);

	return codeLength;
}

quint32 CBerIntegerStorage::deserialize(CBerByteArrayInputStream& iStream, QObject* obj, CBerLength& length, quint32 codeLength, bool explct)
{
	qint32 lenval = length.getVal();
	if ( lenval < 1 || lenval > 8 )
	{
		runtimeError("CBerInteger::deserialize: decoded length");
		return codeLength;
	}

	QByteArray data(lenval, Qt::Initialization::Uninitialized);
	if ( iStream.read(data, 0, lenval) < lenval )
	{
		runtimeError("CBerInteger::deserialize: read wrong");
		return codeLength;
	}

	codeLength += lenval;

	qint64 val = 0;

	if ( (data[0] & 0x80) == 0x80 )
	{
		val = -1;
		for (qint32 i = 0; i < lenval; ++i)
		{
			qint32 numShiftBits = 8 * (lenval - i -1);
			val &= (qint64) ( ( ((qint64) (data[i]) & 0xFF) << numShiftBits) | ~( (qint64)(0xFF) << numShiftBits) );
		}
	}
	else
	{
		val = 0;
		for (qint32 i = 0; i < lenval; ++i)
		{
			val |= ( ((qint64)(data[i]) & 0xFF) << (8 * (lenval - i - 1)) );
		}
	}

	QVariant wrvar(val);
	obj->metaObject()->property(3).write(obj, wrvar);

	return codeLength;

}

quint32 CBerIntegerStorage::encode(CBerByteArrayOutputStream& berOStream, QObject* obj, bool explct)
{
	quint32 codeLength = CBerBaseStorage::encode(berOStream, obj, explct);

	return codeLength;
}

quint32 CBerIntegerStorage::decode(CBerByteArrayInputStream& iStream, QObject* obj, bool explct)
{
	int codeLength =  CBerBaseStorage::decode(iStream, obj, explct);

	return codeLength;
}

void CBerIntegerStorage::encodeAndSave(QObject* obj, qint32 encodingSizeGuess)
{
	CBerInteger* pBerInteger = reinterpret_cast<CBerInteger*>(obj);

	CBerByteArrayOutputStream berOStream(encodingSizeGuess);

	pBerInteger->encode(berOStream, obj, false);
	QByteArray Code = berOStream.getByteArray();

	QVariant wrvar(Code);

	obj->metaObject()->property(2).write(obj, wrvar);
}


