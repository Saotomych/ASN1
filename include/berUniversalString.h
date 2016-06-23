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

#ifndef BER_UNIVERSALSTRING
#define BER_UNIVERSALSTRING

#include "berIdentifier.h"
#include "berLength.h"
#include "storages/berOctetStringStorage.h"
#include "storages/berBaseType.h"
#include "storages/decoder.h"

class ASN1_SHAREDEXPORT CBerUniversalString: public QObject, public IBerBaseType
{
	Q_OBJECT
	Q_PROPERTY(CBerIdentifier Identifier READ getIdentifier)
	Q_PROPERTY(QByteArray* Code READ getCode)
	Q_PROPERTY(QByteArray* OctetString READ getValue)

protected:
	CBerIdentifier m_Identifier;
	QByteArray m_Code;
	QByteArray m_OctetString;

	void setValue(QByteArray* pVal) { m_OctetString = *pVal; }

public:

	ASN1_CODEC(CBerOctetStringStorage)

	static CBerIdentifier s_Identifier;
	static quint32 s_metaTypeId;

	CBerUniversalString()
	{
		m_Identifier = s_Identifier;
	}

	CBerUniversalString(QByteArray& octetString)
	{
		m_Identifier = s_Identifier;
		m_OctetString = octetString;
	}

	CBerUniversalString(const CBerUniversalString& rhs): QObject()
	{
		m_Identifier = rhs.m_Identifier;
		m_Code = rhs.m_Code;
		m_OctetString = rhs.m_OctetString;
	}

	virtual ~CBerUniversalString() {}

	CBerUniversalString& operator=(const CBerUniversalString& rhs)
	{
		if (this == &rhs) return *this;

		m_Identifier = rhs.m_Identifier;
		m_Code = rhs.m_Code;
		m_OctetString = rhs.m_OctetString;

		return *this;
	}

	bool operator!=(const CBerUniversalString& rhs)
	{
		if (this == &rhs) return false;

		return m_OctetString != rhs.m_OctetString;
	}

	QByteArray* getValue() { return &m_OctetString; }
	QByteArray* getCode() { return &m_Code; }
	CBerIdentifier getIdentifier() { return m_Identifier; }

};

Q_DECLARE_METATYPE(CBerUniversalString*)

#endif
