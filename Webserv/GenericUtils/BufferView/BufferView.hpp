// mutable_string_view.hpp
#ifndef WEBSERV_BUFFERVIEW_HPP
# define WEBSERV_BUFFERVIEW_HPP

# include <cstddef>
# include <stdexcept>

class BufferView
{
    public:
        // various constructors
        BufferView();
        BufferView(const std::string& str);
        BufferView(const char* str);
        BufferView(const void* data, const size_t len);
        BufferView(const char* str, const size_t len);

        ~BufferView();
        BufferView(const BufferView& other);
        BufferView& operator=(const BufferView& other);


        const char* data() const;
        size_t 	size() const;

        const char& operator[](size_t startPos) const;
        const char& at(size_t startPos) const;

        BufferView substr(size_t startPos, size_t targetLength) const;

        size_t find(char ch, size_t startPos = 0) const;
        size_t find(const char* target, size_t targetLength, size_t startPos = 0) const;
        size_t find(const BufferView& sv, size_t startPos = 0) const;
        
        void        to_string(std::string& placeResult) const;
        std::string to_string() const;

        static const size_t npos;

        int compare(const BufferView& other) const;

        bool operator==(const BufferView& other) const;
        bool operator!=(const BufferView& other) const;
        bool operator<(const BufferView& other) const;
        bool operator<=(const BufferView& other) const;
        bool operator>(const BufferView& other) const;
        bool operator>=(const BufferView& other) const;

    private:
        const char*         m_data;
        size_t              m_size;

};

std::ostream& operator<<(std::ostream& os, const BufferView& sv);

#endif // MUTABLE_STRING_VIEW_HPP
