

#ifndef MUTABLE_STRING_VIEW_HPP
#define MUTABLE_STRING_VIEW_HPP

#include <cstddef>
#include <stdexcept>
#include <cstring>

class string_view {
public:
    typedef std::size_t size_type;
    typedef char* iterator;
    typedef const char* const_iterator;

private:
    char* data_;
    size_type length_;

public:
    // Constructors
    string_view() : data_(NULL), length_(0) {}
    string_view(char* str, size_type len) : data_(str), length_(len) {}

    // Iterators
    iterator begin() { return data_; }
    iterator end() { return data_ + length_; }
    const_iterator begin() const { return data_; }
    const_iterator end() const { return data_ + length_; }

    // Accessors
    char* data() { return data_; }
    const char* data() const { return data_; }
    size_type size() const { return length_; }
    size_type length() const { return length_; }
    bool empty() const { return length_ == 0; }

    char& operator[](size_type pos) {
        return data_[pos];
    }

    const char& operator[](size_type pos) const {
        return data_[pos];
    }

    char& at(size_type pos) {
        if (pos >= length_) {
            throw std::out_of_range("string_view::at");
        }
        return data_[pos];
    }

    const char& at(size_type pos) const {
        if (pos >= length_) {
            throw std::out_of_range("string_view::at");
        }
        return data_[pos];
    }

    // Substring (returns a view into the same buffer)
    string_view substr(size_type pos, size_type count) {
        if (pos > length_) {
            throw std::out_of_range("string_view::substr");
        }
        if (count > length_ - pos) {
            count = length_ - pos;
        }
        return string_view(data_ + pos, count);
    }

    // Find overloads
    size_type find(char ch, size_type pos = 0) const {
        if (pos >= length_) return npos;
        for (size_type i = pos; i < length_; ++i) {
            if (data_[i] == ch) return i;
        }
        return npos;
    }

    size_type find(const char* s, size_type pos, size_type count) const {
        if (pos >= length_ || count == 0) return npos;
        for (size_type i = pos; i <= length_ - count; ++i) {
            if (std::memcmp(data_ + i, s, count) == 0) {
                return i;
            }
        }
        return npos;
    }

    size_type find(const char* s, size_type pos = 0) const {
        return find(s, pos, std::strlen(s));
    }

    size_type find(const string_view& sv, size_type pos = 0) const {
        return find(sv.data(), pos, sv.size());
    }

    // Constants
    static const size_type npos = static_cast<size_type>(-1);
};

#endif // MUTABLE_STRING_VIEW_HPP
