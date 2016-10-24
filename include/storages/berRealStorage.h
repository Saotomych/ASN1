/*
 * berRealStorage.h
 *
 *  Created on: 3 нояб. 2015 г.
 *      Author: alexey
 */

#ifndef INCLUDE_STORAGES_BERREALSTORAGE_H_
#define INCLUDE_STORAGES_BERREALSTORAGE_H_

#include <berBase.h>

class CBerRealStorage
{
private:
	double* ptrValue(QObject* obj, quint32 idx);

public:
	quint32 serialize(CBerByteArrayOutputStream& berOStream, QObject* obj);
	quint32 deserialize(CBerByteArrayInputStream& iStream, QObject* obj, CBerLength& length, quint32 codeLength);
	void encodeAndSave(QObject* obj, qint32 encodingSizeGuess);
};


#endif /* INCLUDE_STORAGES_BERREALSTORAGE_H_ */
