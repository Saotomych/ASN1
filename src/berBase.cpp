#include "berBase.h"

quint32 CBerBase::encode(CBerByteArrayOutputStream& berOStream, bool explct)
{
	quint32 codeLength = 0;

	if (m_Code.size())
	{
		codeLength = m_Code.size();
		for (qint32 i = codeLength-1; i >= 0; --i)
			berOStream.write((quint8) m_Code[i] );
	}
	else
	{
		codeLength = serialize(berOStream);
	}

	if (explct)
	{
		codeLength += m_Identifier.encode(berOStream);
	}

	return codeLength;
}

quint32 CBerBase::decode(QDataStream& iStream, bool explct)
{
	int codeLength = 0;

	if (explct)
	{
		codeLength += m_Identifier.decodeAndCheck(iStream);
	}

	CBerLength length;
	codeLength += length.decode(iStream);

	codeLength += deserialize(iStream, length, codeLength);

	return codeLength;

}

bool CBerBase::runtimeError(QString strErr)
{
	emit signalDecodeError(strErr);
#ifdef DEBUG
	throw std::runtime_error(strErr.toStdString());
#endif

	return false;
}

bool CBerBase::argumentWrong(QString strErr)
{
	emit signalParameterWrong(strErr);

#ifdef DEBUG
	throw std::invalid_argument(strErr.toStdString());
#endif

	return false;
}
