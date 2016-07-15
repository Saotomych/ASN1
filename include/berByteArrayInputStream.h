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

#ifndef BER_BYTEARRAYINPUTSTREAM
#define BER_BYTEARRAYINPUTSTREAM

#include "asn1_global.h"

class ASN1_SHAREDEXPORT CBerByteArrayInputStream: public QObject
{

	Q_OBJECT

	/**
	 * An byte array that was provided by the creator of the stream.
	 */
	QByteArray m_Buffer;

	/**
	 * The index of next character to read from the input stream buffer.
	 */
	qint32 m_Pos;

	/**
	 * The currently marked position.
	 * It enables to return m_Pos to already reading position
	 * with reset() function.
	 */
	qint32 m_Mark;

	void runtimeError(QString str);
	qint32 checkPosition();

public:

	/**
	 * Creates a <code>CBerByteArrayInputStream</code>
	 * so that it uses <code>buffer</code> as it's buffer array.
	 *
	 * @param buffer the input buffer
	 */
	CBerByteArrayInputStream(QByteArray& buffer);

	/**
	 * Creates a <code>CBerByteArrayInputStream</code>
	 * so that it uses <code>buffer</code> as it's buffer array.
	 *
	 * @param buffer the input buffer
	 */
	CBerByteArrayInputStream(QDataStream& buffer);

	/**
	 * Creates a <code>CBerByteArrayInputStream</code>
	 * so that it uses <code>buffer</code> as it's buffer array.
	 *
	 * @param buffer the input buffer
	 * @param startingIndex the starting offset for reading
	 */
	CBerByteArrayInputStream(QByteArray& buffer, qint32 startingIndex);

	/**
	 * Creates a <code>CBerByteArrayInputStream</code>
	 * so that it uses <code>buffer</code> as it's buffer array.
	 *
	 * @param buffer the input buffer
	 * @param startingIndex the starting offset for reading
	 * @param readingSize the maximum size to read from the buffer
	 */
	CBerByteArrayInputStream(QByteArray& buffer, qint32 startingIndex, qint32 readingSize);

	/**
	 * Reads next byte of data from input stream.
	 *
	 * @return the next byte of data, or in end of buffer case <code>-1</code> and QT signal endOfBufferReached
	 * @fn the QT signal endOfBufferReached when buffer is empty
	 */
	qint8 read();

	/**
	 * Reads up to <code>len</len> bytes of data into the <code>QByteArray</code> reference from input stream.
	 *
	 * @param buffer the input buffer
	 * @param startingIndex the starting offset for reading
	 * @param readingSize the maximum size to read from the buffer
	 *
	 * @return full length in bytes read into the buffer
	 */
	qint32 read(QByteArray& buffer, qint32 startingIndex, qint32 readingSize);


	/**
	 * Return the number of remaining bytes that can be read or skip from input stream.
	 *
	 * @return the number of remaining bytes.
	 */
	qint32 available();

	/**
	 * Tests if this stream supports mark functional.
	 *
	 * @return true when supports.
	 */
	bool markSupported();

	/**
	 * Sets mark position for call reset later.
	 */
	void mark();

	/*
	 * Resets position to marks early, the position 0 marks on creation.
	 */
	void reset();

	/*
	 * Return actual buffer
	 */
	QByteArray get();

signals:
	void signalInputStreamWarning(QString strErr);

};




#endif /* INCLUDE_BERBYTEARRAYINPUTSTREAM_H_ */
