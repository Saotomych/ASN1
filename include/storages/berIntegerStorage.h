/*
 * berIntegerStorage.h
 *
 *  Created on: 3 нояб. 2015 г.
 *      Author: alexey
 */

#ifndef INCLUDE_BERINTEGERSTORAGE_H_
#define INCLUDE_BERINTEGERSTORAGE_H_

#include "berBase.h"

class ASN1_SHAREDEXPORT CBerIntegerStorage//: public CBerBaseStorage
{
private:
	qint64* ptrValue(QObject* obj, quint32 idx);

public:
	quint32 serialize(CBerByteArrayOutputStream& berOStream, QObject* obj);
	quint32 deserialize(CBerByteArrayInputStream& iStream, QObject* obj, CBerLength& length, quint32 codeLength);

public:

	void encodeAndSave(QObject* obj, qint32 encodingSizeGuess);
};

#endif /* INCLUDE_BERINTEGERSTORAGE_H_ */
