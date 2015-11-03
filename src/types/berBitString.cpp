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

CBerBitString::CBerBitString(const CBerBitString& rhs)
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
