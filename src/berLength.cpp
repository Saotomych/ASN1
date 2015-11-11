#include <include/berLength.h>

qint32 CBerLength::encodeLength(CBerByteArrayOutputStream& berOStream, quint8 length)
{

	if (length <= 127) {
		// this is the short form, it is coded differently than the long
		// form for values > 127
		berOStream.write(length);
		return 1;
	}
	else
	{
		quint8 numLengthBytes = 1;

		while (((qint32) (qPow(2, 8 * numLengthBytes) - 1)) < length) {
			numLengthBytes++;
		}

		for (int i = 0; i < numLengthBytes; i++) {
			berOStream.write( (quint8) ((length >> 8 * i) & 0xff) );
		}

		berOStream.write((quint8)(0x80 | numLengthBytes));

		return 1 + numLengthBytes;
	}

}

qint32 CBerLength::decode(CBerByteArrayInputStream& iStream)
{
	if (iStream.available() == 0) return 0;

	m_ValLength = (quint8) iStream.read();

	quint32 length = 1;

	if ((m_ValLength & 0x80) != 0)
	{
		quint32 lengthLength = m_ValLength & 0x7f;

		if (lengthLength == 0) {
			m_ValLength = -1;
			return 1;
		}

		if (lengthLength > 4) {
			emit signalIOError("CBerLength::decode: Length is out of bound!");
			return 1;
		}

		m_ValLength = 0;
		QByteArray byteCode(lengthLength, Qt::Initialization::Uninitialized);

		if (iStream.read(byteCode, 0, lengthLength) == -1) {
			emit signalIOError("CBerLength::decode: Error Decoding ASN1 Integer");
			return 1;
		}

		length += lengthLength;

		for (quint8 i = 0; i < lengthLength; i++)
		{
			m_ValLength |= (byteCode[i] & 0xff) << (8 * (lengthLength - i - 1));
		}

	}

	return length;

}

quint32 CBerLength::getVal()
{
	return m_ValLength;
}
