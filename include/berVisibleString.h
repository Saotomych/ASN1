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

#ifndef BER_VISIBLESTRING
#define BER_VISIBLESTRING

#include "berIdentifier.h"
#include "berLength.h"
#include "storages/berOctetStringStorage.h"
#include "storages/berBaseType.h"
#include "storages/decoder.h"

class ASN1_SHAREDEXPORT CBerVisibleString: public QObject, public IBerBaseType
{
	Q_OBJECT
	Q_PROPERTY(CBerIdentifier Identifier READ getIdentifier)
	Q_PROPERTY(QByteArray* Code READ getCode)
	Q_PROPERTY(QByteArray* OctetString READ getValue WRITE setValue)

protected:
	CBerIdentifier m_Identifier;
	QByteArray m_Code;
	QByteArray m_OctetString;

	void setValue(QByteArray* pVal) { m_OctetString = *pVal; }

public:

	ASN1_CODEC(CBerOctetStringStorage)

	static quint32 s_metaTypeId;

	static CBerIdentifier getBerIdentifier()
	{
		return CBerIdentifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::VISIBLE_STRING_TAG);
	}

	CBerVisibleString();
	CBerVisibleString(QByteArray& octetString);
	CBerVisibleString(QString& octetString);
	CBerVisibleString(const CBerVisibleString& rhs);
	CBerVisibleString& operator=(const CBerVisibleString& rhs);
	bool operator!=(const CBerVisibleString& rhs);

	virtual ~CBerVisibleString() {}

	QByteArray* getValue() { return &m_OctetString; }
	QByteArray* getCode() { return &m_Code; }
	CBerIdentifier getIdentifier() { return m_Identifier; }

};

Q_DECLARE_METATYPE(CBerVisibleString*)

#endif
