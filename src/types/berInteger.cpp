#include "berInteger.h"

CBerInteger::CBerInteger(): m_Val(0),
	c_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::INTEGER_TAG)
{}

CBerInteger::CBerInteger(qint64 val):
	c_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::INTEGER_TAG)
{
	m_Val = val;
}

CBerInteger::CBerInteger(QByteArray code): m_Val(0),
	c_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::INTEGER_TAG)
{
	m_Code = code;
}

CBerInteger::CBerInteger(const CBerInteger& rhs): QObject(),
	c_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::INTEGER_TAG)
{
	m_Val = rhs.m_Val;
	m_Code = rhs.m_Code;
}

CBerInteger& CBerInteger::operator=(const CBerInteger& rhs)
{
	if (this == &rhs) return *this;

	m_Val = rhs.m_Val;
	m_Code = rhs.m_Code;
	return *this;
}

bool CBerInteger::operator!=(const CBerInteger& rhs)
{
	if (this == &rhs) return false;

	return m_Val != rhs.m_Val;
}
