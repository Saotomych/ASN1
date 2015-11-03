#include "berInteger.h"

CBerInteger::CBerInteger(): QObject(), CStorageAdapter(EnumPrimitives::CBerInteger), m_Val(0)
{
	m_Identifier = s_Identifier;
}

CBerInteger::CBerInteger(qint64 val): QObject(), CStorageAdapter(EnumPrimitives::CBerInteger)
{
	m_Identifier = s_Identifier;
	m_Val = val;
}

CBerInteger::CBerInteger(QByteArray& code): QObject(), CStorageAdapter(EnumPrimitives::CBerInteger), m_Val(0)
{
	m_Identifier = s_Identifier;
	m_Code = code;
}

CBerInteger::CBerInteger(const CBerInteger& rhs): QObject(), CStorageAdapter(EnumPrimitives::CBerInteger)
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

