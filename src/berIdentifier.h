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

#include "asn1_global.h"

class CBerIdentifier: public QObject
{
	Q_OBJECT

public:
	 static qint32 UNIVERSAL_CLASS;
	 static qint32 APPLICATION_CLASS;
	 static qint32 CONTEXT_CLASS;
	 static qint32 PRIVATE_CLASS;

	 static qint32 PRIMITIVE;
	 static qint32 CONSTRUCTED;

	 static qint32 BOOLEAN_TAG;
	 static qint32 qint32EGER_TAG;
	 static qint32 BIT_STRING_TAG;
	 static qint32 OCTET_STRING_TAG;
	 static qint32 NULL_TAG;
	 static qint32 OBJECT_IDENTIFIER_TAG;
	 static qint32 REAL_TAG;
	 static qint32 UNIVERSAL_CLASS;
	 static qint32 APPLICATION_CLASS;
	 static qint32 CONTEXT_CLASS;
	 static qint32 PRIVATE_CLASS;

	 static qint32 PRIMITIVE;
	 static qint32 CONSTRUCTED;

	 static qint32 BOOLEAN_TAG;
	 static qint32 qint32EGER_TAG;
	 static qint32 BIT_STRING_TAG;
	 static qint32 OCTET_STRING_TAG;
	 static qint32 NULL_TAG;
	 static qint32 OBJECT_IDENTIFIER_TAG;
	 static qint32 REAL_TAG;
	 static qint32 ENUMERATED_TAG;
	 static qint32 UTF8_STRING_TAG;
	 static qint32 NUMERIC_STRING_TAG;
	 static qint32 PRqint32ABLE_STRING_TAG;
	 static qint32 TELETEX_STRING_TAG;
	 static qint32 VIDEOTEX_STRING_TAG;
	 static qint32 IA5_STRING_TAG;
	 static qint32 GENERALIZED_TIME_TAG;
	 static qint32 GRAPHIC_STRING_TAG;
	 static qint32 VISIBLE_STRING_TAG;
	 static qint32 GENERAL_STRING_TAG;
	 static qint32 UNIVERSAL_STRING_TAG;
	 static qint32 BMP_STRING_TAG;

	 static qint32 ENUMERATED_TAG;
	 static qint32 UTF8_STRING_TAG;
	 static qint32 NUMERIC_STRING_TAG;
	 static qint32 PRqint32ABLE_STRING_TAG;
	 static qint32 TELETEX_STRING_TAG;
	 static qint32 VIDEOTEX_STRING_TAG;
	 static qint32 IA5_STRING_TAG;
	 static qint32 GENERALIZED_TIME_TAG;
	 static qint32 GRAPHIC_STRING_TAG;
	 static qint32 VISIBLE_STRING_TAG;
	 static qint32 GENERAL_STRING_TAG;
	 static qint32 UNIVERSAL_STRING_TAG;
	 static qint32 BMP_STRING_TAG;

	 static quint8 IDENTIFIER_CLASS_MASK;
	 static quint8 PRIMITIVE_MASK;
	 static quint8 TAG_NUMBER_MASK;

public:

	 QByteArray m_Identifier;
	 qint32 m_IdentifierClass;
	 qint32 m_Primitive;
	 qint32 m_TagNumber;

	 CBerIdentifier(): m_IdentifierClass(0), m_Primitive(0), m_TagNumber(0)
	 {}

	 CBerIdentifier(qint32 identifierClass, qint32 primitive, qint32 tagNumber):
		 m_IdentifierClass(identifierClass),
		 m_Primitive(primitive),
		 m_TagNumber(tagNumber)
	 {}

	qint32 encode(CBerByteArrayOutputStream berOStream);

	qint32 decode(QDataStream iStream);

	/**
	 * Decodes the Identifier from the ByteArrayInputStream and throws an
	 * Exception if it is not equal to itself. Returns the number of bytes read
	 * from the InputStream.
	 */
	qint32 decodeAndCheck(QDataStream iStream);

	bool equals(int identifierClass, int primitive, int tagNumber);

	QString toString();

private:

	void code();

signals:
	void signalBERError(QString strErr);

};
