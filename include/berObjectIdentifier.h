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

#ifndef BER_OBJECTIDENTIFIER
#define BER_OBJECTIDENTIFIER

#include "berBase.h"
#include "berByteArrayOutputStream.h"
#include "berIdentifier.h"
#include "berLength.h"

class ASN1_SHAREDEXPORT CBerObjectIdentifier: public CBerBase
{

protected:

	QVector<qint32> m_ObjectIdentifierComponents;

public:
	static CBerIdentifier s_Identifier;

	CBerObjectIdentifier();
	CBerObjectIdentifier(QVector<qint32>& bitString);
	CBerObjectIdentifier(QByteArray& code);

	virtual ~CBerObjectIdentifier() {}

	virtual quint32 serialize(CBerByteArrayOutputStream& berOStream);
	virtual quint32 deserialize(QDataStream& iStream, CBerLength& length, quint32 codeLength);

	quint32 encode(CBerByteArrayOutputStream& berOStream, bool explct);
	quint32 decode(QDataStream& iStream, bool explct);

	virtual QString toString();
};

CBerIdentifier CBerObjectIdentifier::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::OBJECT_IDENTIFIER_TAG);

#endif
