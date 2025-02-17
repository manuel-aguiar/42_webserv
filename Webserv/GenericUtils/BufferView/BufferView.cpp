

// Project headers
#include "BufferView.hpp"
#include "../../../Toolkit/Assert/AssertEqual/AssertEqual.h"

// C++ headers
#include <cstring> //memcmp

const
BufferView::size_type BufferView::npos = static_cast<size_type>(-1);

// Constructors
BufferView::BufferView(const char* str, const size_type len) : m_data(str), m_length(len) {}

BufferView::~BufferView() {}

BufferView::BufferView(const BufferView& other) : m_data(other.m_data), m_length(other.m_length) {}

BufferView& BufferView::operator=(const BufferView& other)
{
	if (this == &other)
		return (*this);
	return (*this);
}

// Iterators
BufferView::const_iterator
BufferView::begin() const        { return (m_data); }

BufferView::const_iterator
BufferView::end() const          { return (m_data + m_length); }

// Accessors
const char*
BufferView::data() const         { return (m_data); }

BufferView::size_type
BufferView::size() const              { return (m_length); }

BufferView::size_type
BufferView::length() const            { return (m_length); }

bool
BufferView::empty() const                              { return (m_length == 0); }

const char&
BufferView::operator[](size_type startPos) const 	{ return m_data[startPos]; }


const char&
BufferView::at(size_type startPos) const
{
	ASSERT_EQUAL(startPos < m_length, true, "StringView::at, accessing beyond the length");
	return (m_data[startPos]);
}

BufferView
BufferView::substr(size_type startPos, size_type targetLength)
{
	ASSERT_EQUAL(startPos < m_length, true, "BufferView::substr, accessing beyond the length");

	if (targetLength > m_length - startPos)
		targetLength = m_length - startPos;
	return (BufferView(const_cast<char*>(m_data) + startPos, targetLength));
}

BufferView::size_type
BufferView::find(char ch, size_type startPos) const
{
	if (startPos >= m_length)
		return npos;
	for (size_type i = startPos; i < m_length; ++i)
	{
		if (m_data[i] == ch)
			return i;
	}
	return (npos);
}

/*
	Naive implementation, character by character comparison
*/
BufferView::size_type
BufferView::find(const char* s, size_type targetLength, size_type startPos) const
{
	if (startPos >= m_length || targetLength == 0)
		return npos;
	for (size_type i = startPos; i <= m_length - targetLength; ++i)
	{
		if (std::memcmp(m_data + i, s, targetLength) == 0)
			return i;
	}
	return (npos);
}

BufferView::size_type
BufferView::find(const BufferView& sv, size_type startPos) const
{
	return (find(sv.data(), startPos, sv.size()));
}

void
BufferView::to_string(std::string& placeResult) const
{
	placeResult.assign(m_data, m_length);
}