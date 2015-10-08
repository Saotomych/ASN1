#include <include/berByteArrayOutputStream.h>

void CBerByteArrayOutputStream::resize()
{
	QByteArray newBuffer(m_Buffer.begin()->size(), Qt::Initialization::Uninitialized);
	m_Buffer.push_front(newBuffer);
	m_Index = m_Buffer.begin()->size()-1;
}

CBerByteArrayOutputStream::CBerByteArrayOutputStream(int bufferSize)
{
	QByteArray newBuffer(bufferSize, Qt::Initialization::Uninitialized);
	m_Buffer.push_front(newBuffer);
	m_Index = bufferSize-1;
	m_AutoResize = false;
}

CBerByteArrayOutputStream::CBerByteArrayOutputStream(quint32 bufferSize, bool automaticResize)
{
	QByteArray newBuffer(bufferSize, Qt::Initialization::Uninitialized);
	m_Buffer.push_front(newBuffer);
	m_Index = bufferSize-1;
	m_AutoResize = automaticResize;
}

CBerByteArrayOutputStream::CBerByteArrayOutputStream(QByteArray& buffer, quint32 startingIndex) {
	m_Buffer.push_front(buffer);
	m_Index = startingIndex;
	m_AutoResize = false;
}

CBerByteArrayOutputStream::CBerByteArrayOutputStream(QByteArray& buffer, quint32 startingIndex, bool automaticResize) {
	m_Buffer.push_front(buffer);
	m_Index = startingIndex;
	m_AutoResize = automaticResize;
}

bool CBerByteArrayOutputStream::write(quint32 arg0)
{
	return write( (quint8) arg0 );
}

bool CBerByteArrayOutputStream::write(quint8 arg0)
{
	if (m_Index < 0)
	{
		emit signalByteArrayIndexIsOutOfBound(QString("CBerByteArrayOutputStream::write: Maximum array size = %1 exceed").arg(m_Buffer.begin()->size()));
		return false;
	}

	QByteArray& lastBtArray = *(m_Buffer.begin());
	lastBtArray[m_Index] = arg0;

	if (m_Index == 0 && m_AutoResize)
	{
		resize();
	}
	else
	{
		m_Index--;
	}

	return true;
}

bool CBerByteArrayOutputStream::write(QByteArray& byteArray)
{
	for (int idx = byteArray.size()-1; idx >= 0; --idx)
	{
		if (write( (quint8) byteArray[idx]) == false) return false;
	}

	return true;
}

QByteArray CBerByteArrayOutputStream::getByteArray()
{
	QByteArray tempBuffer;

	for (QByteArray& byteArray: m_Buffer)
	{
		if (m_Index == -1)
		{
			tempBuffer += byteArray;
		}
		else
		{
			tempBuffer = byteArray.mid(m_Index+1, byteArray.size()-1);
		}
	}

	return tempBuffer;
}
