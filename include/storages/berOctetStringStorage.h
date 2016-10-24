/*
 * CBerOctetStringStorage.h
 *
 *  Created on: 3 нояб. 2015 г.
 *      Author: alexey
 */

#ifndef INCLUDE_CBEROCTETSTRINGSTORAGE_H_
#define INCLUDE_CBEROCTETSTRINGSTORAGE_H_

#include <berBase.h>

class ASN1_SHAREDEXPORT CBerOctetStringStorage
{
private:
	QByteArray* ptrValue(QObject* obj, quint32 idx);

public:
	quint32 serialize(CBerByteArrayOutputStream& berOStream, QObject* obj);
	quint32 deserialize(CBerByteArrayInputStream& iStream, QObject* obj, CBerLength& length, quint32 codeLength);
};

#endif /* INCLUDE_CBEROCTETSTRINGSTORAGE_H_ */
