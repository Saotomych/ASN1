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

#ifndef BER_ANY
#define BER_ANY

#include "berByteArrayOutputStream.h"
#include "berBase.h"

class  ASN1_SHAREDEXPORT CBerAny: public QObject, public CBerBaseStorage
{
	Q_OBJECT
	Q_PROPERTY(CBerIdentifier Identifier MEMBER m_Identifier)
	Q_PROPERTY(QByteArray Code MEMBER m_Code)
	Q_PROPERTY(qint64 Length MEMBER m_Length)

protected:
	CBerIdentifier m_Identifier;
	QByteArray m_Code;
	qint64 m_Length;

public:
	static CBerIdentifier s_Identifier;
	static quint32 s_metaTypeId;

	CBerAny(): m_Length(0)
	{}

	CBerAny(quint32 length): m_Length(length)
	{}

	virtual ~CBerAny() {}

	virtual qint32 encode(CBerByteArrayOutputStream& berOStream, QObject* obj, bool explct)
	{
		return m_Length;
	}

	virtual qint32 decode(CBerByteArrayInputStream& iStream, QObject* obj, bool explct)
	{
		return m_Length;
	}

};

Q_DECLARE_METATYPE(CBerAny)

#endif BER_ANY
