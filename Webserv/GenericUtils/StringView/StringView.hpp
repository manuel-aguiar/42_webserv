// mutable_string_view.hpp
#ifndef MUTABLE_STRING_VIEW_HPP
#define MUTABLE_STRING_VIEW_HPP

#include <cstddef>
#include <stdexcept>

class StringView
{
public:
    typedef std::size_t     size_type;
    typedef char*           iterator;
    typedef const char*     const_iterator;

private:
    char*       m_data;
    size_type   m_length;

public:
    // Constructors
    StringView();
    StringView(char* str, size_type len);

    // Iterators
    iterator		begin();
    iterator 		end();
    const_iterator	begin() const;
    const_iterator	end() const;

    // Accessors
    char* 		data();
    const char* data() const;
    size_type 	size() const;
    size_type 	length() const;
    bool 		empty() const;

    char& 		operator[](size_type pos);
    const char& operator[](size_type pos) const;
    char& 		at(size_type pos);
    const char& at(size_type pos) const;

    // Substring
    StringView substr(size_type pos, size_type count);

    // Find overloads
    size_type find(char ch, size_type pos = 0) const;
    size_type find(const char* s, size_type pos, size_type count) const;
    size_type find(const char* s, size_type pos = 0) const;
    size_type find(const StringView& sv, size_type pos = 0) const;

    // Constants
    static const size_type npos;
};

#endif // MUTABLE_STRING_VIEW_HPP


