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

#include "../../include/berBase.h"
#include "../../include/berByteArrayOutputStream.h"
#include "../../include/berIdentifier.h"
#include "../../include/berLength.h"

class CBerOctetString: public CBerBase
{

protected:

	QByteArray m_OctetString;

public:
	static CBerIdentifier s_Identifier;

	CBerOctetString();
	CBerOctetString(QByteArray& octetString);

	virtual ~CBerOctetString() {}

	virtual quint32 serialize(CBerByteArrayOutputStream& berOStream);
	virtual quint32 deserialize(QDataStream& iStream, CBerLength& length, quint32 codeLength);

	quint32 encode(CBerByteArrayOutputStream& berOStream, bool explct);
	quint32 decode(QDataStream& iStream, bool explct);

	virtual QString toString();

};

CBerIdentifier CBerOctetString::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::OCTET_STRING_TAG);

#endif
