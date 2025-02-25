

// Project headers
#include "BufferView.hpp"
#include "../../../Toolkit/Assert/AssertEqual/AssertEqual.h"

// C++ headers
#include <iostream> //ostream
#include <cstring> //memcmp

const
size_t BufferView::npos = static_cast<size_t>(-1);

// Constructors
BufferView::BufferView() : m_data(NULL), m_size(0) {}
BufferView::BufferView(const std::string& str) : m_data(str.c_str()), m_size(str.size()) {}
BufferView::BufferView(const char* str) : m_data(str), m_size(strlen(str)) {}
BufferView::BufferView(const void* data, const size_t len) : m_data(static_cast<const char*>(data)), m_size(len) {}
BufferView::BufferView(const char* str, const size_t len) : m_data(str), m_size(len) {}

BufferView::~BufferView() {}

BufferView::BufferView(const BufferView& other) : m_data(other.m_data), m_size(other.m_size) {}

BufferView& BufferView::operator=(const BufferView& other)
{
	if (this == &other)
		return (*this);
	m_data = other.m_data;
	m_size = other.m_size;
	return (*this);
}

// Accessors
const char*
BufferView::data() const
{ return (m_data); }

size_t
BufferView::size() const
{ return (m_size); }

const char&
BufferView::operator[](size_t startPos) const
{ 
	ASSERT_EQUAL(startPos < m_size, true, "StringView::at, accessing beyond the length");
	return m_data[startPos]; 
}


const char&
BufferView::at(size_t startPos) const
{
	ASSERT_EQUAL(startPos < m_size, true, "StringView::at, accessing beyond the length");
	return (m_data[startPos]);
}

BufferView
BufferView::substr(size_t startPos, size_t targetLength) const
{
	ASSERT_EQUAL(startPos <= m_size, true, "BufferView::substr, accessing beyond the length");

	if (targetLength > m_size - startPos)
		targetLength = m_size - startPos;
	return (BufferView(const_cast<char*>(m_data) + startPos, targetLength));
}

size_t
BufferView::find(char ch, size_t startPos) const
{
	if (startPos >= m_size)
		return npos;
	
	const char* result = (const char*)std::memchr(m_data + startPos, ch, m_size - startPos);
	if (result == NULL)
		return npos;
	return (result - m_data);
}

/*
	Naive implementation, character by character comparison
*/
size_t
BufferView::find(const char* s, size_t targetLength, size_t startPos) const
{
	if (startPos >= m_size || targetLength == 0 || targetLength > m_size - startPos)
		return npos;
	for (size_t i = startPos; i <= m_size - targetLength; ++i)
	{
		if (std::memcmp(m_data + i, s, targetLength) == 0)
			return i;
	}
	return (npos);
}

size_t
BufferView::find(const BufferView& sv, size_t startPos) const
{
	return (find(sv.data(), sv.size(), startPos));
}

void
BufferView::to_string(std::string& placeResult) const
{
	placeResult.assign(m_data, m_size);
}

std::string
BufferView::to_string() const
{
	std::string result;

	result.assign(m_data, m_size);
	return (result);
}

int BufferView::compare(const BufferView& other) const
{
    size_t min_size = (m_size < other.m_size) ? m_size : other.m_size;
    int result = std::memcmp(m_data, other.m_data, min_size);
    
    if (result == 0)
        return (m_size < other.m_size) ? -1 : (m_size > other.m_size);
    
    return (result);
}

bool BufferView::operator==(const BufferView& other) const
{
	return (compare(other) == 0);
}

bool BufferView::operator!=(const BufferView& other) const
{
	return (compare(other) != 0);
}

bool BufferView::operator<(const BufferView& other) const
{
	return (compare(other) < 0);
}

bool BufferView::operator<=(const BufferView& other) const
{
	return (compare(other) <= 0);
}

bool BufferView::operator>(const BufferView& other) const
{
	return (compare(other) > 0);
}

bool BufferView::operator>=(const BufferView& other) const
{
	return (compare(other) >= 0);
}

std::ostream& operator<<(std::ostream& os, const BufferView& sv)
{
	os.write(sv.data(), sv.size());
	return (os);
}
