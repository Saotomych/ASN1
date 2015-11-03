/*
 * compositeValue.h
 *
 *  Created on: 27 окт. 2015 г.
 *      Author: alexey
 */

#ifndef INCLUDE_COMPOSITEVALUE_H_
#define INCLUDE_COMPOSITEVALUE_H_

#include "compositeSerializer.h"
#include "berByteArrayOutputStream.h"
#include "berIdentifier.h"
#include "berLength.h"
#include "berBitString.h"
#include "berVisibleString.h"
#include "berInteger.h"

class ASN1_SHAREDEXPORT CCompositeValue: public QObject
{
	Q_OBJECT
	Q_PROPERTY(CBerIdentifier Identifier MEMBER m_Identifier)
	Q_PROPERTY(QByteArray Code MEMBER m_Code)
	Q_PROPERTY(CBerInteger Integer MEMBER m_integer)
	Q_PROPERTY(CBerBitString BitString MEMBER m_bitString)
	Q_PROPERTY(CBerVisibleString VisibleString MEMBER m_visibleString)

protected:

	CBerIdentifier m_Identifier;
	QByteArray m_Code;
	CBerInteger m_integer;
	CBerBitString m_bitString;
	CBerVisibleString m_visibleString;

public:

	static CBerIdentifier s_Identifier;
	static quint32 s_metaTypeIdentifier;

	CCompositeValue():
		QObject(),
		m_integer(0)
	{}

	CCompositeValue(CBerInteger& berInt, CBerBitString berBitStr, CBerVisibleString berVisStr):
		QObject(),
		m_integer(berInt),
		m_bitString(berBitStr),
		m_visibleString(berVisStr)
	{}

	CCompositeValue(const CCompositeValue& rhs): QObject()
	{
		m_integer = rhs.m_integer;
		m_bitString = rhs.m_bitString;
		m_visibleString = rhs.m_visibleString;
	}

	CCompositeValue& operator=(const CCompositeValue& rhs)
	{
		if (this == &rhs) return *this;
		m_integer = rhs.m_integer;
		m_bitString = rhs.m_bitString;
		m_visibleString = rhs.m_visibleString;

		return *this;
	}

	virtual ~CCompositeValue() {}
};

Q_DECLARE_METATYPE(CCompositeValue)

#endif /* INCLUDE_COMPOSITEVALUE_H_ */
