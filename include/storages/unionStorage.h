/*
 * unionStorage.h
 *
 *  Created on: 22 авг. 2016 г.
 *      Author: alex
 */

#ifndef INCLUDE_STORAGES_UNIONSTORAGE_H_
#define INCLUDE_STORAGES_UNIONSTORAGE_H_

#include "berBase.h"

class CUnionStorage
{

	WorkType getWorkType(QObject*, QVariant& varpos0, QVariant& varpos1, QVariant& varpos2)
	{
		WorkType type = WorkType::NOT_IDENTIFIED_MODE;

		if (varpos0.canConvert(IBerBaseType::s_metaTypeId))
		{
			if (varpos1.canConvert(CBerIdentifier::s_metaTypeId))
			{
				type = WorkType::PARENT_IDENTIFIER;
			}
			else
			{
				if (varpos1.canConvert(CBerLength::s_metaTypeId) )
				{
					if (varpos2.canConvert(CBerIdentifier::s_metaTypeId) )
						type = WorkType::PARENT_IDENTIFIER_WITH_LENGTH;
				}
			}
		}

		return type;
	}

	QVariant getNextVariant(QObject* obj, QVariant& varposprev, quint32 index)
	{
		if (varposprev.canConvert(IBerBaseType::s_metaTypeId))
			return varposprev;
		else
			return obj->metaObject()->property(index).read(obj);
	}

public:

	quint32 serialize(CBerByteArrayOutputStream& berOStream, QObject* obj, bool)
	{
		quint32 codeLength = 0;
		qint32 size = obj->metaObject()->propertyCount()-1;

		for (qint32 i=size; i >= START_PROPERTY_INDEX; --i)
		{
			if (obj->metaObject()->property(i).isReadable())
			{
				QVariant varpos0 = obj->metaObject()->property(i).read(obj);
				QVariant varpos1 = obj->metaObject()->property(i-1).read(obj);
				QVariant varpos2 = obj->metaObject()->property(i-2).read(obj);

				auto type = getWorkType(obj, varpos0, varpos1, varpos2);

				IBerBaseType* temp_berobject = varpos0.value<IBerBaseType*>();
				if (temp_berobject != nullptr)
				{
					switch(type)
					{
					case WorkType::PARENT_IDENTIFIER:
						{
							codeLength += temp_berobject->encode(berOStream, false);
							CBerIdentifier idobject = varpos1.value<CBerIdentifier>();
							if (idobject.IsExisting())
								codeLength += idobject.encode(berOStream);
						}
						--i;
						break;

					case WorkType::PARENT_IDENTIFIER_WITH_LENGTH:
						{
							quint32 subCodeLength = temp_berobject->encode(berOStream, false);
							subCodeLength += CBerLength::encodeLength(berOStream, subCodeLength);
							codeLength += subCodeLength;

							CBerIdentifier idobject = varpos2.value<CBerIdentifier>();
							if (idobject.IsExisting())
								codeLength += idobject.encode(berOStream);
						}
						i-=2;
						break;

					default:
						QString str = QString("ERROR! serialize type %1 isn't identified")
								.arg( (quint32) type);
						Q_ASSERT_X(false, "CUnionStorage::serialize: ", str.toStdString().c_str());
						break;
					}

					qDebug() << "CUnionStorage: " << berOStream.getByteArray().toHex();

					return codeLength;
				}

			}

		}

		return codeLength;
	}

	quint32 deserialize(CBerByteArrayInputStream& iStream, QObject* obj, CBerLength& length, quint32 codeLength, bool)
	{
		qint32 size = obj->metaObject()->propertyCount();

		CBerIdentifier idobjectReceive;

		for (qint32 i=START_PROPERTY_INDEX; i < size; ++i)
		{
			idobjectReceive = lastBerIdentifier::get(iStream, codeLength);

			qDebug() << "Union deserialize, idobjectReceive =  " << idobjectReceive.toString()
					<< "; as code = " << idobjectReceive.getCode()->toHex();

			// Проверяем режим декодирования
			QVariant varpos2 = obj->metaObject()->property(i).read(obj);
			QVariant varpos1 = getNextVariant(obj, varpos2, i+1);
			QVariant varpos0 = getNextVariant(obj, varpos1, i+2);
			if (varpos1.canConvert(IBerBaseType::s_metaTypeId))	// For not full case
				varpos1 = varpos2;

			auto type = getWorkType(obj, varpos0, varpos1, varpos2);
			qDebug() << "CUnionStorage::deserialize: types: " << varpos2.typeName() << "; "
					<< varpos1.typeName() << "; "
					<< varpos0.typeName() << "; ";

			qDebug() << "CUnionStorage::deserialize: type = " << (quint32) type;

			Q_ASSERT_X(type < WorkType::NOT_IDENTIFIED_MODE, "CUnionStorage::deserialize:", "type < NOT_IDENTIFIED_MODE");

			CBerIdentifier idobjectOriginal = varpos2.value<CBerIdentifier>();
			qDebug() << "CUnionStorage::deserialize, try for idobjectOriginal =  " << idobjectOriginal.toString()
					<< "; as code = " << idobjectOriginal.getCode()->toHex();

			switch(type)
			{
			case WorkType::PARENT_IDENTIFIER:
				{
					if ( idobjectOriginal == idobjectReceive )
					{
						qDebug() << "Union deserialize, idobjectOriginal = idobjectReceive with identifier only";

						IBerBaseType* temp_berobject = (dynamic_cast<IBerBaseType*>(obj))->createMember(idobjectReceive);
						if (temp_berobject != nullptr)
						{
							lastBerIdentifier::reset();
							codeLength += temp_berobject->decode(iStream, false);
						}
						else
						{
							std::runtime_error("Union deserialize. Received object can't create.");
						}

						QByteArray out = iStream.get();
						qDebug() << "CUnionStorage::deserialize: data[" << i << "]: " << out.toHex();

					}
					else
					{
						qDebug() << "CUnionStorage::deserialize: data[" << i << "]: skipped";
					}
				}
				++i;
				break;

			case WorkType::PARENT_IDENTIFIER_WITH_LENGTH:
				{
					if ( idobjectOriginal == idobjectReceive )
					{
						qDebug() << "Union deserialize, idobjectOriginal = idobjectReceive with identifier and length";

						IBerBaseType* temp_berobject = (dynamic_cast<IBerBaseType*>(obj))->createMember(idobjectReceive);
						if (temp_berobject != nullptr)
						{
							lastBerIdentifier::reset();
							quint32 subCodeLength = length.decode(iStream);
							subCodeLength += temp_berobject->decode(iStream, false);
							codeLength += subCodeLength;
						}
						else
						{
							std::runtime_error("Union deserialize. Received object can't create with Union Storage deserialize.");
						}

						QByteArray out = iStream.get();
						qDebug() << "CUnionStorage::deserialize: data[" << i << "]: " << out.toHex();

					}
					else
					{
						qDebug() << "CUnionStorage::deserialize: data[" << i << "]: skipped";
					}
				}
				i+=2;
				break;

			case WorkType::NOT_IDENTIFIED_MODE:
			default:
				QString str = QString("ERROR! CUnionStorage::deserialize type %1 isn't identified")
						.arg( (quint32) type);
				Q_ASSERT_X(false, "CUnionStorage::deserialize", str.toStdString().c_str());
				break;
			}
		}

		return codeLength;
	}

};



#endif /* INCLUDE_STORAGES_UNIONSTORAGE_H_ */
