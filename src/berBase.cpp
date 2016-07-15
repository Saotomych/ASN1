#include "berBase.h"

quint32 CBerBaseStorage::serialize(CBerByteArrayOutputStream& berOStream, QObject* obj, bool explct)
{
	quint32 codeLength = 0;
	qint32 size = obj->metaObject()->propertyCount()-1;

	for (qint32 i=size; i >= 3; --i)
	{
		if (obj->metaObject()->property(i).isReadable())
		{
			QVariant var = obj->metaObject()->property(i).read(obj);
			qDebug() << var.typeName() << "; " << var.userType() << "; ";

			if (var.canConvert(IBerBaseType::s_metaTypeId))
			{
				IBerBaseType* temp_berobject = var.value<IBerBaseType*>();
				if (temp_berobject != nullptr)
				{
					if (obj->metaObject()->property(i-1).isReadable())
					{
						QVariant idvar = obj->metaObject()->property(i-1).read(obj);
						if (idvar.canConvert(CBerIdentifier::s_metaTypeId))
						{
							codeLength += temp_berobject->encode(berOStream, false);

							CBerIdentifier idobject = idvar.value<CBerIdentifier>();
							if (idobject.IsExisting() || idobject.IsMandatory())
								codeLength += idobject.encode(berOStream);

						}
						else
						{
							codeLength += temp_berobject->encode(berOStream, true);
						}

						qDebug() << "Base Encoder: " << berOStream.getByteArray().toHex();

					}
				}
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

	for (qint32 i=3; i < size; ++i)
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
