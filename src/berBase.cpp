#include "berBase.h"

QVariant CBerBaseStorage::getVariantAndCheck(QObject* obj, qint32 index)
{
	if (index >= START_PROPERTY_INDEX)
	{
		const QVariant& var = obj->metaObject()->property(index).read(obj);
		qDebug() << var.typeName() << "; " << var.userType() << "; ";
		return obj->metaObject()->property(index).read(obj);
	}
	else
	{
		QVariant q;
		qDebug() << "Overindex. TypeName is empty; UserType is empty; ";
		return q;
	}
}

quint32 CBerBaseStorage::serialize(CBerByteArrayOutputStream& berOStream, QObject* obj, bool explct)
{
	quint32 codeLength = 0;
	qint32 size = obj->metaObject()->propertyCount()-1;

	for (qint32 i=size; i >= START_PROPERTY_INDEX; --i)
	{
		if (obj->metaObject()->property(i).isReadable())
		{
			QVariant varpos0 = obj->metaObject()->property(i).read(obj); //getVariantAndCheck(obj, i);
			QVariant varpos1 = obj->metaObject()->property(i-1).read(obj); //getVariantAndCheck(obj, i-1);
			QVariant varpos2 = obj->metaObject()->property(i-2).read(obj); //getVariantAndCheck(obj, i-2);

			quint32 type = NOT_IDENTIFIED_MODE;

			if (varpos0.canConvert(IBerBaseType::s_metaTypeId))
			{
				if (varpos1.canConvert(CBerIdentifier::s_metaTypeId))
				{
					type = PARENT_IDENTIFIER;
				}
				else
				{
					if (varpos1.canConvert(CBerLength::s_metaTypeId) )
					{
						if (varpos2.canConvert(CBerIdentifier::s_metaTypeId) )
							type = PARENT_IDENTIFIER_WITH_LENGTH;
						else
							type = ORIGINAL_IDENTIFIER_WITH_LENGTH;
					}
					else
					{
						type = ORIGINAL_IDENTIFIER;
					}
				}
			}

			IBerBaseType* temp_berobject = varpos0.value<IBerBaseType*>();
			if (temp_berobject != nullptr)
			{
				switch(type)
				{
				case ORIGINAL_IDENTIFIER:
					codeLength += temp_berobject->encode(berOStream, true);
					break;

				case ORIGINAL_IDENTIFIER_WITH_LENGTH:
					{
						quint32 subCodeLength = temp_berobject->encode(berOStream, true);
						codeLength += CBerLength::encodeLength(berOStream, subCodeLength);
						codeLength += subCodeLength;
					}
					--i;
					break;

				case PARENT_IDENTIFIER:
					{
						codeLength += temp_berobject->encode(berOStream, false);
						CBerIdentifier idobject = varpos1.value<CBerIdentifier>();
						if (idobject.IsExisting() || idobject.IsMandatory())
							codeLength += idobject.encode(berOStream);
					}
					--i;
					break;

				case PARENT_IDENTIFIER_WITH_LENGTH:
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
					QString str = QString("ERROR! Deserialize type %1 isn't identified")
							.arg(type);
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
		// 1. есть id, объект = nullptr - skip 2
		// 2. есть id, есть объект - decode(.., false)
		// 3. нет id, объект = nullptr - skip 1
		// 4. нет id, есть объект - decode(.., true)

		QVariant id1 = obj->metaObject()->property(i).read(obj);
		if (id1.canConvert(CBerIdentifier::s_metaTypeId))
		{
			// 1 & 2
			CBerIdentifier idobjectorig = id1.value<CBerIdentifier>();

			if (++i == size)
			{
				qDebug() << "ERROR! Decode error: not found IBerBaseType for id "
						<< idobjectorig.toString();
				break;
			}

			QVariant var = obj->metaObject()->property(i).read(obj);
			qDebug() << var.typeName() << "; " << var.userType() << "; ";
			if (var.canConvert(IBerBaseType::s_metaTypeId))
			{
				IBerBaseType* temp_berobject = var.value<IBerBaseType*>();
				if (temp_berobject != nullptr)
				{
					// 2
					CBerIdentifier idobjectrcv;
					codeLength += idobjectrcv.decode(iStream);

					if ( idobjectorig != idobjectrcv )
					{
						qDebug() << "ERROR! Decode error: expected ID = "
								<< idobjectorig.toString()
								<< "; received ID = "
								<< idobjectrcv.toString() << ";";

						throw std::runtime_error("Decode error");
					}

					codeLength += temp_berobject->decode(iStream, false);
					QByteArray out = iStream.get();
					qDebug() << "CBerBaseStorage::deserialize ID+data[" << i << "]: " << out.toHex();
				}
				else
				{
					qDebug() << "Base Decoder: nullptr found";
				}
			}
			else
			{
				qDebug() << "ERROR! Decode error: can't convert to IBerBaseType*"
						<< idobjectorig.toString();
				break;
			}
		}
		else
		{
			// 3 & 4
			QVariant var = obj->metaObject()->property(i).read(obj);
			qDebug() << var.typeName() << "; " << var.userType() << "; ";
			if (var.canConvert(IBerBaseType::s_metaTypeId))
			{
				IBerBaseType* temp_berobject = var.value<IBerBaseType*>();
				if (temp_berobject != nullptr)
				{
					// 3
					codeLength += temp_berobject->decode(iStream, true);
					QByteArray out = iStream.get();
					qDebug() << "CBerBaseStorage::deserialize data[" << i << "]: " << out.toHex();
				}
				else
				{
					qDebug() << "Decode: nullptr found";
				}
			}
			else
			{
				qDebug() << "ERROR! Decode error: can't convert to IBerBaseType*";
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
