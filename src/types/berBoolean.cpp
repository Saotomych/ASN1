#include "berBoolean.h"

CBerBoolean::CBerBoolean(): m_Val(0)
{
	m_Identifier = s_Identifier;
}

CBerBoolean::CBerBoolean(bool val)
{
	m_Identifier = s_Identifier;
	m_Val = val;
}

CBerBoolean::CBerBoolean(QByteArray code): m_Val(0)
{
	m_Code = code;
}

