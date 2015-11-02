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

CBerVisibleString::CBerVisibleString(const CBerVisibleString& rhs)
{
	m_Identifier = rhs.m_Identifier;
	m_OctetString = rhs.m_OctetString;
	m_Code = rhs.m_Code;
}

CBerVisibleString& CBerVisibleString::operator=(const CBerVisibleString& rhs)
{
	if (this == &rhs) return *this;

	m_Identifier = rhs.m_Identifier;
	m_OctetString = rhs.m_OctetString;
	m_Code = rhs.m_Code;

	return *this;
}

quint32 CBerVisibleString::serialize(CBerByteArrayOutputStream& berOStream)
{
	berOStream.write(m_OctetString);

	quint32 codeLength = m_OctetString.size();
	codeLength += CBerLength::encodeLength(berOStream, codeLength);

	return codeLength;
}

quint32 CBerVisibleString::deserialize(CBerByteArrayInputStream& iStream, CBerLength& length, quint32 codeLength)
{
	qint32 lenval = length.getVal();

	if (lenval != 0)
	{
		QByteArray data(lenval, Qt::Initialization::Uninitialized);
		if (iStream.read(data, 0, lenval) < lenval)
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

quint32 CBerVisibleString::decode(CBerByteArrayInputStream& iStream, bool explct)
{
	int codeLength =  CBerBase::decode(iStream, explct);

	return codeLength;
}

QString CBerVisibleString::toString()
{
	QString octetString(m_OctetString);
	return octetString;
}


