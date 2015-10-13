#include "berInteger.h"

CBerInteger::CBerInteger(): m_Val(0)
{
	m_Identifier = s_Identifier;
}

CBerInteger::CBerInteger(qint64 val)
{
	m_Identifier = s_Identifier;
	m_Val = val;
}

CBerInteger::CBerInteger(QByteArray& code): m_Val(0)
{
	m_Identifier = s_Identifier;
	m_Code = code;
}

quint32 CBerInteger::serialize(CBerByteArrayOutputStream& berOStream)
{
	quint32 codeLength = 1;

	while ( ( (m_Val > (pow(2, 8 * codeLength - 1) - 1)) || (m_Val < pow(-2, 8 * codeLength - 1)) ) && (codeLength < 8) )
		codeLength++;

	quint64 val = m_Val;
	for (quint32 i = 0; i < codeLength; ++i)
	{
		berOStream.write( (quint8) (val & 0xFF) );
		val >>= 8;
	}

	codeLength += CBerLength::encodeLength(berOStream, codeLength);

	return codeLength;
}

quint32 CBerInteger::deserialize(CBerByteArrayInputStream& iStream, CBerLength& length, quint32 codeLength)
{

	qint32 lenval = length.getVal();
	if ( lenval < 1 || lenval > 8)
	{
		runtimeError("CBerInteger::deserialize: decoded length");
		return codeLength;
	}

	QByteArray data(lenval, Qt::Initialization::Uninitialized);
	if ( iStream.read(data, 0, lenval) < lenval)
	{
		runtimeError("CBerInteger::deserialize: read wrong");
		return codeLength;
	}

	codeLength += lenval;

	if ( (data[0] & 0x80) == 0x80)
	{
		m_Val = -1;
		for (qint32 i = 0; i < lenval; ++i)
		{
			qint32 numShiftBits = 8 * (lenval - i -1);
			m_Val &= (quint64) ( ( (quint64)(data[i]) << numShiftBits) | ~( (quint64)(0xFF) << numShiftBits) );
		}
	}
	else
	{
		m_Val = 0;
		for (qint32 i = 0; i < lenval; ++i)
		{
			m_Val |= ( (quint64)(data[i]) << (8 * (lenval - i - 1)) );
		}
	}

	return codeLength;

}

quint32 CBerInteger::encode(CBerByteArrayOutputStream& berOStream, bool explct)
{
	quint32 codeLength = CBerBase::encode(berOStream, explct);

	return codeLength;
}

quint32 CBerInteger::decode(CBerByteArrayInputStream& iStream, bool explct)
{
	int codeLength =  CBerBase::decode(iStream, explct);

	return codeLength;
}

void CBerInteger::encodeAndSave(qint32 encodeSizeGuess)
{
	CBerByteArrayOutputStream berOStream(encodeSizeGuess);
	encode(berOStream, false);
	m_Code = berOStream.getByteArray();
}
