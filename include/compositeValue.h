/*
 * compositeValue.h
 *
 *  Created on: 27 окт. 2015 г.
 *      Author: alexey
 */

#ifndef INCLUDE_COMPOSITEVALUE_H_
#define INCLUDE_COMPOSITEVALUE_H_

#include "berByteArrayOutputStream.h"
#include "berIdentifier.h"
#include "berLength.h"
#include "berBitString.h"
#include "berVisibleString.h"
#include "berInteger.h"
#include "storages/berBaseType.h"
#include "storages/decoder.h"

class ASN1_SHAREDEXPORT CCompositeValue: public QObject, public IBerBaseType
{
	Q_OBJECT
	Q_PROPERTY(CBerIdentifier Identifier READ getIdentifier)
	Q_PROPERTY(QByteArray* Code READ getCode)
	Q_PROPERTY(IBerBaseType* Integer READ getValueName1)
	Q_PROPERTY(IBerBaseType* BitString READ getValueName2)
	Q_PROPERTY(IBerBaseType* VisibleString READ getValueName3)

	IBerBaseType* getValueName1() { return &m_integer; }
	IBerBaseType* getValueName2() { return &m_bitString; }
	IBerBaseType* getValueName3() { return &m_visibleString; }

protected:

	CBerIdentifier m_Identifier;
	QByteArray m_Code;

	CBerInteger m_integer;
	CBerBitString m_bitString;
	CBerVisibleString m_visibleString;

	inline IBerBaseType* create_object_by_id(const CBerIdentifier&)
	{
		qDebug() << "INFO: CCompositeValue has members already.";
		return nullptr;
	}

public:

	ASN1_CODEC(CBerBaseStorage)

	static quint32 s_metaTypeIdentifier;

	static CBerIdentifier getBerIdentifier()
	{
		return CBerIdentifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::CONSTRUCTED, CBerIdentifier::PPDU_TAG);
	}

	CCompositeValue():
		m_Identifier(getBerIdentifier()),
		m_integer(0)
	{
	}

	CCompositeValue(CBerInteger& berInt, CBerBitString& berBitStr, CBerVisibleString& berVisStr):
		m_Identifier(getBerIdentifier()),
		m_integer(berInt),
		m_bitString(berBitStr),
		m_visibleString(berVisStr)
	{
	}

	CCompositeValue(const CCompositeValue& rhs): QObject()
	{
		m_Identifier = rhs.m_Identifier;
		m_Code = rhs.m_Code;
		m_integer = rhs.m_integer;
		m_bitString = rhs.m_bitString;
		m_visibleString = rhs.m_visibleString;
	}

	CCompositeValue& operator=(const CCompositeValue& rhs)
	{
		if (this == &rhs) return *this;

		m_Identifier = rhs.m_Identifier;
		m_Code = rhs.m_Code;
		m_integer = rhs.m_integer;
		m_bitString = rhs.m_bitString;
		m_visibleString = rhs.m_visibleString;

		return *this;
	}

	virtual ~CCompositeValue() {}

	QByteArray* getCode() { return &m_Code; }
	CBerIdentifier getIdentifier() { return m_Identifier; }
	CBerInteger* getValueNameInt() { return &m_integer; }
	CBerBitString* getValueNameBitStr() { return &m_bitString; }
	CBerVisibleString* getValueNameVisStr() { return &m_visibleString; }
};

Q_DECLARE_METATYPE(CCompositeValue*)

#endif /* INCLUDE_COMPOSITEVALUE_H_ */
