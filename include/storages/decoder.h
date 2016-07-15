/*
 * decoder.h
 *
 *  Created on: 10 нояб. 2015 г.
 *      Author: alexey
 */

#ifndef INCLUDE_STORAGES_DECODER_H_
#define INCLUDE_STORAGES_DECODER_H_

#include "asn1_global.h"
#include "berByteArrayOutputStream.h"
#include "berByteArrayInputStream.h"
#include "berLength.h"
#include "berIdentifier.h"

#define ASN1_CODEC(Storage) \
	virtual quint32 encode(CBerByteArrayOutputStream& berOStream, bool explct) \
	{ \
		CDecoder< Storage > codec; \
		return codec.encode(berOStream, this, explct); \
	} \
	virtual quint32 decode(CBerByteArrayInputStream& iStream, bool explct) \
	{ \
		CDecoder< Storage > codec; \
		return codec.decode(iStream, this, explct); \
	} \

template<class TStorage>
class ASN1_SHAREDEXPORT CDecoder
{
private:

	TStorage m_storage;

public:

	quint32 encode(CBerByteArrayOutputStream& berOStream, QObject* obj, bool explct)
	{
		quint32 codeLength = 0;

		QVariant codeVariant = obj->metaObject()->property(2).read(obj);
		qDebug() << codeVariant.typeName() << "; " << codeVariant.userType() << "; ";

		QByteArray* pCode = qvariant_cast<QByteArray*> (codeVariant);

		if (pCode && pCode->size())
		{
			codeLength = pCode->size();
			for (qint32 i = codeLength-1; i >= 0; --i)
			{
				berOStream.write((quint8) ((*pCode)[i]) );
			}
		}
		else
		{
			codeLength = m_storage.serialize(berOStream, obj, explct);
		}

		QVariant IdVariant = obj->metaObject()->property(1).read(obj);
		if ( IdVariant.canConvert( CBerIdentifier::s_metaTypeId) )
		{
			qDebug() << IdVariant.typeName() << "; " << IdVariant.userType() << "; ";
			CBerIdentifier BerId = qvariant_cast<CBerIdentifier> (IdVariant);

			if (explct && BerId.IsExisting())
				codeLength += BerId.encode(berOStream);

			if (!explct && BerId.IsMandatory())
				codeLength += BerId.encode(berOStream);
		}

		return codeLength;
	}

	quint32 decode(CBerByteArrayInputStream& iStream, QObject* obj, bool explct)
	{
		qint32 codeLength = 0;

		if (explct)
		{
			QVariant var = obj->metaObject()->property(1).read(obj);
			if ( var.canConvert(CBerIdentifier::s_metaTypeId) )
			{
				qDebug() << var.typeName() << "; " << var.userType() << "; ";
				CBerIdentifier BerId = qvariant_cast<CBerIdentifier> (var);
				if (BerId.IsExisting())
					codeLength += BerId.decodeAndCheck(iStream);
			}
		}

		CBerLength length;

		codeLength += m_storage.deserialize(iStream, obj, length, codeLength, explct);

		QByteArray out = iStream.get();
		qDebug() << "CDecoder::decode class: " << out.toHex();

		return codeLength;

	}

};



#endif /* INCLUDE_STORAGES_DECODER_H_ */
