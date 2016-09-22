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
	return qvariant_cast<QBitArray*> (var);
}

quint32 CBerBitStringStorage::serialize(CBerByteArrayOutputStream& berOStream, QObject* obj, bool explct)
{
	QBitArray BA = *ptrValue(obj, 3);

	quint32 codeLength = 0;

	for (qint32 i = BA.size()-1; i >= 0; --i)
	{
		quint8 data = (BA[i] == false) ? 0 : 0xFF;
		berOStream.write(data);
	}
	berOStream.write( (quint8) (BA.size()) );

	codeLength = BA.size() + 1;

	CBerLength::encodeLength(berOStream, codeLength);

	return codeLength;
}

quint32 CBerBitStringStorage::deserialize(CBerByteArrayInputStream& iStream, QObject* obj, CBerLength& length, quint32 codeLength)
{
	length.decode(iStream);
	qDebug() << "CBerBitStringStorage::deserialize, length extracted: " << length.getVal();

	qint32 lenVal = length.getVal();

	QByteArray data(lenVal, Qt::Initialization::Uninitialized);

	if (data.size() > 0)
	{
		QBitArray val;

		qint32 rdLength = iStream.read(data, 0, lenVal);
		if (rdLength == lenVal)
		{
			val.resize(rdLength);
			for (qint32 i=0; i<rdLength; ++i)
			{
				if (data[i]) val.setBit(i);
				else val.clearBit(i);
			}

			codeLength += rdLength + 1;

			QBitArray* pVal = &val;
			QVariant wrvar(PtrMetaTypes::s_QBitArrayPtrMetaType, &pVal);
			obj->metaObject()->property(3).write(obj, wrvar);
		}
	}

	return codeLength;
}
