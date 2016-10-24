/*
 * berRealStorage.cpp
 *
 *  Created on: 3 нояб. 2015 г.
 *      Author: alexey
 */

#include <storages/berRealStorage.h>
#include <berReal.h>

double* CBerRealStorage::ptrValue(QObject* obj, quint32 idx)
{
	QVariant var = obj->metaObject()->property(idx).read(obj);
	qDebug() << var.typeName() << "; " << var.userType() << "; ";
	return qvariant_cast<double*>(var);
}

quint32 CBerRealStorage::serialize(CBerByteArrayOutputStream& berOStream, QObject* obj)
{
	double Real = *ptrValue(obj, 3);

	quint32 codeLength = 0;

	// explained in Annex C and Ch. 8.5 of X.690

	// we use binary encoding, with base 2 and F==0
	// F is only needed when encoding with base 8 or 16

	union
	{
		double dblVal;
		quint64 intVal;
	} reinterpret;

	reinterpret.dblVal = Real;
	quint64 longVal = reinterpret.intVal;

	quint8 sign = 0;
	if (Real < 0) {
		sign = 0x40;
	}
	quint8 exponentFormat = 0;

	quint64 mask = (quint64)(-1l) << 52;
    qint32 exponent = (longVal >> 52) & 0x7ff;
	quint64 mantissa = (longVal & ~mask) | (1l<<52);

	if (exponent == 0x7ff) {

		// Special numbers
		if (sign == 0) {
			berOStream.write((quint8) 0x40);	// PLUS-INFINITY
		}
		else {
			berOStream.write((quint8) 0x41);	// MINUS_INFINITY
		}
		codeLength++;
		codeLength += CBerLength::encodeLength(berOStream, codeLength);
		return codeLength;
	}
	else
	{
		if (!(exponent == 0 && mantissa == (1l<<52)))
		{
			int exponentIncr = 0;
			while (((mantissa >> exponentIncr) & 0xff) == 0x00) {
				exponentIncr += 8;
			}
			while (((mantissa >> exponentIncr) & 0x01) == 0x00) {
				exponentIncr++;
			}

			exponent += exponentIncr - 1075;

			mantissa >>= exponentIncr;

			int mantissaLength = 1;

			// TODO remove -1?
			while (mantissa > (pow(2, 8 * mantissaLength - 1) - 1)) {
				mantissaLength++;
			}
			for (qint8 i = 0; i < mantissaLength; i++) {
				berOStream.write( (quint8) (((mantissa >> (8 * i))) & 0xff) );
			}
			codeLength += mantissaLength;

			quint8 expLength = 1;
			while ( (exponent > (pow(2, 8 * expLength - 1) - 1)
					|| exponent < pow(-2, 8 * expLength - 1)) && expLength < 8)
			{
				expLength++;
			}
			for (qint8 i = 0; i < expLength; i++) {
				berOStream.write( (quint8) (((exponent >> (8 * i))) & 0xff) );
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
	}

	codeLength += CBerLength::encodeLength(berOStream, codeLength);

	return codeLength;
}

quint32 CBerRealStorage::deserialize(CBerByteArrayInputStream& iStream, QObject* obj, CBerLength& length, quint32 codeLength)
{

	length.decode(iStream);
	qDebug() << "CBerRealStorage deserialize, length extracted: " << length.getVal();

	quint32 lenval = length.getVal();

	double real;

	if (lenval < 2) {
		qDebug() << "CBerRealStorage::deserialize: lenval < 2";
		return codeLength;
	}

	if (lenval == 1) {
		qint8 myByte = iStream.read();
		if (myByte == -1)
		{
			qDebug() << "CBerRealStorage::deserialize: length read wrong";
			return codeLength;
		}

		if ( (myByte == 0x40) || (myByte == 0x41))
		{
			real = std::numeric_limits<double>::infinity();
		}
		else
		{
			qDebug() << "CBerRealStorage::deserialize: invalid real encoding";
			return codeLength;
		}
		return codeLength + 1;
	}

	QByteArray byteCode(lenval, Qt::Initialization::Uninitialized);
	if ( (quint32) iStream.read(byteCode, 0, lenval) < lenval)
	{
		qDebug() << "CBerRealStorage::deserialize: data read wrong";
		return codeLength;
	}

	codeLength += lenval;

	quint32 tempLength = 1;

	qint32 sign = 1;
	if ((byteCode[0] & 0x40) == 0x40) {
		sign = -1;
	}

	quint32 exponentLength = (byteCode[0] & 0x03) + 1;
	if (exponentLength == 4) {
		exponentLength = byteCode[1];
		tempLength++;
	}

	tempLength += exponentLength;

	qint32 exponent = 0;
	for (quint32 i = 0; i < exponentLength; i++) {
		exponent |= byteCode[1 + i] << (8 * (exponentLength - i - 1));
	}

	quint64 mantissa = 0;
	quint32 pos = (lenval - tempLength - 1) * 8;
	for (quint32 i = tempLength; i < lenval; ++i, pos-=8)
	{
		mantissa |= (((quint64)(byteCode[i])) << pos) & (0xFFl << pos);
	}

	real = sign * (double)(mantissa * pow(2, exponent));

	double* pVal = &real;
	QVariant wrvar(PtrMetaTypes::s_doublePtrMetaType, &pVal);
	obj->metaObject()->property(3).write(obj, wrvar);

	return codeLength;
}

void CBerRealStorage::encodeAndSave(QObject* obj, qint32 encodingSizeGuess)
{
	CBerByteArrayOutputStream berOStream(encodingSizeGuess);
	CBerReal* pBerReal = reinterpret_cast<CBerReal*>(obj);

	pBerReal->nextEncode(berOStream);
	QByteArray Code = berOStream.getByteArray();

	QVariant wrvar(Code);

	obj->metaObject()->property(2).write(obj, wrvar);

}


