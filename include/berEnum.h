/*
 * C++/QT Version
 * Open source, 2015
 * Author(s): Alexey Alyoshkin
 *
 * Based on Java version
 * Copyright Fraunhofer ISE, 2011
 * Author(s): Stefan Feuerhahn
 *
 * This file is part of jASN1.
 * For more information visit http://www.openmuc.org
 *
 * jASN1 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * jASN1 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with jASN1.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef BER_ENUM
#define BER_ENUM

#include "berIdentifier.h"
#include "berInteger.h"

class ASN1_SHAREDEXPORT CBerEnum: public CBerInteger
{

	CBerIdentifier m_Identifier;
	QByteArray m_Code;
	qint64 m_Val;

public:

	static CBerIdentifier s_Identifier;

	CBerEnum(): m_Val(0)
	{
		m_Identifier = s_Identifier;
	}

	CBerEnum(QByteArray& code): m_Val(0)
	{
		m_Identifier = s_Identifier;
		m_Code = code;
	}

	CBerEnum(qint64 val)
	{
		m_Identifier = s_Identifier;
		m_Val = val;
	}

};

CBerIdentifier CBerEnum::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::ENUMERATED_TAG);

#endif
