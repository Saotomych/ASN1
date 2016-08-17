/*
 * C++/QT Version
 * Open source, 2015
 * Author(s): Alexey Alyoshkin
 *
 * This file is part of ASN1.
 *
 * ASN1 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ASN1 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with ASN1.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef BER_BASE
#define BER_BASE

#include <stdexcept>
#include "asn1_global.h"
#include "berByteArrayOutputStream.h"
#include "berByteArrayInputStream.h"
#include "berIdentifier.h"
#include "berLength.h"
#include "storages/berBaseType.h"

#define START_PROPERTY_INDEX 3

class CBerBaseStorage
{
public:
	quint32 serialize(CBerByteArrayOutputStream& berOStream, QObject* obj, bool explct);
	quint32 deserialize(CBerByteArrayInputStream& iStream, QObject* obj, CBerLength& length, quint32 codeLength, bool explct);

	void runtimeError(QString strErr);
	void argumentWrong(QString strErr);

public:

	static quint32 s_metaTypeId;

//	virtual quint32 encode(CBerByteArrayOutputStream& berOStream, QObject* obj, bool explct);
//	virtual quint32 decode(CBerByteArrayInputStream& iStream, QObject* obj, bool explct);
//
	void encodeAndSave(QObject*, qint32) { }

private:

	enum { ORIGINAL_IDENTIFIER, PARENT_IDENTIFIER, ORIGINAL_IDENTIFIER_WITH_LENGTH, PARENT_IDENTIFIER_WITH_LENGTH, NOT_IDENTIFIED_MODE };

	quint32 getType(QObject* obj, QVariant& varpos0, QVariant& varpos1, QVariant& varpos2);
	QVariant getNextVariant(QObject* obj, QVariant& varposprev, quint32 index);
};

Q_DECLARE_METATYPE(CBerBaseStorage*)

#endif

