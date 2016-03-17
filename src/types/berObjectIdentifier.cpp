#include "berObjectIdentifier.h"

CBerObjectIdentifier::CBerObjectIdentifier()
{
	m_Identifier = s_Identifier;
}

CBerObjectIdentifier::CBerObjectIdentifier(QVector<qint32>& objectIdentifierComponents)
{
	bool argumentValid = true;

	m_Identifier = s_Identifier;

	if ( objectIdentifierComponents.size() < 2 )
	{
		qDebug() << QString("CBerObjectIdentifier::parameterWrong in constructor");
		argumentValid = false;

		qDebug() << QString("ERROR: CBerObjectIdentifier has error in objectIdentifierComponents with length=")
				<< objectIdentifierComponents.size();
		return;
	}

	if ( ( objectIdentifierComponents[0] == 0 || objectIdentifierComponents[1] == 1) && ( objectIdentifierComponents[1] > 39) )
	{
		qDebug() << QString("CBerObjectIdentifier::parameterWrong in constructor");
		argumentValid = false;
	}

	if ( objectIdentifierComponents[0] > 2 )
	{
		qDebug() << QString("CBerObjectIdentifier::parameterWrong in constructor");
		argumentValid = false;
	}

	for (auto objectIdentifierComponent: objectIdentifierComponents)
	{
		if (objectIdentifierComponent < 0)
		{
			qDebug() << QString("CBerObjectIdentifier::parameterWrong in constructor");
			argumentValid = false;
			break;
		}
	}

	if (argumentValid)
	{
		m_ObjectIdentifier = objectIdentifierComponents;
	}
	else
	{
		qDebug() << QString("ERROR: CBerObjectIdentifier has error in objectIdentifierComponents with length=")
				<< objectIdentifierComponents.size();
		for (auto objectIdentifierComponent: objectIdentifierComponents)
		{
			qDebug() << objectIdentifierComponent;
		}
	}
}

CBerObjectIdentifier::CBerObjectIdentifier(QByteArray code)
{
	m_Identifier = s_Identifier;
	m_Code = code;
}

CBerObjectIdentifier::CBerObjectIdentifier(const CBerObjectIdentifier& rhs): QObject()
{
	m_Identifier = rhs.m_Identifier;
	m_Code = rhs.m_Code;
	m_ObjectIdentifier = rhs.m_ObjectIdentifier;
}

CBerObjectIdentifier& CBerObjectIdentifier::operator=(const CBerObjectIdentifier& rhs)
{
	if (this == &rhs) return *this;

	m_Identifier = rhs.m_Identifier;
	m_Code = rhs.m_Code;
	m_ObjectIdentifier = rhs.m_ObjectIdentifier;

	return *this;
}

bool CBerObjectIdentifier::operator!=(const CBerObjectIdentifier& rhs)
{
	if (this == &rhs) return false;

	return m_ObjectIdentifier != rhs.m_ObjectIdentifier;
}

bool CBerObjectIdentifier::operator==(const CBerObjectIdentifier& rhs)
{
	if (this == &rhs) return true;

	return m_ObjectIdentifier == rhs.m_ObjectIdentifier;
}

