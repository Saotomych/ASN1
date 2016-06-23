#include <include/berIdentifier.h>

qint32 CBerIdentifier::UNIVERSAL_CLASS = 0x00;
qint32 CBerIdentifier::APPLICATION_CLASS = 0x40;
qint32 CBerIdentifier::CONTEXT_CLASS = 0x80;
qint32 CBerIdentifier::PRIVATE_CLASS = 0xc0;

qint32 CBerIdentifier::PRIMITIVE = 0x00;
qint32 CBerIdentifier::CONSTRUCTED = 0x20;

qint32 CBerIdentifier::BOOLEAN_TAG = 1;
qint32 CBerIdentifier::INTEGER_TAG = 2;
qint32 CBerIdentifier::BIT_STRING_TAG = 3;
qint32 CBerIdentifier::OCTET_STRING_TAG = 4;
qint32 CBerIdentifier::NULL_TAG = 5;
qint32 CBerIdentifier::OBJECT_IDENTIFIER_TAG = 6;
qint32 CBerIdentifier::REAL_TAG = 9;
qint32 CBerIdentifier::ENUMERATED_TAG = 10;
qint32 CBerIdentifier::UTF8_STRING_TAG = 12;
qint32 CBerIdentifier::NUMERIC_STRING_TAG = 18;
qint32 CBerIdentifier::PRINTABLE_STRING_TAG = 19;
qint32 CBerIdentifier::TELETEX_STRING_TAG = 20;
qint32 CBerIdentifier::VIDEOTEX_STRING_TAG = 21;
qint32 CBerIdentifier::IA5_STRING_TAG = 22;
qint32 CBerIdentifier::GENERALIZED_TIME_TAG = 24;
qint32 CBerIdentifier::GRAPHIC_STRING_TAG = 25;
qint32 CBerIdentifier::VISIBLE_STRING_TAG = 26;
qint32 CBerIdentifier::GENERAL_STRING_TAG = 27;
qint32 CBerIdentifier::UNIVERSAL_STRING_TAG = 28;
qint32 CBerIdentifier::BMP_STRING_TAG = 30;
qint32 CBerIdentifier::PPDU_TAG = 16;

quint8 CBerIdentifier::IDENTIFIER_CLASS_MASK = 0xC0;
quint8 CBerIdentifier::PRIMITIVE_MASK = 0x20;
quint8 CBerIdentifier::TAG_NUMBER_MASK = 0x1F;

CBerIdentifier::CBerIdentifier(): m_IdentifierClass(0), m_Primitive(0), m_TagNumber(0), isExisting(false), isMandatory(false)
{
	code();
}

CBerIdentifier::CBerIdentifier(qint32 identifierClass, qint32 primitive, qint32 tagNumber, bool mandatory):
m_IdentifierClass(identifierClass),
m_Primitive(primitive),
m_TagNumber(tagNumber),
isExisting(true),
isMandatory(mandatory)
{
	code();
}

CBerIdentifier::CBerIdentifier(const CBerIdentifier& rhs): QObject()
{
	isExisting = rhs.isExisting;
	isMandatory = rhs.isMandatory;
	m_Identifier = rhs.m_Identifier;
	m_IdentifierClass = rhs.m_IdentifierClass;
	m_Primitive = rhs.m_Primitive;
	m_TagNumber = rhs.m_TagNumber;
}

CBerIdentifier& CBerIdentifier::operator=(const CBerIdentifier& rhs)
{
	if (this == &rhs) {
		return *this;
	}

	isExisting = rhs.isExisting;
	isMandatory = rhs.isMandatory;
	m_Identifier = rhs.m_Identifier;
	m_IdentifierClass = rhs.m_IdentifierClass;
	m_Primitive = rhs.m_Primitive;
	m_TagNumber = rhs.m_TagNumber;

	return *this;
}

bool CBerIdentifier::operator==(const CBerIdentifier& rhs)
{
	if (isExisting != rhs.isExisting)
		return false;

	if (isMandatory != rhs.isMandatory)
		return false;

	if (m_Identifier != rhs.m_Identifier)
		return false;

	if (m_IdentifierClass != rhs.m_IdentifierClass)
		return false;

	if (m_Primitive != rhs.m_Primitive)
		return false;

	if (m_TagNumber != rhs.m_TagNumber)
		return false;

	return true;
}

bool CBerIdentifier::operator!=(const CBerIdentifier& rhs)
{
	if (this == &rhs) return false;

	return true;
}

void CBerIdentifier::code()
{
	if (m_TagNumber < 31)
	{
		m_Identifier.resize(1);
		m_Identifier[0] = m_IdentifierClass | m_Primitive | m_TagNumber;
	}
	else
	{
		qint32 tagLength = 1;
		while (m_TagNumber > (qPow(2, (7 * tagLength)) - 1)) {
			tagLength++;
		}

		m_Identifier.resize(1 + tagLength);
		m_Identifier[0] = m_IdentifierClass | m_Primitive | 31;

		for (int j = 1; j <= (tagLength - 1); j++) {
			m_Identifier[j] = (((m_TagNumber >> (7 * (tagLength - j))) & 0xff) | 0x80);
		}

		m_Identifier[tagLength] = (m_TagNumber & 0x7f);

	}
}

quint32 CBerIdentifier::encode(CBerByteArrayOutputStream& berOStream)
{
	for (qint32 i = m_Identifier.size() - 1; i >= 0; i--)
	{
		berOStream.write((quint8) m_Identifier[i]);
	}

	return m_Identifier.size();
}

quint32 CBerIdentifier::decode(CBerByteArrayInputStream& iStream)
{

	if (iStream.available() == 0) return 0;

	quint8 nextByte;
	nextByte = iStream.read();

	m_IdentifierClass = nextByte & IDENTIFIER_CLASS_MASK;
	m_Primitive = nextByte & PRIMITIVE_MASK;
	m_TagNumber = nextByte & TAG_NUMBER_MASK;

	int codeLength = 1;

	if (m_TagNumber == 0x1f) {
		m_TagNumber = 0;

		int counter = 0;

		do {
			nextByte = iStream.read();
			codeLength++;
			if (counter >= 6) {
				emit signalBERError("CBerIdentifier::decode: Invalid Tag");
			}
			m_TagNumber = m_TagNumber << 7;
			m_TagNumber |= (nextByte & 0x7f);
			counter++;
		} while ((nextByte & 0x80) == 0x80);

	}

	code();

	return codeLength;

}

/**
 * Decodes the Identifier from the ByteArrayInputStream and throws an
 * Exception if it is not equal to itself. Returns the number of bytes read
 * from the InputStream.
 */
quint32 CBerIdentifier::decodeAndCheck(CBerByteArrayInputStream& iStream)
{

	quint8 nextByte;
	for (quint8 myByte : m_Identifier) {

		nextByte = iStream.read();

		if (nextByte != (myByte & 0xff))
		{
			emit signalBERError("CBerIdentifier::decode: Identifier does not match!");
			return 0;
		}
	}

	return m_Identifier.size();

}

bool CBerIdentifier::equals(int identifierClass, int primitive, int tagNumber)
{
	return (m_IdentifierClass == identifierClass && m_Primitive == primitive && m_TagNumber == tagNumber);
}

QString CBerIdentifier::toString()
{

	return (QString("identifierClass: %1; primitive: %2; Tag Number: %3").arg(m_IdentifierClass).arg(m_Primitive).arg(m_TagNumber));

}
