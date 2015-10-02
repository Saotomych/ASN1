#include "berNull.h"

CBerNull::CBerNull()
{
	m_Identifier = s_Identifier;
}

quint32 CBerNull::serialize(CBerByteArrayOutputStream& berOStream)
{
	quint32 codeLength = CBerLength::encodeLength(berOStream, 0);

	return codeLength;
}

quint32 CBerNull::deserialize(QDataStream& iStream, CBerLength& length, quint32 codeLength)
{
	return codeLength;
}

quint32 CBerNull::encode(CBerByteArrayOutputStream& berOStream, bool explct)
{
	quint32 codeLength = CBerBase::encode(berOStream, explct);

	return codeLength;
}

quint32 CBerNull::decode(QDataStream& iStream, bool explct)
{
	int codeLength =  CBerBase::decode(iStream, explct);

	if (codeLength != 0)
	{
		emit signalDecodeError("CBerNull::decode wrong");

#ifdef DEBUG
		throw std::runtime_error("CBerNull::decode wrong");
#endif

	}

	return codeLength;
}

