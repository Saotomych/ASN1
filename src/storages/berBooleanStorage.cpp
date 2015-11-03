/*
 * berBoolean.cpp
 *
 *  Created on: 3 нояб. 2015 г.
 *      Author: alexey
 */

#include "storages/berBooleanStorage.h"

bool* CBerBooleanStorage::ptrValue(QObject* obj, quint32 idx)
{
	QVariant var = obj->metaObject()->property(idx).read(obj);
	qDebug() << var.typeName() << "; " << var.userType() << "; ";
	return const_cast<bool*> (reinterpret_cast<const bool*> (var.constData()));
}

quint32 CBerBooleanStorage::serialize(CBerByteArrayOutputStream& berOStream, QObject* obj, bool explct)
{
	bool* pBool = ptrValue(obj, 3);

	quint32 codeLength = 1;

	quint8 data = (*pBool == false) ? 0 : 0xFF;
	berOStream.write(data);

	return codeLength;
}

quint32 CBerBooleanStorage::deserialize(CBerByteArrayInputStream& iStream, QObject* obj, CBerLength& length, quint32 codeLength, bool explct)
{
	if (length.getVal() == 1)
	{
		qint32 data = iStream.read();
		if (data == -1)
		{
			runtimeError("CBerBooleanStorage::deserialize: error reading");
			return codeLength;
		}

		codeLength++;

		bool val = (data == 0) ? false : true;

		QVariant wrvar(val);
		obj->metaObject()->property(3).write(obj, wrvar);
	}
	else
	{
		runtimeError("CBerBooleanStorage::deserialize: length read wrong");
	}

	return codeLength;
}

quint32 CBerBooleanStorage::encode(CBerByteArrayOutputStream& berOStream, QObject* obj, bool explct)
{
	quint32 codeLength = CBerBaseStorage::encode(berOStream, obj, explct);

	return codeLength;
}

quint32 CBerBooleanStorage::decode(CBerByteArrayInputStream& iStream, QObject* obj, bool explct)
{
	int codeLength =  CBerBaseStorage::decode(iStream, obj, explct);

	return codeLength;
}

void CBerBooleanStorage::encodeAndSave(QObject* obj, qint32 encodingSizeGuess)
{
	CBerBoolean* pBerBoolean = reinterpret_cast<CBerBoolean*>(obj);

	CBerByteArrayOutputStream berOStream(encodingSizeGuess);

	pBerBoolean->encode(berOStream, obj, false);
	QByteArray Code = berOStream.getByteArray();

	QVariant wrvar(Code);

	obj->metaObject()->property(2).write(obj, wrvar);

}



