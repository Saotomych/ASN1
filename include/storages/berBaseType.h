/*
 * berBaseType.h
 *
 *  Created on: 10 нояб. 2015 г.
 *      Author: alexey
 */

#ifndef INCLUDE_STORAGES_BERBASETYPE_H_
#define INCLUDE_STORAGES_BERBASETYPE_H_

#include "asn1_global.h"

class ASN1_SHAREDEXPORT IBerBaseType
{
public:
	virtual quint32 encode(CBerByteArrayOutputStream& berOStream, bool explct)=0;
	virtual quint32 decode(CBerByteArrayInputStream& iStream, bool explct)=0;
public:

	static quint32 s_metaTypeId;
};

Q_DECLARE_METATYPE(IBerBaseType*)

#endif /* INCLUDE_STORAGES_BERBASETYPE_H_ */
