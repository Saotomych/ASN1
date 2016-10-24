#include <berBoolean.h>

CBerBoolean::CBerBoolean(): m_Val(0)
{
	m_Identifier = getBerIdentifier();
}

CBerBoolean::CBerBoolean(bool val)
{
	m_Identifier = getBerIdentifier();
	m_Val = val;
}

CBerBoolean::CBerBoolean(QByteArray code): m_Val(0)
{
	m_Code = code;
}

CBerBoolean::CBerBoolean(const CBerBoolean& rhs): QObject()
{
	m_Identifier = rhs.m_Identifier;
	m_Code = rhs.m_Code;
	m_Val = rhs.m_Val;
}

CBerBoolean& CBerBoolean::operator=(const CBerBoolean& rhs)
{
	if (this == &rhs) return *this;

	m_Identifier = rhs.m_Identifier;
	m_Code = rhs.m_Code;
	m_Val = rhs.m_Val;

	return *this;
}

bool CBerBoolean::operator!=(const CBerBoolean& rhs)
{
	if (this == &rhs) return false;

	return m_Val != rhs.m_Val;
}
