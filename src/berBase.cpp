#include "berBase.h"

quint32 CBerBaseStorage::serialize(CBerByteArrayOutputStream& berOStream, QObject* obj, bool explct)
{
	quint32 codeLength = 0;

	for (qint32 i=3; i < obj->metaObject()->propertyCount(); ++i)
	{
		if (obj->metaObject()->property(i).isReadable())
		{
			QVariant var = obj->metaObject()->property(i).read(obj);
			qDebug() << var.typeName() << "; " << var.userType() << "; ";
			CBerBaseStorage* temp_berobject = const_cast<CBerBaseStorage*> (reinterpret_cast<const CBerBaseStorage*> (var.constData()));
			QObject* temp_qobject = const_cast<QObject*> (reinterpret_cast<const QObject*> (var.constData()));

			if (temp_berobject != nullptr)
			{
				(const_cast<CBerBaseStorage*>(temp_berobject))->encode(berOStream, temp_qobject, explct);
			}
		}
	}

	return codeLength;
}

quint32 CBerBaseStorage::deserialize(CBerByteArrayInputStream& iStream, QObject* obj, CBerLength& length, quint32 codeLength, bool explct)
{

	return codeLength;
}

quint32 CBerBaseStorage::encode(CBerByteArrayOutputStream& berOStream, QObject* obj, bool explct)
{
	quint32 codeLength = 0;

	if (obj->metaObject()->propertyCount() < 3)
	{
		runtimeError("CBerBase::encode: QObject doesn't have Q_PROPERTIES.");
	}

	CBerIdentifier* pBerId = nullptr;
	QByteArray* pCode = nullptr;

	{
		QVariant var = obj->metaObject()->property(1).read(obj);
		qDebug() << var.typeName() << "; " << var.userType() << "; ";
		pBerId = const_cast<CBerIdentifier*> (reinterpret_cast<const CBerIdentifier*> (var.constData()));
	}

	{
		QVariant var = obj->metaObject()->property(2).read(obj);
		qDebug() << var.typeName() << "; " << var.userType() << "; ";
		pCode = const_cast<QByteArray*> (reinterpret_cast<const QByteArray*> (var.constData()));
	}

	if (pCode->size())
	{
		codeLength = pCode->size();
		for (qint32 i = codeLength-1; i >= 0; --i)
			berOStream.write((quint8) pCode[i] );
	}
	else
	{
		codeLength = serialize(berOStream, obj, explct);
	}

	if (explct)
	{
		codeLength += pBerId->encode(berOStream);
	}

	return codeLength;
}

quint32 CBerBaseStorage::decode(CBerByteArrayInputStream& iStream, QObject* obj, bool explct)
{
	qint32 codeLength = 0;

	CBerIdentifier* pBerId = nullptr;

	{
		QVariant var = obj->metaObject()->property(1).read(obj);
		qDebug() << var.typeName() << "; " << var.userType() << "; ";
		CBerIdentifier* pBerId = const_cast<CBerIdentifier*> (reinterpret_cast<const CBerIdentifier*> (var.constData()));
	}

	if (explct)
	{
		codeLength += pBerId->decodeAndCheck(iStream);
	}

	CBerLength length;
	codeLength += length.decode(iStream);

	codeLength += deserialize(iStream, obj, length, codeLength, explct);

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
