#include "berObjectIdentifier.h"

CBerObjectIdentifier::CBerObjectIdentifier()
{
	m_Identifier = s_Identifier;
}

CBerObjectIdentifier::CBerObjectIdentifier(QVector<qint32>& objectIdentifierComponents)
{
	bool argumentValid = true;

	m_Identifier = s_Identifier;

	if ( objectIdentifierComponents.size() < 2 )
		argumentValid = argumentWrong("CBerObjectIdentifier::parameterWrong in constructor");

	if ( ( objectIdentifierComponents[0] == 0 || objectIdentifierComponents[1] == 1) && ( objectIdentifierComponents[1] > 39) )
		argumentValid = argumentWrong("CBerObjectIdentifier::parameterWrong in constructor");

	if ( objectIdentifierComponents[0] > 2 )
		argumentValid = argumentWrong("CBerObjectIdentifier::parameterWrong in constructor");

	for (auto objectIdentifierComponent: objectIdentifierComponents)
	{
		if (objectIdentifierComponent < 0)
		{
			argumentValid = argumentWrong("CBerObjectIdentifier::parameterWrong in constructor");
			break;
		}
	}

	if (argumentValid == true)
		m_ObjectIdentifierComponents = objectIdentifierComponents;
}

CBerObjectIdentifier::CBerObjectIdentifier(QByteArray& code)
{
	m_Identifier = s_Identifier;
	m_Code = code;
}

quint32 CBerObjectIdentifier::serialize(CBerByteArrayOutputStream& berOStream)
{
	quint32 firstSubidentifier = 40 * m_ObjectIdentifierComponents[0] + m_ObjectIdentifierComponents[1];

	quint32 subidentifier;

	quint32 codeLength = 0;

	for (int i = (m_ObjectIdentifierComponents.length - 1); i > 0; i--) {

		if (i == 1) {
			subidentifier = firstSubidentifier;
		}
		else {
			subidentifier = m_ObjectIdentifierComponents[i];
		}

		// get length of subidentifier
		int subIDLength = 1;
		while (subidentifier > (pow(2, (7 * subIDLength)) - 1)) {
			subIDLength++;
		}

		berOStream.write(subidentifier & 0x7f);

		for (int j = 1; j <= (subIDLength - 1); j++) {
			berOStream.write(((subidentifier >> (7 * j)) & 0xff) | 0x80);
		}

		codeLength += subIDLength;
	}

	codeLength += CBerLength::encodeLength(berOStream, codeLength);

	return codeLength;
}

quint32 CBerObjectIdentifier::deserialize(QDataStream& iStream, CBerLength& length, quint32 codeLength)
{
	quint32 lenval = length.getVal();

	if (lenval == 0) {
		m_ObjectIdentifierComponents.clear();
		return codeLength;
	}

	char byteCode[lenval];
	if (iStream.readRawData(byteCode, lenval) == -1) {
		runtimeError("CBerObjectIdentifier::deserialize: Read wrong");
		return codeLength;
	}

	codeLength += lenval;

	QVector<qint32> objectIdentifierComponents;

	int subIDEndIndex = 0;
	while ((byteCode[subIDEndIndex] & 0x80) == 0x80) {
		if (subIDEndIndex >= (lenval - 1)) {
			runtimeError("CBerObjectIdentifier::deserialize: Invalid Object Identifier");
			return codeLength;
		}
		subIDEndIndex++;
	}

	int subidentifier = 0;
	for (int i = 0; i <= subIDEndIndex; i++) {
		subidentifier |= (byteCode[i] << ((subIDEndIndex - i) * 7));
	}

	if (subidentifier < 40) {
		objectIdentifierComponents.push_back(0);
		objectIdentifierComponents.push_back(subidentifier);
	}
	else if (subidentifier < 80) {
		objectIdentifierComponents.push_back(1);
		objectIdentifierComponents.push_back(subidentifier - 40);
	}
	else {
		objectIdentifierComponents.push_back(2);
		objectIdentifierComponents.push_back(subidentifier - 80);
	}

	subIDEndIndex++;

	while (subIDEndIndex < lenval) {
		int subIDStartIndex = subIDEndIndex;

		while ((byteCode[subIDEndIndex] & 0x80) == 0x80) {
			if (subIDEndIndex == (lenval - 1)) {
				runtimeError("Invalid Object Identifier");
				return codeLength;
			}
			subIDEndIndex++;
		}
		subidentifier = 0;
		for (int j = subIDStartIndex; j <= subIDEndIndex; j++) {
			subidentifier |= ((byteCode[j] & 0x7f) << ((subIDEndIndex - j) * 7));
		}
		objectIdentifierComponents.push_back(subidentifier);
		subIDEndIndex++;
	}

	m_ObjectIdentifierComponents = objectIdentifierComponents;

	return codeLength;
}

quint32 CBerObjectIdentifier::encode(CBerByteArrayOutputStream& berOStream, bool explct)
{
	quint32 codeLength = CBerBase::encode(berOStream, explct);

	return codeLength;
}

quint32 CBerObjectIdentifier::decode(QDataStream& iStream, bool explct)
{
	int codeLength =  CBerBase::decode(iStream, explct);

	return codeLength;
}

QString CBerObjectIdentifier::toString()
{
	QString objIDString("");

	if (m_ObjectIdentifierComponents.size() == 0) return objIDString;

	objIDString += QString("%1").arg(m_ObjectIdentifierComponents[0]);
	for (qint32 i = 0; i < m_ObjectIdentifierComponents.size(); ++i)
	{
		objIDString += QString(".%1").arg(m_ObjectIdentifierComponents[i]);
	}

	return objIDString;
}
