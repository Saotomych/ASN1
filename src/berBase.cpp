#include "berBase.h"

WorkType CBerBaseStorage::getWorkType(QObject* obj, QVariant& varpos0, QVariant& varpos1, QVariant& varpos2)
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

QVariant CBerBaseStorage::getNextVariant(QObject* obj, QVariant& varposprev, quint32 index)
{
	if (varposprev.canConvert(IBerBaseType::s_metaTypeId))
		return varposprev;
	else
		return obj->metaObject()->property(index).read(obj);
}

quint32 CBerBaseStorage::serialize(CBerByteArrayOutputStream& berOStream, QObject* obj, bool explct)
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
						if (idobject.IsExisting() || idobject.IsMandatory())
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
						if (idobject.IsExisting() || idobject.IsMandatory())
							codeLength += idobject.encode(berOStream);
					}
					i-=2;
					break;

				default:
					QString str = QString("ERROR! serialize type %1 isn't identified")
							.arg( (quint32) type);
					Q_ASSERT_X(false, "CBerBaseStorage::serialize", str.toStdString().c_str());
					break;
				}

				qDebug() << "Base Encoder: " << berOStream.getByteArray().toHex();

			}

		}

	}

	codeLength += CBerLength::encodeLength(berOStream, codeLength);
	qDebug() << "Base Encoder, length added: " << berOStream.getByteArray().toHex();

	return codeLength;
}

quint32 CBerBaseStorage::deserialize(CBerByteArrayInputStream& iStream, QObject* obj, CBerLength& length, quint32 codeLength, bool explct)
{

	length.decode(iStream);
	qDebug() << "Base deserialize, length extracted: " << length.getVal();

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
		qDebug() << "CBerBaseStorage::deserialize: types: " << varpos2.typeName() << "; "
				<< varpos1.typeName() << "; "
				<< varpos0.typeName() << "; ";

		qDebug() << "CBerBaseStorage::deserialize: type = " << (quint32) type;

		Q_ASSERT_X(type < WorkType::NOT_IDENTIFIED_MODE, "CBerBaseStorage::deserialize:", "type < NOT_IDENTIFIED_MODE");

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

					if ( idobjectOriginal.IsExisting() )
					{
						codeLength += idobjectReceive.decode(iStream);

						if ( idobjectOriginal != idobjectReceive )
						{
							qDebug() << "ERROR! Decode error: expected ID = "
									<< idobjectOriginal.toString()
									<< "; received ID = "
									<< idobjectReceive.toString() << ";";

							throw std::runtime_error("Decode error");
						}
					}

					codeLength += temp_berobject->decode(iStream, false);
				}
				++i;
				break;

			case WorkType::PARENT_IDENTIFIER_WITH_LENGTH:
				{
					CBerIdentifier idobjectOriginal = varpos2.value<CBerIdentifier>();
					CBerIdentifier idobjectReceive = varpos1.value<CBerIdentifier>();

					if ( idobjectOriginal.IsExisting() )
					{
						codeLength += idobjectReceive.decode(iStream);

						if ( idobjectOriginal != idobjectReceive )
						{
							qDebug() << "ERROR! Decode error: expected ID = "
									<< idobjectOriginal.toString()
									<< "; received ID = "
									<< idobjectReceive.toString() << ";";

							throw std::runtime_error("Decode error");
						}
					}

					quint32 subCodeLength = length.decode(iStream);
					subCodeLength += temp_berobject->decode(iStream, false);
					codeLength += subCodeLength;
				}
				i+=2;
				break;

			case WorkType::NOT_IDENTIFIED_MODE:
			default:
				QString str = QString("ERROR! Deserialize type %1 isn't identified")
						.arg( (quint32) type);
				Q_ASSERT_X(false, "CBerBaseStorage::deserialize", str.toStdString().c_str());
				break;
			}

			QByteArray out = iStream.get();
			qDebug() << "CBerBaseStorage::deserialize: data[" << i << "]: " << out.toHex();
		}
		else
		{
			qDebug() << "CBerBaseStorage::deserialize: nullptr found";
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

void CBerBaseStorage::runtimeError(QString strErr)
{
	qDebug() << strErr;
#ifdef DEBUG
	throw std::runtime_error(strErr.toStdString());
#endif
}

void CBerBaseStorage::argumentWrong(QString strErr)
{
	qDebug() << strErr;

#ifdef DEBUG
	throw std::invalid_argument(strErr.toStdString());
#endif
}
