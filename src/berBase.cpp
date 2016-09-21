#include "berBase.h"
#include "storages/decoder.h"

namespace lastBerIdentifier
{

	CBerIdentifier id;
	bool loaded = false;

	CBerIdentifier get(CBerByteArrayInputStream& iStream, quint32& codeLength)
	{
		if ( loaded == false )
		{
			codeLength += id.decode(iStream);

			qDebug() << "lastBerIdentifier::get, idobjectReceive =  " << id.toString()
					<< "; as code = " << id.getCode()->toHex();

			loaded = true;
		}

		return id;
	}

	void reset()
	{
		loaded = false;
	}
}


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

	CBerIdentifier idobjectReceive;

	for (qint32 i=START_PROPERTY_INDEX; i < size; ++i)
	{
		idobjectReceive = lastBerIdentifier::get(iStream, codeLength);

		// Проверяем режим декодирования
		QVariant varpos2 = obj->metaObject()->property(i).read(obj);
		QVariant varpos1 = getNextVariant(obj, varpos2, i+1);
		QVariant varpos0 = getNextVariant(obj, varpos1, i+2);
		if (varpos1.canConvert(IBerBaseType::s_metaTypeId))	// For not full case
			varpos1 = varpos2;

		auto type = getWorkType(obj, varpos0, varpos1, varpos2);

		qDebug() << "CBerBaseStorage::deserialize: types 2, 1, 0: " << varpos2.typeName() << "; "
				<< varpos1.typeName() << "; "
				<< varpos0.typeName() << ".";

		qDebug() << "CBerBaseStorage::deserialize: type = "
				<< ((type == WorkType::PARENT_IDENTIFIER) ? "Id without common length" : "Id with common length");

		Q_ASSERT_X(type < WorkType::NOT_IDENTIFIED_MODE, "CBerBaseStorage::deserialize:", "type < NOT_IDENTIFIED_MODE");

		CBerIdentifier idobjectOriginal = varpos2.value<CBerIdentifier>();

		qDebug() << "CBerBaseStorage::deserialize, try for idobjectReceive =  " << idobjectReceive.toString()
						<< "; as code = " << idobjectReceive.getCode()->toHex();

		qDebug() << "CBerBaseStorage::deserialize, try for idobjectOriginal =  " << idobjectOriginal.toString()
				<< "; as code = " << idobjectOriginal.getCode()->toHex();

		switch(type)
		{
		case WorkType::PARENT_IDENTIFIER:
			{
				if ( idobjectOriginal == idobjectReceive || !idobjectOriginal.IsExisting() )
				{
					qDebug() << "Base deserialize, idobjectOriginal = idobjectReceive with index = " << i;

					IBerBaseType* temp_berobject = (dynamic_cast<IBerBaseType*>(obj))->createMember(idobjectOriginal);
					if (temp_berobject != nullptr)
					{
						if (idobjectOriginal.IsExisting())
							lastBerIdentifier::reset();
						codeLength += temp_berobject->decode(iStream, false);
					}
					else
					{
						runtimeError("Base deserialize. Received object can't create by Base Storage deserialize.");
					}

					QByteArray out = iStream.get();
					qDebug() << "CBerBaseStorage::deserialize: data[" << i << "]: " << out.toHex();

				}
				else
				{
					qDebug() << "CBerBaseStorage::deserialize: data[" << i << "]: skipped";
				}
			}
			++i;
			break;

		case WorkType::PARENT_IDENTIFIER_WITH_LENGTH:
			{
				if ( idobjectOriginal == idobjectReceive || !idobjectOriginal.IsExisting() )
				{
					qDebug() << "Base deserialize, idobjectOriginal = idobjectReceive with index = " << i;

					IBerBaseType* temp_berobject = (dynamic_cast<IBerBaseType*>(obj))->createMember(idobjectOriginal);
					if (temp_berobject != nullptr)
					{
						if (idobjectOriginal.IsExisting())
							lastBerIdentifier::reset();
						quint32 subCodeLength = length.decode(iStream);
						subCodeLength += temp_berobject->decode(iStream, false);
						codeLength += subCodeLength;
					}
					else
					{
						runtimeError("Base deserialize. Received object can't create by Base Storage deserialize.");
					}

					QByteArray out = iStream.get();
					qDebug() << "CBerBaseStorage::deserialize: data[" << i << "]: " << out.toHex();

				}
				else
				{
					qDebug() << "CBerBaseStorage::deserialize: data[" << i << "]: skipped";
				}
			}
			i+=2;
			break;

		case WorkType::NOT_IDENTIFIED_MODE:
		default:
			QString str = QString("ERROR! CBerBaseStorage::deserialize type %1 isn't identified")
					.arg( (quint32) type);
			Q_ASSERT_X(false, "CBerBaseStorage::deserialize", str.toStdString().c_str());
			break;
		}
	}

	return codeLength;
}

void CBerBaseStorage::runtimeError(QString strErr)
{
	qDebug() << strErr;
//#ifdef DEBUG
	throw std::runtime_error(strErr.toStdString());
//#endif
}

void CBerBaseStorage::argumentWrong(QString strErr)
{
	qDebug() << strErr;

//#ifdef DEBUG
	throw std::invalid_argument(strErr.toStdString());
//#endif
}
