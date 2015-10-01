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

quint32 CBerBoolean::deserialize(QDataStream& iStream, CBerLength& length, quint32 codeLength)
{
	if (length.getVal() == 1)
	{
		char data;
		qint32 rdLength = iStream.readRawData(&data, 1);

		codeLength++;

		m_Val = (data == 0) ? false : true;
	}

	return codeLength;
}

quint32 CBerBoolean::encode(CBerByteArrayOutputStream& berOStream, bool explct)
{
	quint32 codeLength = CBerBase::encode(berOStream, explct);

	return codeLength;
}

quint32 CBerBoolean::decode(QDataStream& iStream, bool explct)
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

