/*
 * qlistStorage.h
 *
 *  Created on: 16 нояб. 2015 г.
 *      Author: alexey
 */

#ifndef INCLUDE_STORAGES_QLISTSTORAGE_H_
#define INCLUDE_STORAGES_QLISTSTORAGE_H_

#include "berBase.h"

template<class BaseClassType, class ContainerType, class DataType>
class CContainerStorage
{

	WorkType getWorkType(QObject*, QVariant& varpos0, QVariant& varpos1, QVariant& varpos2)
	{
		WorkType type = WorkType::NOT_IDENTIFIED_MODE;

		if (varpos0.canConvert(BaseClassType::s_metaTypeListId))
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
		if (varposprev.canConvert(BaseClassType::s_metaTypeListId))
			return varposprev;
		else
			return obj->metaObject()->property(index).read(obj);
	}

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

						qDebug() << "CContainerStorage::serialize: " << berOStream.getByteArray().toHex();

					}
				}
			}
		}

		codeLength += CBerLength::encodeLength(berOStream, codeLength);
		qDebug() << "CContainerStorage::serialize, length added: " << berOStream.getByteArray().toHex();

		return codeLength;
	}

	quint32 deserialize(CBerByteArrayInputStream& iStream, QObject* obj, CBerLength& length, quint32 codeLength, bool)
	{
		length.decode(iStream);
		qDebug() << "CContainerStorage::deserialize, length extracted: " << length.getVal();

		qint32 size = obj->metaObject()->propertyCount();

		for (qint32 i=START_PROPERTY_INDEX; i < size; ++i)
		{
			// Проверяем режим декодирования
			QVariant varpos2 = obj->metaObject()->property(i).read(obj);
			QVariant varpos1 = getNextVariant(obj, varpos2, i+1);
			QVariant varpos0 = getNextVariant(obj, varpos1, i+2);
			if (varpos1.canConvert(BaseClassType::s_metaTypeListId))	// For not full case
				varpos1 = varpos2;

			WorkType type = getWorkType(obj, varpos0, varpos1, varpos2);
			qDebug() << "CContainerStorage::deserialize: types: " << varpos2.typeName() << "; "
					<< varpos1.typeName() << "; "
					<< varpos0.typeName() << "; ";

			qDebug() << "CContainerStorage::deserialize: type = " << (quint32) type;

			Q_ASSERT_X(type < WorkType::NOT_IDENTIFIED_MODE, "CContainerStorage::deserialize:", "type < NOT_IDENTIFIED_MODE");

			ContainerType* temp_berobject = varpos0.value<ContainerType*>();
			if (temp_berobject != nullptr)
			{
				switch(type)
				{
				case WorkType::ORIGINAL_IDENTIFIER:
					for (DataType& val: *temp_berobject)
						codeLength += val.decode(iStream, true);
					break;

				case WorkType::ORIGINAL_IDENTIFIER_WITH_LENGTH:
					{
						codeLength += length.decode(iStream);

						quint32 subCodeLength = 0;
						for (DataType& val: *temp_berobject)
							subCodeLength += val.decode(iStream, true);

						if (subCodeLength != length.getVal())
						{
							qDebug() << "ERROR! CContainerStorage::deserialize container is wrong for type: " << varpos0.typeName()
									<< "; length original = " << length.getVal()
									<< "; length received = " << subCodeLength;
							throw std::runtime_error("Decode error");
						}

						codeLength += subCodeLength;
					}
					++i;
					break;

				case WorkType::PARENT_IDENTIFIER:
					{
						CBerIdentifier idobjectOriginal = varpos2.value<CBerIdentifier>();
						CBerIdentifier idobjectReceive = varpos1.value<CBerIdentifier>();

						for (DataType& val: *temp_berobject)
						{
							if ( idobjectOriginal.IsExisting() )
							{
								codeLength += idobjectReceive.decode(iStream);

								if ( idobjectOriginal != idobjectReceive )
								{
									qDebug() << "ERROR! CContainerStorage::deserialize error: expected ID = "
											<< idobjectOriginal.toString()
											<< "; received ID = "
											<< idobjectReceive.toString() << ";";

									throw std::runtime_error("Decode error");
								}
							}

							codeLength += val.decode(iStream, false);
						}

					}
					++i;
					break;

				case WorkType::PARENT_IDENTIFIER_WITH_LENGTH:
					{
						CBerIdentifier idobjectOriginal = varpos2.value<CBerIdentifier>();
						CBerIdentifier idobjectReceive = varpos1.value<CBerIdentifier>();

						quint32 subCodeLength = 0;

						for (DataType& val: *temp_berobject)
						{
							if ( idobjectOriginal.IsExisting() )
							{
								codeLength += idobjectReceive.decode(iStream);

								if ( idobjectOriginal != idobjectReceive )
								{
									qDebug() << "ERROR! CContainerStorage::deserialize error: expected ID = "
											<< idobjectOriginal.toString()
											<< "; received ID = "
											<< idobjectReceive.toString() << ";";

									throw std::runtime_error("Decode error");
								}
							}

							codeLength += length.decode(iStream);

							subCodeLength += val.decode(iStream, true);
						}

						if (subCodeLength != length.getVal())
						{
							qDebug() << "ERROR! CContainerStorage::deserialize: container is wrong for type: " << varpos0.typeName()
									<< "; length original = " << length.getVal()
									<< "; length received = " << subCodeLength;
							throw std::runtime_error("Decode error");
						}

						codeLength += subCodeLength;
					}
					i+=2;
					break;

				default:
					QString str = QString("ERROR! CContainerStorage::deserialize type %1 isn't identified")
							.arg( (quint32) type);
					Q_ASSERT_X(false, "CContainerStorage::deserialize", str.toStdString().c_str());
					break;
				}

				QByteArray out = iStream.get();
				qDebug() << "CContainerStorage::deserialize: data[" << i << "]: " << out.toHex();
			}
			else
			{
				qDebug() << "CContainerStorage::deserialize: nullptr found";
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

#endif /* INCLUDE_STORAGES_QLISTSTORAGE_H_ */
