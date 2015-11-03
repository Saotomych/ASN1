/*
 * berBitArrayStorage.cpp
 *
 *  Created on: 3 нояб. 2015 г.
 *      Author: alexey
 */

#include "storages/berBitStringStorage.h"

QBitArray* CBerBitStringStorage::ptrValue(QObject* obj, quint32 idx)
{
	QVariant var = obj->metaObject()->property(idx).read(obj);
	qDebug() << var.typeName() << "; " << var.userType() << "; ";
	return const_cast<QBitArray*> (reinterpret_cast<const QBitArray*> (var.constData()));
}

quint32 CBerBitStringStorage::serialize(CBerByteArrayOutputStream& berOStream, QObject* obj, bool explct)
{
	QBitArray* pBA = ptrValue(obj, 3);

	quint32 codeLength = 0;

	for (qint32 i = pBA->size()-1; i >= 0; --i)
	{
		quint8 data = (pBA[i] == false) ? 0 : 0xFF;
		berOStream.write(data);
	}
	berOStream.write( (quint8) (pBA->size()) );

	codeLength = pBA->size() + 1;

	return codeLength;
}

quint32 CBerBitStringStorage::deserialize(CBerByteArrayInputStream& iStream, QObject* obj, CBerLength& length, quint32 codeLength, bool explct)
{
	QByteArray data(length.getVal(), Qt::Initialization::Uninitialized);
	if (data.size() > 0)
	{
		QBitArray val;

		quint32 rdLength = iStream.read(data, 0, (qint32) (length.getVal()) );
		if (rdLength == length.getVal())
		{
			val.resize(rdLength);
			for (quint32 i=0; i<rdLength; ++i)
			{
				if (data[i]) val.setBit(i);
				else val.clearBit(i);
			}

			codeLength += rdLength + 1;

			QVariant wrvar(val);
			obj->metaObject()->property(3).write(obj, wrvar);
		}
		else
		{
			runtimeError("CBerBitStringStorage::deserialize: error decoding");
		}
	}

	return codeLength;
}

quint32 CBerBitStringStorage::encode(CBerByteArrayOutputStream& berOStream, QObject* obj, bool explct)
{
	quint32 codeLength = CBerBaseStorage::encode(berOStream, obj, explct);

	return codeLength;
}

quint32 CBerBitStringStorage::decode(CBerByteArrayInputStream& iStream, QObject* obj, bool explct)
{
	int codeLength =  CBerBaseStorage::decode(iStream, obj, explct);

	return codeLength;
}
