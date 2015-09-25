#include "berByteArrayOutputStream.h"

void CBerByteArrayOutputStream::resize()
{
	QByteArray newBuffer(m_buffer.begin()->size(), Qt::Initialization::Uninitialized);
	m_buffer.push_front(newBuffer);
	m_index = m_buffer.begin()->size()-1;
}

CBerByteArrayOutputStream::CBerByteArrayOutputStream(int bufferSize)
{
	QByteArray newBuffer(bufferSize, Qt::Initialization::Uninitialized);
	m_buffer.push_front(newBuffer);
	m_index = bufferSize-1;
	m_bAutoResize = false;
}

CBerByteArrayOutputStream::CBerByteArrayOutputStream(quint32 bufferSize, bool automaticResize)
{
	QByteArray newBuffer(bufferSize, Qt::Initialization::Uninitialized);
	m_buffer.push_front(newBuffer);
	m_index = bufferSize-1;
	m_bAutoResize = automaticResize;
}

CBerByteArrayOutputStream::CBerByteArrayOutputStream(QByteArray& buffer, quint32 startingIndex) {
	m_buffer.push_front(buffer);
	m_index = startingIndex;
	m_bAutoResize = false;
}

CBerByteArrayOutputStream::CBerByteArrayOutputStream(QByteArray& buffer, quint32 startingIndex, bool automaticResize) {
	m_buffer = buffer;
	m_index = startingIndex;
	m_bAutoResize = automaticResize;
}

bool CBerByteArrayOutputStream::write(quint32 arg0)
{
	return write( (quint8) arg0 );
}

bool CBerByteArrayOutputStream::write(quint8 arg0)
{
	if (m_index < 0)
	{
		emit signalByteArrayIndexIsOutOfBound(QString("CBerByteArrayOutputStream::write: Maximum array size = %1 exceed").arg(m_buffer.begin()->size()));
		return false;
	}

	m_buffer.begin()[m_index] = arg0;

	if (m_index == 0 && m_bAutoResize)
	{
		resize();
	}
	else
	{
		m_index--;
	}

	return true;
}

bool CBerByteArrayOutputStream::write(QByteArray& byteArray)
{
	for (auto b: byteArray)
	{
		if (write( (quint8) b) == false) return false;
	}

	return true;
}

QByteArray CBerByteArrayOutputStream::getByteArray()
{
	QByteArray tempBuffer;

	for (QByteArray& byteArray: m_buffer)
	{
		if (byteArray.size()) tempBuffer += byteArray;
	}

	return QByteArray;
}
