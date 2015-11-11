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

CBerInteger::CBerInteger(const CBerInteger& rhs): QObject()
{
	m_Identifier = rhs.m_Identifier;
	m_Val = rhs.m_Val;
	m_Code = rhs.m_Code;
}

CBerInteger& CBerInteger::operator=(const CBerInteger& rhs)
{
	if (this == &rhs) return *this;

	m_Identifier = rhs.m_Identifier;
	m_Val = rhs.m_Val;
	m_Code = rhs.m_Code;
	return *this;
}

bool CBerInteger::operator!=(const CBerInteger& rhs)
{
	if (this == &rhs) return false;

	return m_Val != rhs.m_Val;
}
