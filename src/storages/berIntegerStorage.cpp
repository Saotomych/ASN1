/*
 * berIntegerStorage.cpp
 *
 *  Created on: 3 нояб. 2015 г.
 *      Author: alexey
 */

#include "storages/berIntegerStorage.h"
#include "berInteger.h"

qint64* CBerIntegerStorage::ptrValue(QObject* obj, quint32 idx)
{
	QVariant var = obj->metaObject()->property(idx).read(obj);
	qDebug() << var.typeName() << "; " << var.userType() << "; ";
	return qvariant_cast<qint64*> (var);
}

quint32 CBerIntegerStorage::serialize(CBerByteArrayOutputStream& berOStream, QObject* obj, bool explct)
{
	quint32 codeLength = 1;

	qint64 Integer = *ptrValue(obj, 3);

	while ( ( (Integer > (pow(2, 8 * codeLength - 1) - 1)) || (Integer < pow(-2, 8 * codeLength - 1)) ) && (codeLength < 8) )
		codeLength++;

	qint64 val = Integer;
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
	length.decode(iStream);
	qDebug() << "CBerIntegerStorage::deserialize, length extracted: " << length.getVal();

	qint32 lenval = length.getVal();

	if ( lenval < 1 || lenval > 8 )
	{
//		runtimeError("CBerInteger::deserialize: decoded length");
		return codeLength;
	}

	QByteArray data(lenval, Qt::Initialization::Uninitialized);
	if ( iStream.read(data, 0, lenval) < lenval )
	{
//		runtimeError("CBerInteger::deserialize: read wrong");
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

	qint64* pVal = &val;
	QVariant wrvar(PtrMetaTypes::s_qint64PtrMetaType, &pVal);
	obj->metaObject()->property(3).write(obj, wrvar);

	return codeLength;

}

void CBerIntegerStorage::encodeAndSave(QObject* obj, qint32 encodingSizeGuess)
{
	CBerInteger* pBerInteger = reinterpret_cast<CBerInteger*>(obj);

	CBerByteArrayOutputStream berOStream(encodingSizeGuess);

	pBerInteger->encode(berOStream, false);
	QByteArray Code = berOStream.getByteArray();

	QVariant wrvar(Code);

	obj->metaObject()->property(2).write(obj, wrvar);
}


