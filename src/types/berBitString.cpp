#include "berBitString.h"

CBerBitString::CBerBitString()
{
	m_Identifier = getBerIdentifier();
}

CBerBitString::CBerBitString(QBitArray& bitString)
{
	m_Identifier = getBerIdentifier();
	m_BitString = bitString;
}

CBerBitString::CBerBitString(QByteArray& code)
{
	m_Identifier = getBerIdentifier();
	m_Code = code;
}

CBerBitString::CBerBitString(const CBerBitString& rhs): QObject()
{
	m_Identifier = rhs.m_Identifier;
	m_BitString = rhs.m_BitString;
	m_Code = rhs.m_Code;
}

CBerBitString& CBerBitString::operator=(const CBerBitString& rhs)
{
	if (this == &rhs) return *this;

	m_Identifier = rhs.m_Identifier;
	m_BitString = rhs.m_BitString;
	m_Code = rhs.m_Code;

	return *this;
}

bool CBerBitString::operator!=(const CBerBitString& rhs)
{
	Q_ASSERT(false);

	if (this == &rhs) return false;

	return m_BitString != rhs.m_BitString;
}
