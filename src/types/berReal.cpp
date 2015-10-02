#include "berReal.h"

CBerReal::CBerReal(): m_Real(0.0)
{
	m_Identifier = s_Identifier;
}

CBerReal::CBerReal(double real)
{
	m_Identifier = s_Identifier;
	m_Real = real;
}

CBerReal::CBerReal(QByteArray& code): m_Real(0.0)
{
	m_Identifier = s_Identifier;
	m_Code = code;
}

quint32 CBerReal::serialize(CBerByteArrayOutputStream& berOStream)
{
	quint32 codeLength = 0;

	// explained in Annex C and Ch. 8.5 of X.690

	// we use binary encoding, with base 2 and F==0
	// F is only needed when encoding with base 8 or 16

	union
	{
		double dblVal;
		quint64 intVal;
	} reinterpret;

	reinterpret.dblVal = m_Real;
	quint64 longVal = reinterpret.intVal;

	quint8 sign = 0;
	if (m_Real < 0) {
		sign = 0x40;
	}
	quint8 exponentFormat = 0;

	qint8 exponent = ((int) (longVal >> 52)) & 0x7ff;
	quint64 mantissa = (longVal & 0x000fffffffffffffL) | 0x0010000000000000L;

	if (exponent == 0x7ff) {
		if (mantissa == 0x0010000000000000L) {
			if (sign == 0) {
				berOStream.write((quint8) 0x40);
			}
			else {
				berOStream.write((quint8) 0x41);
			}
			codeLength++;
		}
		else {
			argumentWrong("CBerReal::serialize: NAN not supported");
		}
	}
	else if (!(exponent == 0 && mantissa == 0x0010000000000000L)) {
		exponent -= 1075;
		int exponentIncr = 0;
		while (((longVal >> exponentIncr) & 0xff) == 0x00) {
			exponentIncr += 8;
		}
		while (((longVal >> exponentIncr) & 0x01) == 0x00) {
			exponentIncr++;
		}

		exponent += exponentIncr;

		mantissa >>= exponentIncr;

		int mantissaLength = 1;

		// TODO remove -1?
		while (mantissa > (pow(2, 8 * mantissaLength - 1) - 1)) {
			mantissaLength++;
		}
		for (qint8 i = 0; i < mantissaLength; i++) {
			berOStream.write( (quint8) (((int) (mantissa >> 8 * (i))) & 0xff) );
		}
		codeLength += mantissaLength;

		quint8 expLength = 1;
		while ( (exponent > (pow(2, 8 * expLength - 1) - 1)
				|| exponent < pow(-2, 8 * expLength - 1)) && expLength < 8)
		{
			expLength++;
		}
		for (qint8 i = 0; i < expLength; i++) {
			berOStream.write( (quint8) (((int) (exponent >> 8 * (i))) & 0xff) );
		}
		codeLength += expLength;

		if (expLength < 4) {
			exponentFormat = (quint8) (expLength - 1);
		}
		else {
			berOStream.write(expLength);
			codeLength++;
			exponentFormat = 0x03;
		}

		berOStream.write( (quint8) (0x80 | sign | exponentFormat) );

		codeLength++;
	}

	codeLength += CBerLength::encodeLength(berOStream, codeLength);

	return codeLength;
}

quint32 CBerReal::deserialize(QDataStream& iStream, CBerLength& length, quint32 codeLength)
{
	bool runtimeValid = true;

	qint32 lenval = length.getVal();

	if (lenval == 0) {
		m_Real = 0;
		return codeLength;
	}

	if (lenval == 1) {
		char myByte;
		quint32 len = 1;
		iStream.readRawData(&myByte, len);

		if ( (myByte == 0x40) || (myByte == 0x41))
		{
			m_Real = std::numeric_limits<double>::infinity();
		}
		else
		{
			runtimeValid = runtimeError("CBerReal::deserialize: invalid real encoding");
		}
		return codeLength + 1;
	}

	char byteCode[lenval];
	if (iStream.readRawData(byteCode, lenval) < lenval)
	{
		runtimeValid = runtimeError("Error Decoding BerInteger");
	}

	codeLength += lenval;

	int tempLength = 1;

	int sign = 1;
	if ((byteCode[0] & 0x40) == 0x40) {
		sign = -1;
	}

	int exponentLength = (byteCode[0] & 0x03) + 1;
	if (exponentLength == 4) {
		exponentLength = byteCode[1];
		tempLength++;
	}

	tempLength += exponentLength;

	int exponent = 0;
	for (int i = 0; i < exponentLength; i++) {
		exponent |= byteCode[1 + i] << (8 * (exponentLength - i - 1));
	}
	int mantissa = 0;
	for (int i = 0; i < lenval - tempLength; i++) {
		mantissa |= byteCode[i + tempLength] << (8 * (lenval - tempLength - i - 1));
	}

	if (runtimeValid == true)
		m_Real = sign * mantissa * pow(2, exponent);

	return codeLength;
}

quint32 CBerReal::encode(CBerByteArrayOutputStream& berOStream, bool explct)
{
	quint32 codeLength = CBerBase::encode(berOStream, explct);

	return codeLength;
}

quint32 CBerReal::decode(QDataStream& iStream, bool explct)
{
	int codeLength =  CBerBase::decode(iStream, explct);

	return codeLength;
}

void CBerReal::encodeAndSave(qint32 encodingSizeGuess)
{
	CBerByteArrayOutputStream berOStream(encodingSizeGuess);
	encode(berOStream, false);
	m_Code = berOStream.getByteArray();
}
