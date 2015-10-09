#include "berBoolean.h"

CBerBoolean::CBerBoolean(): m_Val(0)
{
	m_Identifier = s_Identifier;
}

CBerBoolean::CBerBoolean(bool val)
{
	m_Identifier = s_Identifier;
	m_Val = val;
}

CBerBoolean::CBerBoolean(QByteArray code): m_Val(0)
{
	m_Code = code;
}

quint32 CBerBoolean::serialize(CBerByteArrayOutputStream& berOStream)
{
	quint32 codeLength = 1;

	quint8 data = (m_Val == false) ? 0 : 0xFF;
	berOStream.write(data);

	return codeLength;
}

quint32 CBerBoolean::deserialize(CBerByteArrayInputStream& iStream, CBerLength& length, quint32 codeLength)
{
	if (length.getVal() == 1)
	{
		qint32 data = iStream.read();
		if (data == -1)
		{
			runtimeError("CBerBoolean::deserialize: error reading");
			return codeLength;
		}

		codeLength++;

		m_Val = (data == 0) ? false : true;
	}
	else
	{
		runtimeError("CBerBoolean::deserialize: length read wrong");
	}

	return codeLength;
}

quint32 CBerBoolean::encode(CBerByteArrayOutputStream& berOStream, bool explct)
{
	quint32 codeLength = CBerBase::encode(berOStream, explct);

	return codeLength;
}

quint32 CBerBoolean::decode(CBerByteArrayInputStream& iStream, bool explct)
{
	int codeLength =  CBerBase::decode(iStream, explct);

	return codeLength;
}

void CBerBoolean::encodeAndSave(qint32 encodeSizeGuess)
{
	CBerByteArrayOutputStream berOStream(encodeSizeGuess);
	encode(berOStream, false);
	m_Code = berOStream.getByteArray();
}

