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

enum class WorkType { PARENT_IDENTIFIER, PARENT_IDENTIFIER_WITH_LENGTH, NOT_IDENTIFIED_MODE };

class CBerBaseStorage
{
public:
	quint32 serialize(CBerByteArrayOutputStream& berOStream, QObject* obj);
	quint32 deserialize(CBerByteArrayInputStream& iStream, QObject* obj, CBerLength& length, quint32 codeLength);

	void runtimeError(QString strErr);
	void argumentWrong(QString strErr);

public:

	static quint32 s_metaTypeId;

	void encodeAndSave(QObject*, qint32) { }

private:

public:
	WorkType getWorkType(QObject* obj, QVariant& varpos0, QVariant& varpos1, QVariant& varpos2);
	QVariant getNextVariant(QObject* obj, QVariant& varposprev, quint32 index);
};

Q_DECLARE_METATYPE(CBerBaseStorage*)

#endif

