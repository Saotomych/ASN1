/*
 * berBaseType.h
 *
 *  Created on: 10 нояб. 2015 г.
 *      Author: alexey
 */

#ifndef INCLUDE_STORAGES_BERBASETYPE_H_
#define INCLUDE_STORAGES_BERBASETYPE_H_

#include <asn1_global.h>
#include <berIdentifier.h>

class ASN1_SHAREDEXPORT IBerBaseType
{

public:
	virtual quint32 startEncode(CBerByteArrayOutputStream& berOStream)=0;
	virtual quint32 nextEncode(CBerByteArrayOutputStream& berOStream)=0;
	virtual quint32 startDecode(CBerByteArrayInputStream& iStream)=0;
	virtual quint32 nextDecode(CBerByteArrayInputStream& iStream)=0;
	virtual IBerBaseType* createMember(CBerIdentifier&)=0;
    virtual CBerIdentifier getIdentifier()=0;
    virtual QString getString() { return QString(); }

	virtual ~IBerBaseType() { }

	virtual bool hasLength()
	{
		return true;
	}

	static quint32 s_metaTypeId;
};

Q_DECLARE_METATYPE(IBerBaseType*)

#endif /* INCLUDE_STORAGES_BERBASETYPE_H_ */
