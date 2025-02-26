

# include "HeapBuffer.hpp"

# include <cstring> // memmove

HeapBuffer::HeapBuffer(size_t size)
: BaseBuffer()
{
	m_data = new unsigned char[size];
	this->reset(m_data, m_data + size);
}

HeapBuffer::HeapBuffer(const HeapBuffer& copy)
: BaseBuffer()
{
	m_data = new unsigned char[copy.size()];
	this->reset(m_data, m_data + copy.size());
	std::memmove(m_data, copy.data(), copy.size());
}

HeapBuffer::~HeapBuffer()
{
	delete[] m_data;
}

HeapBuffer&
HeapBuffer::operator=(const HeapBuffer& assign)
{
	if (this == &assign)
		return (*this);

	if (capacity() != assign.capacity())
	{
		delete[] m_data;
		m_data = new unsigned char[assign.size()];
	}
	this->reset(m_data, m_data + assign.size());
	std::memmove(m_data, assign.data(), assign.size());

	return (*this);
}
