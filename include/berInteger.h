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

#ifndef BER_INTEGER
#define BER_INTEGER

#include "berIdentifier.h"
#include "berLength.h"
#include "storages/berIntegerStorage.h"
#include "storages/berBaseType.h"
#include "storages/decoder.h"

class ASN1_SHAREDEXPORT CBerInteger: public QObject, public IBerBaseType
{
	Q_OBJECT
	Q_PROPERTY(CBerIdentifier Identifier READ getIdentifier)
	Q_PROPERTY(QByteArray* Code READ getCode)
	Q_PROPERTY(qint64* Value READ getValue WRITE setValue)

protected:
	QByteArray m_Code;
	qint64 m_Val;

	const CBerIdentifier c_Identifier;

	void setValue(qint64* pVal) { m_Val = *pVal; }

	inline IBerBaseType* create_object_by_id(const CBerIdentifier&)
	{
		qDebug() << "INFO: CBerInteger has member already.";
		return nullptr;
	}

public:

	ASN1_CODEC(CBerIntegerStorage)

	static quint32 s_metaTypeId;

	static CBerIdentifier getBerIdentifier()
	{
		return CBerIdentifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::INTEGER_TAG);
	}

	CBerInteger();
	CBerInteger(qint64 val);
	CBerInteger(QByteArray code);
	CBerInteger(const CBerInteger& rhs);
	CBerInteger& operator=(const CBerInteger& rhs);
	bool operator!=(const CBerInteger& rhs);

	virtual ~CBerInteger() {}

	qint64* getValue() { return &m_Val; }
	QByteArray* getCode() { return &m_Code; }

	CBerIdentifier getIdentifier() { return c_Identifier; }

};

Q_DECLARE_METATYPE(CBerInteger*)

#endif
