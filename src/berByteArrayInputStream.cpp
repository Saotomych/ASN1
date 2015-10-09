/*
 * berByteArrayInputStream.cpp
 *
 *  Created on: 9 окт. 2015 г.
 *      Author: alexey
 */

#include "berByteArrayInputStream.h"

void CBerByteArrayInputStream::runtimeError(QString str)
{
#ifdef DEBUG
		throw std::runtime_error(str.toStdString());
#else
		emit signalInputStreamWarning(str);
#endif
}

qint32 CBerByteArrayInputStream::checkPosition()
{
	if (m_Pos >= m_Buffer.size())
	{
		QString info("CBerByteArrayInputStream::read: info: end of buffer reached: ");
		QString params(
				QString("position = %1, stream.size = %2")
					 .arg(m_Pos)
					 .arg(m_Buffer.size())
					 );
		emit signalInputStreamWarning(info + params);
		return -1;
	}

	return 0;
}


CBerByteArrayInputStream::CBerByteArrayInputStream(QByteArray& buffer): m_Pos(0), m_Mark(0)
{
	m_Buffer = buffer;
}

CBerByteArrayInputStream::CBerByteArrayInputStream(QByteArray& buffer, qint32 startingIndex): m_Pos(startingIndex), m_Mark(0)
{
	m_Buffer = buffer;
}

qint8 CBerByteArrayInputStream::read()
{
	if (checkPosition() == -1) return -1;

	return m_Buffer[m_Pos++];
}

qint32 CBerByteArrayInputStream::read(QByteArray& buffer, qint32 startingIndex, qint32 readingSize)
{
	qint32 available = m_Buffer.size()-startingIndex;
	if ( 0 > startingIndex || 0 > readingSize || available < readingSize)
	{
		QString info("CBerByteArrayInputStream::read: critical: invalid arguments: ");
		QString params( QString("buffer.size = %1, startingIndex = %2, readingSize = %3")
					 .arg(buffer.size())
					 .arg(startingIndex)
					 .arg(readingSize) );
		runtimeError(info + params);
		return -1;
	}

	if (checkPosition() == -1) return -1;

	qint32 availableSize = m_Buffer.size() - m_Pos;

	qint32 realReadingSize = readingSize;
	if (readingSize > availableSize) realReadingSize = availableSize;

	// copy stream buffer from m_Pos to qbytearray from startingIndex
	qint32 endi = startingIndex + realReadingSize;
	for (qint32 i = startingIndex; i < endi; ++i, ++m_Pos)
	{
		buffer[i] = m_Buffer[m_Pos];
	}

	return realReadingSize;
}

qint32 CBerByteArrayInputStream::available()
{
	return m_Buffer.size() - m_Pos;
}

bool CBerByteArrayInputStream::markSupported()
{
	return true;
}

void CBerByteArrayInputStream::mark()
{
	m_Mark = m_Pos;
}

void CBerByteArrayInputStream::reset()
{
	m_Pos = m_Mark;
}

