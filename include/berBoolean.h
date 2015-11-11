/*
 * C++/QT Version
 * Open source, 2015
 * Author(s): Alexey Alyoshkin
 *
 * Based on Java version
 * Copyright Fraunhofer ISE, 2011
 * Author(s): Stefan Feuerhahn
 *
 * This file is part of jASN1.
 * For more information visit http://www.openmuc.org
 *
 * jASN1 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * jASN1 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with jASN1.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef BER_BOOLEAN
#define BER_BOOLEAN

#include "berIdentifier.h"
#include "berLength.h"
#include "storages/berBooleanStorage.h"
#include "storages/berBaseType.h"
#include "storages/decoder.h"

class ASN1_SHAREDEXPORT CBerBoolean: public QObject, public IBerBaseType
{
	Q_OBJECT
	Q_PROPERTY(CBerIdentifier* Identifier READ getIdentifier)
	Q_PROPERTY(QByteArray* Code READ getCode)
	Q_PROPERTY(bool* Value READ getValue WRITE setValue)

protected:
	CBerIdentifier m_Identifier;
	QByteArray m_Code;
	bool m_Val;

	void setValue(bool* val) { m_Val = *val; }

public:

	ASN1_CODEC(CBerBooleanStorage)

	static CBerIdentifier s_Identifier;
	static quint32 s_metaTypeId;

	CBerBoolean();
	CBerBoolean(bool val);
	CBerBoolean(QByteArray code);
	CBerBoolean(const CBerBoolean& rhs);
	virtual ~CBerBoolean() {}

	CBerBoolean& operator=(const CBerBoolean& rhs);
	bool operator!=(const CBerBoolean& rhs);

	CBerIdentifier* getIdentifier() { return &m_Identifier; }
	QByteArray* getCode() { return &m_Code; }
	bool* getValue() {return &m_Val;}
};

Q_DECLARE_METATYPE(CBerBoolean*)

#endif
