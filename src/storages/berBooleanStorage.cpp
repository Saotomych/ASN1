/*
 * berBoolean.cpp
 *
 *  Created on: 3 нояб. 2015 г.
 *      Author: alexey
 */

#include "storages/berBooleanStorage.h"
#include "berBoolean.h"

bool* CBerBooleanStorage::ptrValue(QObject* obj, quint32 idx)
{
	QVariant var = obj->metaObject()->property(idx).read(obj);
	qDebug() << var.typeName() << "; " << var.userType() << "; ";
	return qvariant_cast <bool*> (var);
}

quint32 CBerBooleanStorage::serialize(CBerByteArrayOutputStream& berOStream, QObject* obj, bool explct)
{
	bool Bool = *ptrValue(obj, 3);

	quint32 codeLength = 1;

	quint8 data = (Bool == false) ? 0 : 0xFF;
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
//			runtimeError("CBerBooleanStorage::deserialize: error reading");
			return codeLength;
		}

		codeLength++;

		bool val = (data == 0) ? false : true;
		bool* pVal = &val;
		QVariant wrvar(PtrMetaTypes::s_boolPtrMetaType, &pVal);
		obj->metaObject()->property(3).write(obj, wrvar);
	}
	else
	{
//		runtimeError("CBerBooleanStorage::deserialize: length read wrong");
	}

	return codeLength;
}

void CBerBooleanStorage::encodeAndSave(QObject* obj, qint32 encodingSizeGuess)
{
	CBerBoolean* pBerBoolean = reinterpret_cast<CBerBoolean*>(obj);

	CBerByteArrayOutputStream berOStream(encodingSizeGuess);

	pBerBoolean->encode(berOStream, false);
	QByteArray Code = berOStream.getByteArray();

	QVariant wrvar(Code);

	obj->metaObject()->property(2).write(obj, wrvar);

}


