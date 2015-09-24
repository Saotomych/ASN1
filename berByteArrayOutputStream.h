/*
 * Java version
 * Copyright Fraunhofer ISE, 2011
 * Author(s): Stefan Feuerhahn
 *
 * C++/QT Version
 * Open source, 2015
 * Author(s): Alexey Alyoshkin
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

class CBerByteArrayOutputStream: public QDataStream
{

	QByteBuffer m_buffer;
	quint32 m_index;
	bool m_bAutoResize;

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
	CBerByteArrayOutputStream(int bufferSize)
	{
		m_buffer.resize(bufferSize);
		m_index = bufferSize-1;
		m_bAutoResize = false;
	}

	CBerByteArrayOutputStream(quint32 bufferSize, bool automaticResize)
	{
		m_buffer.resize(bufferSize);
		m_index = bufferSize-1;
		m_bAutoResize = automaticResize;
	}

	CBerByteArrayOutputStream(QByteBuffer& buffer, quint32 startingIndex) {
		m_buffer = buffer;
		m_index = startingIndex;
		m_bAutoResize = false;
	}

	CBerByteArrayOutputStream(QByteBuffer& buffer, int startingIndex, bool automaticResize) {
		m_buffer = buffer;
		m_index = startingIndex;
		m_bAutoResize = automaticResize;
	}


};

