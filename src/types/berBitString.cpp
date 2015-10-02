#include "berBitString.h"

CBerBitString::CBerBitString()
{
	m_Identifier = s_Identifier;
}

CBerBitString::CBerBitString(QBitArray& bitString)
{
	m_Identifier = s_Identifier;
	m_BitString = bitString;
}

CBerBitString::CBerBitString(QByteArray& code)
{
	m_Identifier = s_Identifier;
	m_Code = code;
}

quint32 CBerBitString::serialize(CBerByteArrayOutputStream& berOStream)
{
	quint32 codeLength = 0;

	for (quint32 i = m_BitString.size()-1; i >= 0; --i)
	{
		quint8 data = (m_BitString[i] == false) ? 0 : 0xFF;
		berOStream.write(data);
	}
	berOStream.write( (quint8) (m_BitString.size()) );

	codeLength = m_BitString.size() + 1;

	return codeLength;
}

quint32 CBerBitString::deserialize(QDataStream& iStream, CBerLength& length, quint32 codeLength)
{
	m_BitString.clear();

	qint8 data[length.getVal()];
	if (m_BitString.size() > 0)
	{
		qint32 rdLength = iStream.readRawData((char*)data, (qint32) (length.getVal()) );
		if (rdLength == length.getVal())
		{
			m_BitString.resize(rdLength);
			for (qint32 i=0; i<rdLength; ++i)
			{
				if (data[i]) m_BitString.setBit(i);
				else m_BitString.clearBit(i);
			}

			codeLength += rdLength + 1;
		}
	}

	return codeLength;
}

quint32 CBerBitString::encode(CBerByteArrayOutputStream& berOStream, bool explct)
{
	quint32 codeLength = CBerBase::encode(berOStream, explct);

	return codeLength;
}

quint32 CBerBitString::decode(QDataStream& iStream, bool explct)
{
	int codeLength =  CBerBase::decode(iStream, explct);

	return codeLength;
}
