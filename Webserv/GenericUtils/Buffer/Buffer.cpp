
// Project headers
# include "Buffer.hpp"
# include "../../../Toolkit/Assert/AssertEqual/AssertEqual.h"

// C++ headers
# include <string>
# include <cstring> // memcpy

// C headers
# include <unistd.h> // read/write

Buffer::Buffer() : m_writeOffset(0), m_size(0), m_capacity(sizeof(m_buffer)) {}

Buffer::~Buffer() {}

int Buffer::read(Ws::fd fd, int startIndex)
{
    ASSERT_EQUAL((size_t)startIndex < m_capacity - 1, true, "Buffer::read(): starting index is beyond buffer capacity");
    ASSERT_EQUAL((size_t)startIndex <= m_size, true, "Buffer::read(): starting index is beyond current buffer size, no fragmentation allowed");

    m_size = startIndex + ::read(fd, m_buffer, m_capacity - startIndex - 1);
    return (m_size);
}

int Buffer::write(Ws::fd fd, int startIndex)
{
    ASSERT_EQUAL((size_t)startIndex < m_size, true, "Buffer::write(): starting index is beyond current buffer size");

    int bytesWritten = ::write(fd, m_buffer + startIndex, m_size - startIndex);
    
    m_writeOffset += bytesWritten;
    if (m_writeOffset == m_size)
        clear();

    return (bytesWritten);
}

size_t Buffer::size() const
{
    return (m_size);
}

size_t Buffer::capacity() const
{
    return (m_capacity);
}

size_t Buffer::writeOffset() const
{
    return (m_writeOffset);
}

void Buffer::push(const char* data, size_t size)
{
    ASSERT_EQUAL(m_size + size < m_capacity - 1, true, "Buffer::push(): data to push is beyond buffer capacity");

    std::memcpy(m_buffer + m_size, data, size);
    m_size += size;
}

void Buffer::push(const std::string& data)
{
    push(data.c_str(), data.size());
}

void Buffer::clear()
{
    m_size = 0;
    m_writeOffset = 0;
}

BufferView
Buffer::view() const
{
    return (BufferView(m_buffer, m_size));
}


// private
Buffer::Buffer(const Buffer& copy) : m_size(copy.m_size), m_capacity(copy.m_capacity)
{
    std::memcpy(m_buffer, copy.m_buffer, m_size);
}

Buffer& Buffer::operator=(const Buffer& assign)
{
    if (this == &assign)
        return (*this);

    ASSERT_EQUAL(m_capacity, assign.m_capacity, "assignment operator between buffers of different sizes is forbidden");

    m_size = assign.m_size;

    std::memcpy(m_buffer, assign.m_buffer, m_size);

    return (*this);
}