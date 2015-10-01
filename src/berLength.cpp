#include "src/berLength.h"

qint32 CBerLength::encodeLength(CBerByteArrayOutputStream& berOStream, qint32 length)
{

	if (length <= 127) {
		// this is the short form, it is coded differently than the long
		// form for values > 127
		berOStream.write((quint8) length);
		return 1;
	}
	else {
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

qint32 CBerLength::decode(QDataStream& iStream)
{
	if (iStream.device()->bytesAvailable() == 0) return 0;

	iStream >> m_ValLength;

	qint32 length = 1;

	if ((m_ValLength & 0x80) != 0)
	{
		qint32 lengthLength = m_ValLength & 0x7f;

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

		char *pBuffer = byteCode.d->data();

		if (iStream.readRawData(pBuffer, lengthLength) == -1) {
			emit signalIOError("CBerLength::decode: Error Decoding ASN1Integer");
			return 1;
		}

		length += lengthLength;

		for (int i = 0; i < lengthLength; i++)
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
