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

#ifndef BER_IDENTIFIER
#define BER_IDENTIFIER

#include "asn1_global.h"
#include "berByteArrayOutputStream.h"
#include "berByteArrayInputStream.h"
#include "storages/berBaseType.h"

class ASN1_SHAREDEXPORT CBerIdentifier: public QObject
{
	Q_OBJECT
	Q_PROPERTY(QByteArray* Code READ getCode)

	QByteArray m_Identifier;
	qint32 m_IdentifierClass;
	qint32 m_Primitive;
	qint32 m_TagNumber;
	bool isExisting;

public:
	 static qint32 UNIVERSAL_CLASS;
	 static qint32 APPLICATION_CLASS;
	 static qint32 CONTEXT_CLASS;
	 static qint32 PRIVATE_CLASS;

	 static qint32 PRIMITIVE;
	 static qint32 CONSTRUCTED;

	 static qint32 BOOLEAN_TAG;
	 static qint32 INTEGER_TAG;
	 static qint32 BIT_STRING_TAG;
	 static qint32 OCTET_STRING_TAG;
	 static qint32 NULL_TAG;
	 static qint32 OBJECT_IDENTIFIER_TAG;
	 static qint32 REAL_TAG;
	 static qint32 ENUMERATED_TAG;
	 static qint32 UTF8_STRING_TAG;
	 static qint32 NUMERIC_STRING_TAG;
	 static qint32 PRINTABLE_STRING_TAG;
	 static qint32 TELETEX_STRING_TAG;
	 static qint32 VIDEOTEX_STRING_TAG;
	 static qint32 IA5_STRING_TAG;
	 static qint32 GENERALIZED_TIME_TAG;
	 static qint32 GRAPHIC_STRING_TAG;
	 static qint32 VISIBLE_STRING_TAG;
	 static qint32 GENERAL_STRING_TAG;
	 static qint32 UNIVERSAL_STRING_TAG;
	 static qint32 BMP_STRING_TAG;
	 static qint32 PPDU_TAG;

	 static quint8 IDENTIFIER_CLASS_MASK;
	 static quint8 PRIMITIVE_MASK;
	 static quint8 TAG_NUMBER_MASK;

public:

	static quint32 s_metaTypeId;

	CBerIdentifier();

	CBerIdentifier(qint32 identifierClass, qint32 primitive, qint32 tagNumber);

	CBerIdentifier(const CBerIdentifier& rhs);

	CBerIdentifier& operator=(const CBerIdentifier& that);

	bool operator==(const CBerIdentifier& that);

	bool operator!=(const CBerIdentifier& that);

	~CBerIdentifier() {}

	quint32 encode(CBerByteArrayOutputStream& berOStream);

	quint32 decode(CBerByteArrayInputStream& iStream);

	QByteArray* getCode() { return &m_Identifier; }

	/**
	 * Decodes the Identifier from the ByteArrayInputStream and throws an
	 * Exception if it is not equal to itself. Returns the number of bytes read
	 * from the InputStream.
	 */
	quint32 decodeAndCheck(CBerByteArrayInputStream& iStream);

	bool equals(CBerIdentifier& obj)
	{
		return equals(obj.m_IdentifierClass, m_Primitive, m_TagNumber);
	}

	bool equals(int identifierClass, int primitive, int tagNumber);

	QString toString();

	bool IsExisting() { return isExisting; }

private:

	void code();

signals:
	void signalBERError(QString strErr);

};

Q_DECLARE_METATYPE(CBerIdentifier)

#endif
