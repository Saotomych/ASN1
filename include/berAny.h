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

#ifndef BER_ANY
#define BER_ANY

#include <berByteArrayOutputStream.h>
#include <berBase.h>
#include <storages/berBaseType.h>
#include <storages/decoder.h>

class  ASN1_SHAREDEXPORT CBerAny: public QObject, public IBerBaseType
{
	Q_OBJECT
	Q_PROPERTY(CBerIdentifier Identifier READ getIdentifier)
	Q_PROPERTY(QByteArray* Code READ getCode)
	Q_PROPERTY(qint64* Length READ getValue)

protected:
	QByteArray m_Code;
	qint64 m_Length;

	CBerIdentifier c_Identifier;

	inline IBerBaseType* create_object_by_id(const CBerIdentifier&)
	{
		qDebug() << "INFO: CBerAny has member already.";
		return nullptr;
	}

public:
	static quint32 s_metaTypeId;

	static CBerIdentifier getBerIdentifier()
	{
		return CBerIdentifier(0,0,0);
	}

	CBerAny(): m_Length(0),
			c_Identifier(getBerIdentifier())
	{}

	CBerAny(quint32 length): m_Length(length),
			c_Identifier(getBerIdentifier())
	{}

	CBerAny(const CBerAny& rhs): QObject()
	{
		m_Code = rhs.m_Code;
		m_Length = rhs.m_Length;
	}

	CBerAny& operator=(const CBerAny& rhs)
	{
		if (this == &rhs) return *this;

		m_Code = rhs.m_Code;
		m_Length = rhs.m_Length;

		return *this;
	}

	bool operator!=(const CBerAny& rhs)
	{
		if (this == &rhs) return false;

		return m_Length != rhs.m_Length;
	}

	virtual ~CBerAny() {}

	virtual quint32 startEncode(CBerByteArrayOutputStream&)
	{
		return m_Length;
	}

	virtual quint32 nextEncode(CBerByteArrayOutputStream&)
	{
		return m_Length;
	}

	virtual quint32 startDecode(CBerByteArrayInputStream&)
	{
		return m_Length;
	}

	virtual quint32 nextDecode(CBerByteArrayInputStream&)
	{
		return m_Length;
	}

	virtual IBerBaseType* createMember(CBerIdentifier& id)
	{
		return create_object_by_id(id);
	}

	CBerIdentifier getIdentifier() { return c_Identifier; }

	QByteArray* getCode() { return &m_Code; }
	qint64* getValue() { return &m_Length; }

};

Q_DECLARE_METATYPE(CBerAny*)

#endif // BER_ANY
