/*
 * berNullStorage.h
 *
 *  Created on: 3 нояб. 2015 г.
 *      Author: alexey
 */

#ifndef INCLUDE_STORAGES_BERNULLSTORAGE_H_
#define INCLUDE_STORAGES_BERNULLSTORAGE_H_

#include <berBase.h>

class CBerNullStorage
{
public:
	quint32 serialize(CBerByteArrayOutputStream& berOStream, QObject* obj);
	quint32 deserialize(CBerByteArrayInputStream& iStream, QObject* obj, CBerLength& length, quint32 codeLength);

};

#endif /* INCLUDE_STORAGES_BERNULLSTORAGE_H_ */
