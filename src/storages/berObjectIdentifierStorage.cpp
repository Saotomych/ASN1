/*
 * berObjectIdentifierStorage.cpp
 *
 *  Created on: 3 нояб. 2015 г.
 *      Author: alexey
 */

#include "storages/berObjectIdentifierStorage.h"
#include "berObjectIdentifier.h"

QVector<qint32>* CBerObjectIdentifierStorage::ptrValue(QObject* obj, quint32 idx)
{
	QVariant var = obj->metaObject()->property(idx).read(obj);
	qDebug() << var.typeName() << "; " << var.userType() << "; ";
	return qvariant_cast<QVector<qint32>* > (var);
}

quint32 CBerObjectIdentifierStorage::serialize(CBerByteArrayOutputStream& berOStream, QObject* obj, bool explct)
{
	QVector<qint32> OIC = *(ptrValue(obj, 3));

	if ( OIC.size() < 2)
	{
		qDebug() << "ERROR! CBerObjectIdentifierStorage::serialize: Object identifier size < 2";
		return 0;
	}

	quint32 firstSubidentifier = 40 * OIC[0] + OIC[1];

	quint32 subidentifier;

	quint32 codeLength = 0;

	for (int i = (OIC.size() - 1); i > 0; i--) {

		if (i == 1) {
			subidentifier = firstSubidentifier;
		}
		else {
			subidentifier = OIC[i];
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

quint32 CBerObjectIdentifierStorage::deserialize(CBerByteArrayInputStream& iStream, QObject* obj, CBerLength& length, quint32 codeLength)
{
	length.decode(iStream);
	qDebug() << "CBerObjectIdentifierStorage::deserialize, length extracted: " << length.getVal();

	qint32 lenval = length.getVal();

	QVector<qint32> objectIdentifierComponents;

	if (lenval == 0) {
		QVariant wrvar(PtrMetaTypes::s_QVectorQint32PtrMetaType, (const void*) &objectIdentifierComponents);
		obj->metaObject()->property(3).write(obj, wrvar);
		return codeLength;
	}

	QByteArray byteCode(lenval, Qt::Initialization::Uninitialized);
	if (iStream.read(byteCode, 0, lenval) == -1) {
		return codeLength;
	}

	codeLength += lenval;

	int subIDEndIndex = 0;
	while ((byteCode[subIDEndIndex] & 0x80) == 0x80) {
		if (subIDEndIndex >= (lenval - 1)) {
//			runtimeError("CBerObjectIdentifierStorage::deserialize: Invalid Object Identifier");
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

	QVector<qint32>* pOIC = &objectIdentifierComponents;
	QVariant wrvar(PtrMetaTypes::s_QVectorQint32PtrMetaType, &pOIC );
	obj->metaObject()->property(3).write(obj, wrvar);

	return codeLength;
}

QString CBerObjectIdentifierStorage::toString(QObject* obj)
{
	QString objIDString("");
	QVector<qint32>* pOIC = ptrValue(obj, 3);

	if (pOIC->size() == 0) return objIDString;

	objIDString += QString("%1").arg((*pOIC)[0]);
	for (qint32 i = 0; i < pOIC->size(); ++i)
	{
		objIDString += QString(".%1").arg((*pOIC)[i]);
	}

	return objIDString;
}



