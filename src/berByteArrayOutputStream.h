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

#include "asn1_global.h"

class CBerByteArrayOutputStream: public QObject
{

	Q_OBJECT

	QList<QByteArray> m_buffer;
	qint32 m_index;
	bool m_bAutoResize;

	void resize();

public:

	/**
	 * Creates a <code>BerByteArrayOutputStream</code> with a byte array of size
	 * <code>bufferSize</code>. The buffer will not be resized automatically.
	 * Use {@link #BerByteArrayOutputStream(int, boolean)} instead if you want
	 * the buffer to be dynamically resized.
	 *
	 * @param bufferSize
	 *            the size of the underlying buffer
	 */
	CBerByteArrayOutputStream(int bufferSize);
	CBerByteArrayOutputStream(quint32 bufferSize, bool automaticResize);
	CBerByteArrayOutputStream(QByteArray& buffer, quint32 startingIndex);
	CBerByteArrayOutputStream(QByteArray& buffer, quint32 startingIndex, bool automaticResize);

	bool write(quint32 arg0);

	bool write(quint8 arg0);

	bool write(QByteArray& byteArray);

	QByteArray getByteArray();

signals:
	void signalByteArrayIndexIsOutOfBound(QString strErr);

};
