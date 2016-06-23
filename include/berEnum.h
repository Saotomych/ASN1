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
#include "berLength.h"
#include "storages/berIntegerStorage.h"
#include "storages/berBaseType.h"
#include "storages/decoder.h"

class ASN1_SHAREDEXPORT CBerEnum: public QObject, public IBerBaseType
{
	Q_OBJECT
	Q_PROPERTY(CBerIdentifier Identifier READ getIdentifier)
	Q_PROPERTY(QByteArray* Code READ getCode)
	Q_PROPERTY(qint64* Val READ getValue WRITE setValue)

protected:
	CBerIdentifier m_Identifier;
	QByteArray m_Code;
	qint64 m_Val;

	void setValue(qint64* pVal) { m_Val = *pVal; }

public:

	ASN1_CODEC(CBerIntegerStorage)

	static CBerIdentifier s_Identifier;
	static quint32 s_metaTypeId;

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

	CBerEnum(const CBerEnum& rhs): QObject()
	{
		m_Identifier = rhs.m_Identifier;
		m_Code = rhs.m_Code;
		m_Val = rhs.m_Val;
	}

	virtual ~CBerEnum() {}

	CBerEnum& operator=(const CBerEnum& rhs)
	{
		if (this == &rhs) return *this;

		m_Identifier = rhs.m_Identifier;
		m_Code = rhs.m_Code;
		m_Val = rhs.m_Val;

		return *this;
	}

	CBerIdentifier getIdentifier() { return m_Identifier; }
	QByteArray* getCode() { return &m_Code; }
	qint64* getValue() { return &m_Val; }

};

Q_DECLARE_METATYPE(CBerEnum*)

#endif
