/*
 * berNullStorage.cpp
 *
 *  Created on: 3 нояб. 2015 г.
 *      Author: alexey
 */

#include <storages/berNullStorage.h>

quint32 CBerNullStorage::serialize(CBerByteArrayOutputStream& berOStream, QObject* obj)
{
	quint32 codeLength = CBerLength::encodeLength(berOStream, 0);

	return codeLength;
}

quint32 CBerNullStorage::deserialize(CBerByteArrayInputStream& iStream, QObject* obj, CBerLength& length, quint32 codeLength)
{
	length.decode(iStream);
	qDebug() << "CBerNullStorage::deserialize, length extracted: " << length.getVal();

	return codeLength;
}

