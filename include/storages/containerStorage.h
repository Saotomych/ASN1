/*
 * qlistStorage.h
 *
 *  Created on: 16 нояб. 2015 г.
 *      Author: alexey
 */

#ifndef INCLUDE_STORAGES_QLISTSTORAGE_H_
#define INCLUDE_STORAGES_QLISTSTORAGE_H_

#include "berBase.h"

template<class ContainerType, class DataType>
class CContainerStorage
{
public:

	quint32 serialize(CBerByteArrayOutputStream& berOStream, QObject* obj, bool)
	{
		quint32 codeLength = 0;
		qint32 size = obj->metaObject()->propertyCount()-1;

		for (qint32 i=size; i >= 3; --i)
		{
			if (obj->metaObject()->property(i).isReadable())
			{
				QVariant var = obj->metaObject()->property(i).read(obj);
				qDebug() << var.typeName() << "; " << var.userType() << "; ";

				ContainerType* temp_berobject = var.value<ContainerType*>();
				if (temp_berobject != nullptr)
				{
					if (obj->metaObject()->property(i-1).isReadable())
					{
						QVariant idvar = obj->metaObject()->property(i-1).read(obj);
						if (idvar.canConvert(CBerIdentifier::s_metaTypeId))
						{
							for (DataType& val: *temp_berobject)
							{
								codeLength += val.encode(berOStream, false);
								CBerIdentifier idobject = idvar.value<CBerIdentifier>();
								codeLength += idobject.encode(berOStream);
							}
						}
						else
						{
							for (DataType& val: *temp_berobject)
								codeLength += val.encode(berOStream, true);
						}

						qDebug() << "Container Encoder: " << berOStream.getByteArray().toHex();

					}
				}
			}
		}

		codeLength += CBerLength::encodeLength(berOStream, codeLength);
		qDebug() << "Container Encoder, length added: " << berOStream.getByteArray().toHex();

		return codeLength;
	}

	quint32 deserialize(CBerByteArrayInputStream& iStream, QObject* obj, CBerLength&, quint32 codeLength, bool explct)
	{
		qint32 idx = obj->metaObject()->propertyCount();

		if (idx == 3)
		{
			if (obj->metaObject()->property(3).isReadable())
			{
				QVariant var = obj->metaObject()->property(3).read(obj);
				qDebug() << var.typeName() << "; " << var.userType() << "; ";
				ContainerType* temp_berobject = var.value<ContainerType*>();

				if (temp_berobject != nullptr)
				{
					for (DataType val: *temp_berobject)
						codeLength += val.decode(iStream, explct);
				}
			}
		}

		return codeLength;
	}

};

#endif /* INCLUDE_STORAGES_QLISTSTORAGE_H_ */
