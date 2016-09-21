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

#ifndef BER_NULL
#define BER_NULL

#include "berIdentifier.h"
#include "berBase.h"
#include "berLength.h"
#include "storages/berNullStorage.h"
#include "storages/berBaseType.h"
#include "storages/decoder.h"

class ASN1_SHAREDEXPORT CBerNull: public QObject, public IBerBaseType
{
	Q_OBJECT
	Q_PROPERTY(CBerIdentifier Identifier READ getIdentifier)
	Q_PROPERTY(QByteArray* Code READ getCode)

protected:
	CBerIdentifier m_Identifier;
	QByteArray m_Code;

	inline IBerBaseType* create_object_by_id(const CBerIdentifier&)
	{
		qDebug() << "INFO: CBerNull has member already.";
		return nullptr;
	}

public:

	ASN1_CODEC(CBerNullStorage)

	static quint32 s_metaTypeId;

	static CBerIdentifier getBerIdentifier()
	{
		return CBerIdentifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::NULL_TAG);
	}

	CBerNull()
	{
		m_Identifier = getBerIdentifier();
	}

	virtual ~CBerNull() {}

	CBerNull(const CBerNull& rhs): QObject()
	{
		m_Identifier = rhs.m_Identifier;
		m_Code = rhs.m_Code;
	}

	CBerNull& operator=(const CBerNull& rhs)
	{
		if (this == &rhs) return *this;

		m_Identifier = rhs.m_Identifier;
		m_Code = rhs.m_Code;

		return *this;
	}

	QByteArray* getCode() { return &m_Code; }
	CBerIdentifier getIdentifier() { return m_Identifier; }
};

Q_DECLARE_METATYPE(CBerNull*)

#endif
