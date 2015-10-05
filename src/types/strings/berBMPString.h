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

#ifndef BER_OCTETSTRING
#define BER_OCTETSTRING

#include "types/berOctetString.h"
#include "berByteArrayOutputStream.h"
#include "berIdentifier.h"
#include "berLength.h"

class CBerBMPString: public CBerOctetString
{

public:
	static CBerIdentifier s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::BMP_STRING_TAG);

	CBerBMPString()
	{
		m_Identifier = s_Identifier;
	}

	CBerBMPString(QByteArray& octetString)
	{
		m_Identifier = s_Identifier;
		m_OctetString = octetString;
	}

	virtual ~CBerBMPString() {}
};

#endif
