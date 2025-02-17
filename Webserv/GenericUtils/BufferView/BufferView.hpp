// mutable_string_view.hpp
#ifndef MUTABLE_STRING_VIEW_HPP
#define MUTABLE_STRING_VIEW_HPP

#include <cstddef>
#include <stdexcept>

class BufferView
{
public:
    typedef std::size_t     size_type;
    typedef char*           iterator;
    typedef const char*     const_iterator;

private:
    const char*         m_data;
    const size_type     m_length;

public:
    BufferView(const char* str, const size_type len);

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

    char& 		operator[](size_type startPos);
    const char& operator[](size_type startPos) const;
    char& 		at(size_type startPos);
    const char& at(size_type startPos) const;

    // Substring
    BufferView substr(size_type startPos, size_type targetLength);

    // Find overloads
    size_type find(char ch, size_type startPos = 0) const;
    size_type find(const char* target, size_type targetLength, size_type startPos = 0) const;
    size_type find(const BufferView& sv, size_type startPos = 0) const;
    
    void    to_string(std::string& placeResult) const;

    // Constants
    static const size_type npos;
};

#endif // MUTABLE_STRING_VIEW_HPP


