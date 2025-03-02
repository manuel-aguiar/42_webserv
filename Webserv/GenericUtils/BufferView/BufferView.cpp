

// Project headers
#include "BufferView.hpp"
#include "../../../Toolkit/Assert/AssertEqual/AssertEqual.h"

// C++ headers
#include <iostream> //ostream
#include <cstring> //memcmp
#include <algorithm> //std::search std::find

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

BufferView
BufferView::trim(const BufferView trimSet) const
{
	size_t start = 0;
	size_t end = m_size;

	while (start < end && trimSet.find(m_data[start]) != BufferView::npos)
		++start;
	while (end > start && trimSet.find(m_data[end - 1]) != BufferView::npos)
		--end;
	return (BufferView(m_data + start, end - start));
}

BufferView
BufferView::modify_ToLowerCase()
{
	char* data = const_cast<char*>(m_data);

	for (size_t i = 0; i < m_size; ++i)
	{
		if (std::isalpha(data[i]) && std::isupper(data[i]))
			data[i] = std::tolower(data[i]);
	}
	return (*this);
}

BufferView
BufferView::modify_ToUpperCase()
{
	char* data = const_cast<char*>(m_data);

	for (size_t i = 0; i < m_size; ++i)
	{
		if (std::isalpha(data[i]) && std::islower(data[i]))
			data[i] = std::toupper(data[i]);
	}
	return (*this);
}

BufferView
BufferView::modify_ToCapitalized()
{
	char* data = const_cast<char*>(m_data);

	if (m_size > 0 && std::isalpha(data[0]) && std::islower(data[0]))
		data[0] = std::toupper(data[0]);
	for (size_t i = 1; i < m_size; ++i)
	{
		if (!std::isalpha(data[i - 1]) && std::islower(data[i]))
			data[i] = std::toupper(data[i]);
		else if (std::isalpha(data[i - 1]) && std::isupper(data[i]))
			data[i] = std::tolower(data[i]);
	}
	return (*this);
}

bool
BufferView::isOnlyTheseChars(const BufferView charSet) const
{
	for (size_t i = 0; i < m_size; ++i)
	{
		if (charSet.find(m_data[i]) == BufferView::npos)
			return (false);
	}
	return (true);
}

size_t
BufferView::find(char ch, size_t startPos) const
{
	if (startPos >= m_size)
		return npos;
	
	const char* result = std::find(m_data + startPos, m_data + m_size, ch);
	if (result == m_data + m_size)
		return (npos);
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
	
	const char* result = std::search(m_data + startPos, m_data + m_size, s, s + targetLength);
	if (result == m_data + m_size)
		return npos;
	return (result - m_data);
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
