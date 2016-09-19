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
					else
						type = WorkType::ORIGINAL_IDENTIFIER_WITH_LENGTH;
				}
				else
				{
					type = WorkType::ORIGINAL_IDENTIFIER;
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
					case WorkType::ORIGINAL_IDENTIFIER:
						codeLength += temp_berobject->encode(berOStream, true);
						break;

					case WorkType::ORIGINAL_IDENTIFIER_WITH_LENGTH:
						{
							quint32 subCodeLength = temp_berobject->encode(berOStream, true);
							codeLength += CBerLength::encodeLength(berOStream, subCodeLength);
							codeLength += subCodeLength;
						}
						--i;
						break;

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

		for (qint32 i=START_PROPERTY_INDEX; i < size; ++i)
		{
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

			IBerBaseType* temp_berobject = varpos0.value<IBerBaseType*>();
			if (temp_berobject != nullptr)
			{
				switch(type)
				{
				case WorkType::ORIGINAL_IDENTIFIER:
					codeLength += temp_berobject->decode(iStream, true);
					break;

				case WorkType::ORIGINAL_IDENTIFIER_WITH_LENGTH:
					{
						codeLength += length.decode(iStream);
						quint32 subCodeLength = temp_berobject->decode(iStream, true);
						codeLength += subCodeLength;
					}
					++i;
					break;

				case WorkType::PARENT_IDENTIFIER:
					{
						CBerIdentifier idobjectOriginal = varpos2.value<CBerIdentifier>();
						CBerIdentifier idobjectReceive = varpos1.value<CBerIdentifier>();

						codeLength += idobjectReceive.decode(iStream);

						if ( idobjectOriginal != idobjectReceive )
						{
							qDebug() << "ERROR! CUnionStorage::deserialize error: expected ID = "
									<< idobjectOriginal.toString()
									<< "; received ID = "
									<< idobjectReceive.toString() << ";";

							throw std::runtime_error("Decode error");
						}

						codeLength += temp_berobject->decode(iStream, false);
					}
					++i;
					break;

				case WorkType::PARENT_IDENTIFIER_WITH_LENGTH:
					{
						CBerIdentifier idobjectOriginal = varpos2.value<CBerIdentifier>();
						CBerIdentifier idobjectReceive = varpos1.value<CBerIdentifier>();

						codeLength += idobjectReceive.decode(iStream);

						if ( idobjectOriginal != idobjectReceive )
						{
							qDebug() << "ERROR! CUnionStorage::deserialize error: expected ID = "
									<< idobjectOriginal.toString()
									<< "; received ID = "
									<< idobjectReceive.toString() << ";";

							throw std::runtime_error("Decode error");
						}

						quint32 subCodeLength = length.decode(iStream);
						subCodeLength += temp_berobject->decode(iStream, false);
						codeLength += subCodeLength;
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

				QByteArray out = iStream.get();
				qDebug() << "CUnionStorage::deserialize: data[" << i << "]: " << out.toHex();

				return codeLength;
			}
			else
			{
				qDebug() << "CUnionStorage::deserialize: nullptr found";
				switch(type)
				{
				case WorkType::ORIGINAL_IDENTIFIER_WITH_LENGTH:
				case WorkType::PARENT_IDENTIFIER:
					++i;
					break;

				case WorkType::PARENT_IDENTIFIER_WITH_LENGTH:
					i+=2;
					break;

				case WorkType::ORIGINAL_IDENTIFIER:
				case WorkType::NOT_IDENTIFIED_MODE:
				default:
					break;
				}
			}
		}

		return codeLength;
	}

};



#endif /* INCLUDE_STORAGES_UNIONSTORAGE_H_ */
