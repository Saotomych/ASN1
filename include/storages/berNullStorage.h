/*
 * berNullStorage.h
 *
 *  Created on: 3 нояб. 2015 г.
 *      Author: alexey
 */

#ifndef INCLUDE_STORAGES_BERNULLSTORAGE_H_
#define INCLUDE_STORAGES_BERNULLSTORAGE_H_

#include "berBase.h"

class CBerNullStorage: public CBerBaseStorage
{
protected:
	virtual quint32 serialize(CBerByteArrayOutputStream& berOStream, QObject* obj, bool explct);
	virtual quint32 deserialize(CBerByteArrayInputStream& iStream, QObject* obj, CBerLength& length, quint32 codeLength, bool explct);

public:
	virtual quint32 encode(CBerByteArrayOutputStream& berOStream, QObject* obj, bool explct);
	virtual quint32 decode(CBerByteArrayInputStream& iStream, QObject* obj, bool explct);

	virtual void encodeAndSave(QObject* obj, qint32 encodingSizeGuess);
};

#endif /* INCLUDE_STORAGES_BERNULLSTORAGE_H_ */
