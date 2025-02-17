

// mutable_string_view.cpp
#include "StringView.hpp"
#include <cstring>

const StringView::size_type StringView::npos = static_cast<size_type>(-1);

// Constructors
StringView::StringView() : m_data(NULL), m_length(0) {}
StringView::StringView(char* str, size_type len) : m_data(str), m_length(len) {}

// Iterators
StringView::iterator StringView::begin() { return m_data; }
StringView::iterator StringView::end() { return m_data + m_length; }
StringView::const_iterator StringView::begin() const { return m_data; }
StringView::const_iterator StringView::end() const { return m_data + m_length; }

// Accessors
char* StringView::data() { return m_data; }
const char* StringView::data() const { return m_data; }
StringView::size_type StringView::size() const { return m_length; }
StringView::size_type StringView::length() const { return m_length; }
bool StringView::empty() const { return m_length == 0; }

char& StringView::operator[](size_type pos) { return m_data[pos]; }
const char& StringView::operator[](size_type pos) const { return m_data[pos]; }

char& StringView::at(size_type pos) {
    if (pos >= m_length) throw std::out_of_range("StringView::at");
    return m_data[pos];
}
const char& StringView::at(size_type pos) const {
    if (pos >= m_length) throw std::out_of_range("StringView::at");
    return m_data[pos];
}

// Substring
StringView StringView::substr(size_type pos, size_type count) {
    if (pos > m_length) throw std::out_of_range("StringView::substr");
    if (count > m_length - pos) count = m_length - pos;
    return StringView(m_data + pos, count);
}

// Find overloads
StringView::size_type StringView::find(char ch, size_type pos) const {
    if (pos >= m_length) return npos;
    for (size_type i = pos; i < m_length; ++i) {
        if (m_data[i] == ch) return i;
    }
    return npos;
}

StringView::size_type StringView::find(const char* s, size_type pos, size_type count) const {
    if (pos >= m_length || count == 0) return npos;
    for (size_type i = pos; i <= m_length - count; ++i) {
        if (std::memcmp(m_data + i, s, count) == 0) return i;
    }
    return npos;
}

StringView::size_type StringView::find(const char* s, size_type pos) const {
    return find(s, pos, std::strlen(s));
}

StringView::size_type StringView::find(const StringView& sv, size_type pos) const {
    return find(sv.data(), pos, sv.size());
}