/*
 * berBooleanStorage.h
 *
 *  Created on: 3 нояб. 2015 г.
 *      Author: alexey
 */

#ifndef INCLUDE_STORAGES_BERBOOLEANSTORAGE_H_
#define INCLUDE_STORAGES_BERBOOLEANSTORAGE_H_

#include "asn1_global.h"
#include "berByteArrayOutputStream.h"
#include "berByteArrayInputStream.h"
#include "berLength.h"

class CBerBooleanStorage
{
private:
	bool* ptrValue(QObject* obj, quint32 idx);

public:
	quint32 serialize(CBerByteArrayOutputStream& berOStream, QObject* obj);
	quint32 deserialize(CBerByteArrayInputStream& iStream, QObject* obj, CBerLength& length, quint32 codeLength);
	void encodeAndSave(QObject* obj, qint32 encodingSizeGuess);
};



#endif /* INCLUDE_STORAGES_BERBOOLEANSTORAGE_H_ */
