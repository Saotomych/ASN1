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

#ifndef BER_REAL
#define BER_REAL

#include "berIdentifier.h"
#include "berLength.h"
#include "storages/berRealStorage.h"
#include "storages/berBaseType.h"
#include "storages/decoder.h"

class ASN1_SHAREDEXPORT CBerReal: public QObject, public IBerBaseType
{
	Q_OBJECT
	Q_PROPERTY(CBerIdentifier Identifier READ getIdentifier)
	Q_PROPERTY(QByteArray* Code READ getCode)
	Q_PROPERTY(double* Real READ getValue WRITE setValue)

protected:
	CBerIdentifier m_Identifier;
	QByteArray m_Code;
	double m_Real;

	void setValue(double* pVal) { m_Real = *pVal; }

public:

	ASN1_CODEC(CBerRealStorage)

	static CBerIdentifier s_Identifier;
	static quint32 s_metaTypeId;

	CBerReal();
	CBerReal(double real);
	CBerReal(QByteArray& code);
	CBerReal(const CBerReal& rhs);
	CBerReal& operator=(const CBerReal& rhs);
	bool operator!=(const CBerReal& rhs);

	virtual ~CBerReal() {}

	double* getValue() { return &m_Real; }
	QByteArray* getCode() { return &m_Code; }
	CBerIdentifier getIdentifier() { return m_Identifier; }
};

Q_DECLARE_METATYPE(CBerReal*)

#endif
