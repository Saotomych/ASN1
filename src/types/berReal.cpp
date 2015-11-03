#include "berReal.h"

CBerReal::CBerReal(): m_Real(0.0)
{
	m_Identifier = s_Identifier;
}

CBerReal::CBerReal(double real)
{
	m_Identifier = s_Identifier;
	m_Real = real;
}

CBerReal::CBerReal(QByteArray& code): m_Real(0.0)
{
	m_Identifier = s_Identifier;
	m_Code = code;
}

