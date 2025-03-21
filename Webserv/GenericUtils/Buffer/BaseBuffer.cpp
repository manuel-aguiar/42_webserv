
// Project headers
# include "BaseBuffer.hpp"
# include "../../../Toolkit/Assert/AssertEqual/AssertEqual.h"

// C++ headers
# include <string>
# include <cstring> // memcpy

// C headers
# include <unistd.h> // read/write

BaseBuffer::BaseBuffer() : m_begin(NULL), m_end(NULL), m_writeOffset(0), m_size(0) {}
BaseBuffer::BaseBuffer(unsigned char* begin, unsigned char* end) : m_begin(begin), m_end(end), m_writeOffset(0), m_size(0) {}
BaseBuffer::~BaseBuffer() {}

void    BaseBuffer::reset(unsigned char* begin, unsigned char* end)
{
    m_begin = begin;
    m_end = end;
    m_writeOffset = 0;
    m_size = 0;
}

int BaseBuffer::read(const Ws::fd fd, const int startIndex)
{
    int bytesRead;

    ASSERT_EQUAL((size_t)startIndex < capacity(), true, "BaseBuffer::read(): starting index is beyond buffer capacity");
    ASSERT_EQUAL((size_t)startIndex <= m_size, true, "BaseBuffer::read(): starting index is beyond current buffer size, no fragmentation allowed");

    bytesRead = ::read(fd, m_begin + startIndex, capacity() - startIndex);

    if (bytesRead > 0)
        m_size = startIndex + bytesRead;
    return (bytesRead);
}

int BaseBuffer::readAppend(const Ws::fd fd, size_t maxBytes)
{
    int bytesRead;

    bytesRead = ::read(fd, m_begin + m_size, std::min(capacity() - m_size, maxBytes));

    if (bytesRead > 0)
        m_size += bytesRead;
    return (bytesRead);
}

int BaseBuffer::write(Ws::fd fd, int startIndex)
{
    if (startIndex >= (int)m_size)
        return (0);

    int bytesWritten = ::write(fd, m_begin + startIndex, m_size - startIndex);
    
    if (bytesWritten > 0)
        m_writeOffset += bytesWritten;

    return (bytesWritten);
}

const unsigned char* BaseBuffer::data() const
{
    return (m_begin);
}

size_t BaseBuffer::available() const
{
    return (capacity() - m_size);
}

size_t BaseBuffer::size() const
{
    return (m_size);
}

size_t BaseBuffer::capacity() const
{
    return (m_end - m_begin);
}

size_t BaseBuffer::writeOffset() const
{
    return (m_writeOffset);
}

void BaseBuffer::push(const char* data)
{
    push(data, std::strlen(data));
}

void BaseBuffer::push(const char* data, size_t size)
{
    ASSERT_EQUAL(m_size + size <= capacity(), true, "BaseBuffer::push(): data to push is beyond buffer capacity");
    ASSERT_EQUAL(data != NULL, true, "BaseBuffer::truncatePush(): data to push is NULL");
    ASSERT_EQUAL(data != NULL, true, "BaseBuffer::truncatePush(): data to push is NULL");

    if (!size)
        return ;

    std::memmove(m_begin + m_size, data, size);
    m_size += size;
}

void BaseBuffer::push(const std::string& data)
{
    push(data.c_str(), data.size());
}

void BaseBuffer::push(const BufferView& data)
{
    ASSERT_EQUAL(m_size + data.size() <= capacity(), true, "BaseBuffer::push(): data to push is beyond buffer capacity");

    if (!data.size())
        return ;
    
    std::memmove(m_begin + m_size, data.data(), data.size());
    m_size += data.size();
}

void BaseBuffer::pop(size_t size)
{
    ASSERT_EQUAL(size <= m_size, true, "BaseBuffer::pop(): size to pop is beyond buffer size");
    m_size -= size;
}

void BaseBuffer::truncatePush(const char* data)
{
    ASSERT_EQUAL(data != NULL, true, "BaseBuffer::truncatePush(): data to push is NULL");
    truncatePush(BufferView(data));
}

void BaseBuffer::truncatePush(const BufferView& data)
{
    ASSERT_EQUAL(data.size() <= capacity(), true, "BaseBuffer::truncatePush(): data to push is beyond buffer capacity");

    if (data.size() && (unsigned char*)data.data() != m_begin)
        std::memmove(m_begin, data.data(), data.size());
    m_size = data.size();
}

void BaseBuffer::truncatePush(const std::string& data)
{
    truncatePush(BufferView(data));
}

void BaseBuffer::truncatePush(const char* data, size_t size)
{
    ASSERT_EQUAL(data != NULL, true, "BaseBuffer::truncatePush(): data to push is NULL");
    truncatePush(BufferView(data, size));
}

void BaseBuffer::clear()
{
    m_size = 0;
    m_writeOffset = 0;
}

BufferView
BaseBuffer::view() const
{
    return (BufferView(m_begin, m_size));
}

unsigned char&		BaseBuffer::operator[](size_t index)
{
    ASSERT_EQUAL(index < m_size, true, "BaseBuffer::operator[]: index is beyond buffer size");
    return (m_begin[index]);
}

unsigned const char& BaseBuffer::operator[](size_t index) const
{
    ASSERT_EQUAL(index < m_size, true, "BaseBuffer::operator[]: index is beyond buffer size");
    return (m_begin[index]);
}



// private
BaseBuffer::BaseBuffer(const BaseBuffer& copy) : m_begin(copy.m_begin), m_end(copy.m_end), m_writeOffset(copy.m_writeOffset), m_size(copy.m_size)
{

}

BaseBuffer& BaseBuffer::operator=(const BaseBuffer& assign)
{
    if (this == &assign)
        return (*this);

    m_begin = assign.m_begin;
    m_end = assign.m_end;
    m_writeOffset = assign.m_writeOffset;
    m_size = assign.m_size;

    return (*this);
}
