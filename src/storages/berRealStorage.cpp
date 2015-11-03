/*
 * berRealStorage.cpp
 *
 *  Created on: 3 нояб. 2015 г.
 *      Author: alexey
 */

#include "storages/berRealStorage.h"

double* CBerRealStorage::ptrValue(QObject* obj, quint32 idx)
{
	QVariant var = obj->metaObject()->property(idx).read(obj);
	qDebug() << var.typeName() << "; " << var.userType() << "; ";
	return const_cast<double*> (reinterpret_cast<const double*> (var.constData()));
}

quint32 CBerRealStorage::serialize(CBerByteArrayOutputStream& berOStream, QObject* obj, bool explct)
{
	double* pReal = ptrValue(obj, 3);

	quint32 codeLength = 0;

	// explained in Annex C and Ch. 8.5 of X.690

	// we use binary encoding, with base 2 and F==0
	// F is only needed when encoding with base 8 or 16

	union
	{
		double dblVal;
		quint64 intVal;
	} reinterpret;

	reinterpret.dblVal = *pReal;
	quint64 longVal = reinterpret.intVal;

	quint8 sign = 0;
	if (*pReal < 0) {
		sign = 0x40;
	}
	quint8 exponentFormat = 0;

	qint32 exponent = ((qint32) (longVal >> 52)) & 0x7ff;
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
			argumentWrong("CBerRealStorage::serialize: NAN not supported");
			return 0;
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

quint32 CBerRealStorage::deserialize(CBerByteArrayInputStream& iStream, QObject* obj, CBerLength& length, quint32 codeLength, bool explct)
{
	qint32 lenval = length.getVal();

	double real;

	if (lenval == 0) {
		real = 0;
		return codeLength;
	}

	if (lenval == 1) {
		qint8 myByte = iStream.read();
		if (myByte == -1)
		{
			runtimeError("CBerRealStorage::deserialize: length read wrong");
			return codeLength;
		}

		if ( (myByte == 0x40) || (myByte == 0x41))
		{
			real = std::numeric_limits<double>::infinity();
		}
		else
		{
			runtimeError("CBerRealStorage::deserialize: invalid real encoding");
			return codeLength;
		}
		return codeLength + 1;
	}

	QByteArray byteCode(lenval, Qt::Initialization::Uninitialized);
	if (iStream.read(byteCode, 0, lenval) < lenval)
	{
		runtimeError("CBerRealStorage::deserialize: data read wrong");
		return codeLength;
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

	real = sign * mantissa * pow(2, exponent);

	QVariant wrvar(real);
	obj->metaObject()->property(3).write(obj, wrvar);

	return codeLength;
}

quint32 CBerRealStorage::encode(CBerByteArrayOutputStream& berOStream, QObject* obj, bool explct)
{
	quint32 codeLength = CBerBaseStorage::encode(berOStream, obj, explct);

	return codeLength;
}

quint32 CBerRealStorage::decode(CBerByteArrayInputStream& iStream, QObject* obj, bool explct)
{
	int codeLength =  CBerBaseStorage::decode(iStream, obj, explct);

	return codeLength;
}

void CBerRealStorage::encodeAndSave(QObject* obj, qint32 encodingSizeGuess)
{
	CBerByteArrayOutputStream berOStream(encodingSizeGuess);
	CBerReal* pBerReal = reinterpret_cast<CBerReal*>(obj);

	pBerReal->encode(berOStream, obj, false);
	QByteArray Code = berOStream.getByteArray();

	QVariant wrvar(Code);

	obj->metaObject()->property(2).write(obj, wrvar);

}


