/*
 * qlistStorage.h
 *
 *  Created on: 16 нояб. 2015 г.
 *      Author: alexey
 */

#ifndef INCLUDE_STORAGES_QLISTSTORAGE_H_
#define INCLUDE_STORAGES_QLISTSTORAGE_H_

#include "berBase.h"

template<class Type>
class CContainerStorage
{
public:

	quint32 serialize(CBerByteArrayOutputStream& berOStream, QObject* obj, bool explct)
	{
		quint32 codeLength = 0;

		qint32 idx = obj->metaObject()->propertyCount();

		if (idx == 3)
		{
			if (obj->metaObject()->property(3).isReadable())
			{
				QVariant var = obj->metaObject()->property(3).read(obj);
				qDebug() << var.typeName() << "; " << var.userType() << "; ";
				Type* temp_berobject = var.value<Type*>();

				if (temp_berobject != nullptr)
				{
					for (auto val: *temp_berobject)
						codeLength += val.encode(berOStream, explct);
				}
			}
		}

		return codeLength;
	}

	quint32 deserialize(CBerByteArrayInputStream& iStream, QObject* obj, CBerLength& length, quint32 codeLength, bool explct)
	{
		qint32 idx = obj->metaObject()->propertyCount();

		if (idx == 3)
		{
			if (obj->metaObject()->property(3).isReadable())
			{
				QVariant var = obj->metaObject()->property(3).read(obj);
				qDebug() << var.typeName() << "; " << var.userType() << "; ";
				Type* temp_berobject = var.value<Type*>();

				if (temp_berobject != nullptr)
				{
					for (auto val: *temp_berobject)
						codeLength += val.decode(iStream, explct);
				}
			}
		}

		return codeLength;
	}

};

#endif /* INCLUDE_STORAGES_QLISTSTORAGE_H_ */
