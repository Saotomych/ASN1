#include "berBaseOctetString.h"

CBerBaseOctetString::CBerBaseOctetString()
{
}

CBerBaseOctetString::CBerBaseOctetString(QByteArray& octetString)
{
	m_OctetString = octetString;
}

quint32 CBerBaseOctetString::serialize(CBerByteArrayOutputStream& berOStream)
{

	berOStream.write(m_OctetString);

	quint32 codeLength = m_OctetString.size();
	codeLength += CBerLength::encodeLength(berOStream, codeLength);

	return codeLength;
}

quint32 CBerBaseOctetString::deserialize(CBerByteArrayInputStream& iStream, CBerLength& length, quint32 codeLength)
{

	qint32 lenval = length.getVal();

	if (lenval != 0)
	{
		QByteArray data(lenval, Qt::Initialization::Uninitialized);
		if (iStream.read(data, 0, lenval) < lenval)
		{
			runtimeError("CBerBaseOctetString::deserialize: error reading");
			return codeLength;
		}

		m_OctetString.clear();
		m_OctetString.append(data, sizeof(data) / sizeof(data[0]) );

		codeLength += lenval;
	}

	return codeLength;
}

quint32 CBerBaseOctetString::encode(CBerByteArrayOutputStream& berOStream, bool explct)
{
	quint32 codeLength = CBerBase::encode(berOStream, explct);

	return codeLength;
}

quint32 CBerBaseOctetString::decode(CBerByteArrayInputStream& iStream, bool explct)
{
	int codeLength =  CBerBase::decode(iStream, explct);

	return codeLength;
}

QString CBerBaseOctetString::toString()
{
	QString str(m_OctetString);

	return str;
}
