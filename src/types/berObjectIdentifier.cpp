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
		argumentValid = argumentWrong("CBerObjectIdentifier::parameterWrong in constructor");

	if ( ( objectIdentifierComponents[0] == 0 || objectIdentifierComponents[1] == 1) && ( objectIdentifierComponents[1] > 39) )
		argumentValid = argumentWrong("CBerObjectIdentifier::parameterWrong in constructor");

	if ( objectIdentifierComponents[0] > 2 )
		argumentValid = argumentWrong("CBerObjectIdentifier::parameterWrong in constructor");

	for (auto objectIdentifierComponent: objectIdentifierComponents)
	{
		if (objectIdentifierComponent < 0)
		{
			argumentValid = argumentWrong("CBerObjectIdentifier::parameterWrong in constructor");
			break;
		}
	}

	if (argumentValid == true)
		m_ObjectIdentifier = objectIdentifierComponents;
}

CBerObjectIdentifier::CBerObjectIdentifier(QByteArray& code)
{
	m_Identifier = s_Identifier;
	m_Code = code;
}

