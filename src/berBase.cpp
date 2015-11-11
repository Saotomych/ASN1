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
			QObject* temp_qobject = qvariant_cast<QObject*> (var);
			IBerBaseType* temp_berobject = dynamic_cast<IBerBaseType*> (temp_qobject);

			if (temp_berobject != nullptr)
			{
				codeLength += temp_berobject->encode(berOStream, explct);
			}
		}
	}

	codeLength += CBerLength::encodeLength(berOStream, codeLength);

	return codeLength;
}

quint32 CBerBaseStorage::deserialize(CBerByteArrayInputStream& iStream, QObject* obj, CBerLength& length, quint32 codeLength, bool explct)
{
	qint32 size = obj->metaObject()->propertyCount();

	for (qint32 i=3; i < size; ++i)
	{
		if (obj->metaObject()->property(i).isReadable())
		{
			QVariant var = obj->metaObject()->property(i).read(obj);
			qDebug() << var.typeName() << "; " << var.userType() << "; ";
			QObject* temp_qobject = qvariant_cast<QObject*> (var);
			IBerBaseType* temp_berobject = dynamic_cast<IBerBaseType*> (temp_qobject);

			if (temp_berobject != nullptr)
			{
				codeLength += temp_berobject->decode(iStream, explct);
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
