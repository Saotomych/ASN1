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
	qint32 size = obj->metaObject()->propertyCount();

	for (qint32 i=3; i < size; ++i)
	{
		QVariant var = obj->metaObject()->property(i).read(obj);
		qDebug() << var.typeName() << "; " << var.userType() << "; ";

		if (var.canConvert(IBerBaseType::s_metaTypeId))
		{
			IBerBaseType* temp_berobject = var.value<IBerBaseType*>();

			if (temp_berobject != nullptr)
			{
				codeLength += temp_berobject->decode(iStream, explct);
			}

			if (obj->metaObject()->property(i-1).isReadable())
			{
				QVariant idvar = obj->metaObject()->property(i-1).read(obj);
				if (idvar.canConvert(CBerIdentifier::s_metaTypeId))
				{
					CBerIdentifier idobjectorig = idvar.value<CBerIdentifier>();
					CBerIdentifier idobjectrcv;
					codeLength += idobjectrcv.decode(iStream);

					if ( idobjectorig != idobjectrcv )
					{
						codeLength += temp_berobject->decode(iStream, false);
						qDebug() << "ERROR! Decode error: expected ID = "
								<< idobjectorig.toString()
								<< "; received ID = "
								<< idobjectrcv.toString() << ";";
					}

					codeLength += temp_berobject->decode(iStream, false);

				}
				else
				{
					codeLength += temp_berobject->decode(iStream, true);
				}
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
