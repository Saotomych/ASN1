/*
 * berObjectIdentifierStorage.h
 *
 *  Created on: 3 нояб. 2015 г.
 *      Author: alexey
 */

#ifndef INCLUDE_STORAGES_BEROBJECTIDENTIFIERSTORAGE_H_
#define INCLUDE_STORAGES_BEROBJECTIDENTIFIERSTORAGE_H_

#include <asn1_global.h>
#include <berByteArrayOutputStream.h>
#include <berByteArrayInputStream.h>
#include <berLength.h>

class CBerObjectIdentifierStorage
{
private:
	QVector<qint32>* ptrValue(QObject* obj, quint32 idx);

public:
	quint32 serialize(CBerByteArrayOutputStream& berOStream, QObject* obj);
	quint32 deserialize(CBerByteArrayInputStream& iStream, QObject* obj, CBerLength& length, quint32 codeLength);
	QString toString(QObject* obj);
};

#endif /* INCLUDE_STORAGES_BEROBJECTIDENTIFIERSTORAGE_H_ */
