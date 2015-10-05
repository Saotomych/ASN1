#include "berVisibleString.h"

CBerVisibleString::CBerVisibleString()
{
	m_Identifier = s_Identifier;
}

CBerVisibleString::CBerVisibleString(QByteArray& octetString)
{
	m_Identifier = s_Identifier;
	m_OctetString = octetString;
}

CBerVisibleString::CBerVisibleString(QString& octetString)
{
	m_Identifier = s_Identifier;
	m_OctetString = octetString.toUtf8();
}

quint32 CBerVisibleString::serialize(CBerByteArrayOutputStream& berOStream)
{
	berOStream.write(m_OctetString);

	quint32 codeLength = m_OctetString.size();
	codeLength += CBerLength::encodeLength(berOStream, codeLength);

	return codeLength;
}

quint32 CBerVisibleString::deserialize(QDataStream& iStream, CBerLength& length, quint32 codeLength)
{
	qint32 lenval = length.getVal();

	if (lenval != 0)
	{
		char data[lenval];
		if (iStream.readRawData(data, lenval) < lenval)
		{
			runtimeError("CBerVisibleString::deserialize: error decoding");
			return codeLength;
		}

		m_OctetString.clear();
		m_OctetString.append(data, sizeof(data) / sizeof(data[0]) );

		codeLength += lenval;
	}

	return codeLength;
}

quint32 CBerVisibleString::encode(CBerByteArrayOutputStream& berOStream, bool explct)
{
	quint32 codeLength = CBerBase::encode(berOStream, explct);

	return codeLength;
}

quint32 CBerVisibleString::decode(QDataStream& iStream, bool explct)
{
	int codeLength =  CBerBase::decode(iStream, explct);

	return codeLength;
}

QString CBerVisibleString::toString()
{
	QString octetString(m_OctetString);
	return octetString;
}


