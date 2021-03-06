/*
 * berOctetStringStorage.cpp
 *
 *  Created on: 3 нояб. 2015 г.
 *      Author: alexey
 */

#include <storages/berOctetStringStorage.h>

QByteArray* CBerOctetStringStorage::ptrValue(QObject* obj, quint32 idx)
{
	QVariant var = obj->metaObject()->property(idx).read(obj);
	qDebug() << var.typeName() << "; " << var.userType() << "; ";
	return qvariant_cast<QByteArray*> (var);
}

quint32 CBerOctetStringStorage::serialize(CBerByteArrayOutputStream& berOStream, QObject* obj)
{
	QByteArray berOS = *ptrValue(obj, 3);

	berOStream.write(berOS);

	quint32 codeLength = berOS.size();
	codeLength += CBerLength::encodeLength(berOStream, codeLength);

	return codeLength;
}

quint32 CBerOctetStringStorage::deserialize(CBerByteArrayInputStream& iStream, QObject* obj, CBerLength& length, quint32 codeLength)
{
	length.decode(iStream);
	qDebug() << "CBerOctetStringStorage deserialize, length extracted: " << length.getVal();

	qint32 lenval = length.getVal();

	if (lenval != 0)
	{
		QByteArray data(lenval, Qt::Initialization::Uninitialized);
		if (iStream.read(data, 0, lenval) < lenval)
		{
			return codeLength;
		}

		QByteArray val;
		val.append(data, sizeof(data) / sizeof(data[0]) );

		QByteArray* pVal = &val;
		QVariant wrvar(PtrMetaTypes::s_QByteArrayPtrMetaType, &pVal);
		obj->metaObject()->property(3).write(obj, wrvar);

		codeLength += lenval;
	}

	return codeLength;

}
