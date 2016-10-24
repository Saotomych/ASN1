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

#include <asn1_global.h>
#include <berByteArrayOutputStream.h>
#include <berByteArrayInputStream.h>

class ASN1_SHAREDEXPORT CBerIdentifier: public QObject
{
	Q_OBJECT
	Q_PROPERTY(QByteArray* Code READ getCodeNoConst)

	QByteArray m_Identifier;
	qint32 m_IdentifierClass;
	qint32 m_Primitive;
	qint32 m_TagNumber;
	bool isExisting;

public:

    enum {
        UNIVERSAL_CLASS = 0x00,
        APPLICATION_CLASS = 0x40,
        CONTEXT_CLASS = 0x80,
        PRIVATE_CLASS = 0xc0,
        PRIMITIVE = 0x00,
        CONSTRUCTED = 0x20
    };

    enum {
        BOOLEAN_TAG = 1,
        INTEGER_TAG = 2,
        BIT_STRING_TAG = 3,
        OCTET_STRING_TAG = 4,
        NULL_TAG = 5,
        OBJECT_IDENTIFIER_TAG = 6,
        REAL_TAG = 9,
        ENUMERATED_TAG = 10,
        UTF8_STRING_TAG = 12,
        NUMERIC_STRING_TAG = 18,
        PRINTABLE_STRING_TAG = 19,
        TELETEX_STRING_TAG = 20,
        VIDEOTEX_STRING_TAG = 21,
        IA5_STRING_TAG = 22,
        GENERALIZED_TIME_TAG = 24,
        GRAPHIC_STRING_TAG = 25,
        VISIBLE_STRING_TAG = 26,
        GENERAL_STRING_TAG = 27,
        UNIVERSAL_STRING_TAG = 28,
        BMP_STRING_TAG = 30,
        PPDU_TAG = 16
    };

    enum {
        IDENTIFIER_CLASS_MASK = 0xC0,
        PRIMITIVE_MASK = 0x20,
        TAG_NUMBER_MASK = 0x1F,
    };

public:

	static quint32 s_metaTypeId;

	CBerIdentifier();

	CBerIdentifier(qint32 identifierClass, qint32 primitive, qint32 tagNumber);

	CBerIdentifier(const CBerIdentifier& rhs);

	CBerIdentifier& operator=(const CBerIdentifier& that);

	bool operator==(const CBerIdentifier& that) const;

	bool operator!=(const CBerIdentifier& that) const;

	~CBerIdentifier() {}

	quint32 encode(CBerByteArrayOutputStream& berOStream);

	quint32 decode(CBerByteArrayInputStream& iStream);

	const QByteArray* getCode() const { return &m_Identifier; }
	QByteArray* getCodeNoConst() { return &m_Identifier; }

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
