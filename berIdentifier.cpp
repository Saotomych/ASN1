#include "berIdentifier.h"

qint32 CBerIdentifier::UNIVERSAL_CLASS = 0x00;
qint32 CBerIdentifier::APPLICATION_CLASS = 0x40;
qint32 CBerIdentifier::CONTEXT_CLASS = 0x80;
qint32 CBerIdentifier::PRIVATE_CLASS = 0xc0;

qint32 CBerIdentifier::PRIMITIVE = 0x00;
qint32 CBerIdentifier::CONSTRUCTED = 0x20;

qint32 CBerIdentifier::BOOLEAN_TAG = 1;
qint32 CBerIdentifier::qint32EGER_TAG = 2;
qint32 CBerIdentifier::BIT_STRING_TAG = 3;
qint32 CBerIdentifier::OCTET_STRING_TAG = 4;
qint32 CBerIdentifier::NULL_TAG = 5;
qint32 CBerIdentifier::OBJECT_IDENTIFIER_TAG = 6;
qint32 CBerIdentifier::REAL_TAG = 9;
qint32 CBerIdentifier::UNIVERSAL_CLASS = 0x00;
qint32 CBerIdentifier::APPLICATION_CLASS = 0x40;
qint32 CBerIdentifier::CONTEXT_CLASS = 0x80;
qint32 CBerIdentifier::PRIVATE_CLASS = 0xc0;

qint32 CBerIdentifier::PRIMITIVE = 0x00;
qint32 CBerIdentifier::CONSTRUCTED = 0x20;

qint32 CBerIdentifier::BOOLEAN_TAG = 1;
qint32 CBerIdentifier::qint32EGER_TAG = 2;
qint32 CBerIdentifier::BIT_STRING_TAG = 3;
qint32 CBerIdentifier::OCTET_STRING_TAG = 4;
qint32 CBerIdentifier::NULL_TAG = 5;
qint32 CBerIdentifier::OBJECT_IDENTIFIER_TAG = 6;
qint32 CBerIdentifier::REAL_TAG = 9;
qint32 CBerIdentifier::ENUMERATED_TAG = 10;
qint32 CBerIdentifier::UTF8_STRING_TAG = 12;
qint32 CBerIdentifier::NUMERIC_STRING_TAG = 18;
qint32 CBerIdentifier::PRqint32ABLE_STRING_TAG = 19;
qint32 CBerIdentifier::TELETEX_STRING_TAG = 20;
qint32 CBerIdentifier::VIDEOTEX_STRING_TAG = 21;
qint32 CBerIdentifier::IA5_STRING_TAG = 22;
qint32 CBerIdentifier::GENERALIZED_TIME_TAG = 24;
qint32 CBerIdentifier::GRAPHIC_STRING_TAG = 25;
qint32 CBerIdentifier::VISIBLE_STRING_TAG = 26;
qint32 CBerIdentifier::GENERAL_STRING_TAG = 27;
qint32 CBerIdentifier::UNIVERSAL_STRING_TAG = 28;
qint32 CBerIdentifier::BMP_STRING_TAG = 30;

qint32 CBerIdentifier::ENUMERATED_TAG = 10;
qint32 CBerIdentifier::UTF8_STRING_TAG = 12;
qint32 CBerIdentifier::NUMERIC_STRING_TAG = 18;
qint32 CBerIdentifier::PRqint32ABLE_STRING_TAG = 19;
qint32 CBerIdentifier::TELETEX_STRING_TAG = 20;
qint32 CBerIdentifier::VIDEOTEX_STRING_TAG = 21;
qint32 CBerIdentifier::IA5_STRING_TAG = 22;
qint32 CBerIdentifier::GENERALIZED_TIME_TAG = 24;
qint32 CBerIdentifier::GRAPHIC_STRING_TAG = 25;
qint32 CBerIdentifier::VISIBLE_STRING_TAG = 26;
qint32 CBerIdentifier::GENERAL_STRING_TAG = 27;
qint32 CBerIdentifier::UNIVERSAL_STRING_TAG = 28;
qint32 CBerIdentifier::BMP_STRING_TAG = 30;

quint8 CBerIdentifier::IDENTIFIER_CLASS_MASK = 0xC0;
quint8 CBerIdentifier::PRIMITIVE_MASK = 0x20;
quint8 CBerIdentifier::TAG_NUMBER_MASK = 0x1F;

void CBerIdentifier::code()
{
	if (m_tagNumber < 31)
	{
		m_identifier.resize(1);
		m_identifier[0] = m_identifierClass | m_primitive | m_tagNumber;
	}
	else
	{
		qint32 tagLength = 1;
		while (m_tagNumber > (qPow(2, (7 * tagLength)) - 1)) {
			tagLength++;
		}

		m_identifier.resize(1 + tagLength);
		m_identifier[0] = m_identifierClass | m_primitive | 31;

		for (int j = 1; j <= (tagLength - 1); j++) {
			m_identifier[j] = (((m_tagNumber >> (7 * (tagLength - j))) & 0xff) | 0x80);
		}

		m_identifier[tagLength] = (m_tagNumber & 0x7f);

	}
}

qint32 CBerIdentifier::encode(CBerByteArrayOutputStream berOStream)
{
	for (qint32 i = m_identifier.size() - 1; i >= 0; i--)
	{
		berOStream.write((quint8) m_identifier[i]);
	}

	return m_identifier.size();
}

qint32 CBerIdentifier::decode(QDataStream iStream)
{

	if (iStream.device()->bytesAvailable() == false) return 0;

	quint8 nextByte;
	nextByte << iStream;

	m_identifierClass = nextByte & IDENTIFIER_CLASS_MASK;
	m_primitive = nextByte & PRIMITIVE_MASK;
	m_tagNumber = nextByte & TAG_NUMBER_MASK;

	int codeLength = 1;

	if (m_tagNumber == 0x1f) {
		m_tagNumber = 0;

		int counter = 0;

		do {
			iStream >> nextByte;
			codeLength++;
			if (counter >= 6) {
				emit signalBERError("CBerIdentifier::decode: Invalid Tag");
			}
			m_tagNumber = m_tagNumber << 7;
			m_tagNumber |= (nextByte & 0x7f);
			counter++;
		} while ((nextByte & 0x80) == 0x80);

	}

	return codeLength;

}

/**
 * Decodes the Identifier from the ByteArrayInputStream and throws an
 * Exception if it is not equal to itself. Returns the number of bytes read
 * from the InputStream.
 */
qint32 CBerIdentifier::decodeAndCheck(QDataStream iStream)
{

	quint8 nextByte;
	for (quint8 myByte : m_identifier) {

		nextByte << iStream;

		if (nextByte != (myByte & 0xff))
		{
			emit signalBERError("CBerIdentifier::decode: Identifier does not match!");
			return 0;
		}
	}

	return m_identifier.size();

}

bool CBerIdentifier::equals(int identifierClass, int primitive, int tagNumber)
{
	return (m_identifierClass == identifierClass && m_primitive == primitive && m_tagNumber == tagNumber);
}

QString CBerIdentifier::toString()
{

	return (QString("identifierClass: %1; primitive: %2; Tag Number: %3").arg(m_identifierClass).arg(m_primitive).arg(m_tagNumber));

}
