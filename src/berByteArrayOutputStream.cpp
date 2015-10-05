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

	m_Buffer.begin()[m_Index].push_back(arg0);

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
	for (auto b: byteArray)
	{
		if (write( (quint8) b) == false) return false;
	}

	return true;
}

QByteArray CBerByteArrayOutputStream::getByteArray()
{
	QByteArray tempBuffer;

	for (QByteArray& byteArray: m_Buffer)
	{
		if (byteArray.size()) tempBuffer += byteArray;
	}

	return tempBuffer;
}
