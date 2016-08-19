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

	quint32 deserialize(CBerByteArrayInputStream& iStream, QObject* obj, CBerLength& length, quint32 codeLength, bool)
	{
		length.decode(iStream);
		qDebug() << "CContainerStorage::deserialize, length extracted: " << length.getVal();

		qint32 size = obj->metaObject()->propertyCount();

		for (qint32 i=3; i < size; ++i)
		{
			if (obj->metaObject()->property(i).isReadable())
			{
				QVariant idvar = obj->metaObject()->property(i).read(obj);

				if (idvar.canConvert(CBerIdentifier::s_metaTypeId))
				{
					// 1 & 2 variants with dedicated ID
					CBerIdentifier idobjectorig = idvar.value<CBerIdentifier>();

					if (++i == size)
					{
						qDebug() << "ERROR! Decode error: not found ContainerType for id "
								<< idobjectorig.toString();
						break;
					}

					QVariant var = obj->metaObject()->property(i).read(obj);

					qDebug() << var.typeName() << "; " << var.userType() << "; ";

					ContainerType* temp_berobject = var.value<ContainerType*>();
					if (temp_berobject != nullptr)
					{
						if (obj->metaObject()->property(i).isReadable())
						{
							if (idvar.canConvert(CBerIdentifier::s_metaTypeId))
							{
								CBerIdentifier idobject = idvar.value<CBerIdentifier>();
								for (DataType& val: *temp_berobject)
								{
									codeLength += idobject.decode(iStream);
									codeLength += val.decode(iStream, false);
								}

								QByteArray out = iStream.get();
								qDebug() << "ContainerStorage::deserialize ID+data[" << i << "]: " << out.toHex();
							}
							else
							{
								for (DataType& val: *temp_berobject)
									codeLength += val.decode(iStream, true);

								QByteArray out = iStream.get();
								qDebug() << "ContainerStorage::deserialize data[" << i << "]: " << out.toHex();
							}
						}
					}
					else
					{
						qDebug() << "Decode ContainerType: ERROR! Decode error: can't convert to ContainerType or nullptr found";
					}
				}
				else
				{
					// 3 & 4 variants with
					QVariant var = obj->metaObject()->property(i).read(obj);
					qDebug() << var.typeName() << "; " << var.userType() << "; ";

					// 3
					ContainerType* temp_berobject = var.value<ContainerType*>();
					if (temp_berobject != nullptr)
					{
						for (DataType& val: *temp_berobject)
							codeLength += val.decode(iStream, true);

						QByteArray out = iStream.get();
						qDebug() << "ContainerStorage::deserialize data[" << i << "]: " << out.toHex();
					}
					else
					{
						qDebug() << "Decode ContainerType: ERROR! Decode error: can't convert to ContainerType or nullptr found";
					}
				}
			}
		}

		return codeLength;
	}

};

#endif /* INCLUDE_STORAGES_QLISTSTORAGE_H_ */
