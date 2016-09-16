#include "berReal.h"

CBerReal::CBerReal(): m_Real(0.0)
{
	m_Identifier = getBerIdentifier();
}

CBerReal::CBerReal(double real)
{
	m_Identifier = getBerIdentifier();
	m_Real = real;
}

CBerReal::CBerReal(QByteArray& code): m_Real(0.0)
{
	m_Identifier = getBerIdentifier();
	m_Code = code;
}

CBerReal::CBerReal(const CBerReal& rhs): QObject()
{
	m_Identifier = rhs.m_Identifier;
	m_Code = rhs.m_Code;
	m_Real = rhs.m_Real;
}

CBerReal& CBerReal::operator=(const CBerReal& rhs)
{
	if (this == &rhs) return *this;

	m_Identifier = rhs.m_Identifier;
	m_Code = rhs.m_Code;
	m_Real = rhs.m_Real;

	return *this;
}

bool CBerReal::operator!=(const CBerReal& rhs)
{
	if (this == &rhs) return false;

	return m_Real != rhs.m_Real;
}
