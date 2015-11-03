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

#ifndef BER_BITSTRING
#define BER_BITSTRING

#include "berIdentifier.h"
#include "berLength.h"
#include "storages/berBitStringStorage.h"

class ASN1_SHAREDEXPORT CBerBitString: public QObject, public CBerBitStringStorage
{
	Q_OBJECT
	Q_PROPERTY(CBerIdentifier Identifier MEMBER m_Identifier)
	Q_PROPERTY(QByteArray Code MEMBER m_Code)
	Q_PROPERTY(QBitArray BitString MEMBER m_BitString)

protected:
	CBerIdentifier m_Identifier;
	QByteArray m_Code;
	QBitArray m_BitString;

public:
	static CBerIdentifier s_Identifier;
	static quint32 s_metaTypeId;

	CBerBitString();
	CBerBitString(QBitArray& bitString);
	CBerBitString(QByteArray& code);
	CBerBitString(const CBerBitString& rhs);
	CBerBitString& operator=(const CBerBitString& rhs);
};

Q_DECLARE_METATYPE(CBerBitString)

#endif
