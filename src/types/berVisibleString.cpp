#include <berVisibleString.h>

CBerVisibleString::CBerVisibleString()
{
	m_Identifier = getBerIdentifier();
}

CBerVisibleString::CBerVisibleString(QByteArray& octetString)
{
	m_Identifier = getBerIdentifier();
	m_OctetString = octetString;
}

CBerVisibleString::CBerVisibleString(QString& octetString)
{
	m_Identifier = getBerIdentifier();
	m_OctetString = octetString.toUtf8();
}

CBerVisibleString::CBerVisibleString(const CBerVisibleString& rhs): QObject()
{
	m_Identifier = rhs.m_Identifier;
	m_OctetString = rhs.m_OctetString;
	m_Code = rhs.m_Code;
}

CBerVisibleString& CBerVisibleString::operator=(const CBerVisibleString& rhs)
{
	if (this == &rhs) return *this;

	m_Identifier = rhs.m_Identifier;
	m_OctetString = rhs.m_OctetString;
	m_Code = rhs.m_Code;

	return *this;
}

bool CBerVisibleString::operator!=(const CBerVisibleString& rhs)
{
	if (this == &rhs) return false;

	return m_OctetString != rhs.m_OctetString;
}


