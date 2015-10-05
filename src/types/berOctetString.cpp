#include "berOctetString.h"

CBerOctetString::CBerOctetString()
{
	m_Identifier = s_Identifier;
}

CBerOctetString::CBerOctetString(QByteArray& octetString)
{
	m_Identifier = s_Identifier;
	m_OctetString = octetString;
}

quint32 CBerOctetString::serialize(CBerByteArrayOutputStream& berOStream)
{

	berOStream.write(m_OctetString);

	quint32 codeLength = m_OctetString.size();
	codeLength += CBerLength::encodeLength(berOStream, codeLength);

	return codeLength;
}

quint32 CBerOctetString::deserialize(QDataStream& iStream, CBerLength& length, quint32 codeLength)
{

	qint32 lenval = length.getVal();

	if (lenval != 0)
	{
		char data[lenval];
		if (iStream.readRawData(data, lenval) < lenval)
		{
			runtimeError("CBerOctetString::deserialize: error decoding");
			return codeLength;
		}

		m_OctetString.clear();
		m_OctetString.append(data, sizeof(data) / sizeof(data[0]) );

		codeLength += lenval;
	}

	return codeLength;
}

quint32 CBerOctetString::encode(CBerByteArrayOutputStream& berOStream, bool explct)
{
	quint32 codeLength = CBerBase::encode(berOStream, explct);

	return codeLength;
}

quint32 CBerOctetString::decode(QDataStream& iStream, bool explct)
{
	int codeLength =  CBerBase::decode(iStream, explct);

	return codeLength;
}

QString CBerOctetString::toString()
{
	QString str(m_OctetString);

	return str;
}
