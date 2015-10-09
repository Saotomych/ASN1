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

	for (qint32 i = m_BitString.size()-1; i >= 0; --i)
	{
		quint8 data = (m_BitString[i] == false) ? 0 : 0xFF;
		berOStream.write(data);
	}
	berOStream.write( (quint8) (m_BitString.size()) );

	codeLength = m_BitString.size() + 1;

	return codeLength;
}

quint32 CBerBitString::deserialize(CBerByteArrayInputStream& iStream, CBerLength& length, quint32 codeLength)
{
	m_BitString.clear();

	QByteArray data(length.getVal(), Qt::Initialization::Uninitialized);
	if (m_BitString.size() > 0)
	{
		quint32 rdLength = iStream.read(data, 0, (qint32) (length.getVal()) );
		if (rdLength == length.getVal())
		{
			m_BitString.resize(rdLength);
			for (quint32 i=0; i<rdLength; ++i)
			{
				if (data[i]) m_BitString.setBit(i);
				else m_BitString.clearBit(i);
			}

			codeLength += rdLength + 1;
		}
		else
		{
			runtimeError("CBerBitString::deserialize: error decoding");
		}
	}

	return codeLength;
}

quint32 CBerBitString::encode(CBerByteArrayOutputStream& berOStream, bool explct)
{
	quint32 codeLength = CBerBase::encode(berOStream, explct);

	return codeLength;
}

quint32 CBerBitString::decode(CBerByteArrayInputStream& iStream, bool explct)
{
	int codeLength =  CBerBase::decode(iStream, explct);

	return codeLength;
}
