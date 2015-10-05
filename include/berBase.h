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

#include "../../include/berByteArrayOutputStream.h"
#include "../../include/berIdentifier.h"
#include "../../include/berLength.h"
#include "asn1_global.h"

class CBerBase: public QObject
{
	Q_OBJECT

protected:
	CBerIdentifier m_Identifier;
	QByteArray m_Code;

	quint32 encode(CBerByteArrayOutputStream& berOStream, bool explct);
	quint32 decode(QDataStream& iStream, bool explct);

	virtual quint32 serialize(CBerByteArrayOutputStream& berOStream) = 0;
	virtual quint32 deserialize(QDataStream& iStream, CBerLength& length, quint32 codeLength) = 0;

	virtual bool argumentWrong(QString strErr);
	virtual bool runtimeError(QString strErr);

public:

	virtual ~CBerBase() {}

signals:
	void signalDecodeError(QString strErr);
	void signalParameterWrong(QString strErr);

};

#endif

