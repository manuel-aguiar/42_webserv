// mutable_string_view.hpp
#ifndef WEBSERV_BUFFERVIEW_HPP
# define WEBSERV_BUFFERVIEW_HPP

# include <cstddef>
# include <stdexcept>

class BufferView
{
    public:
        BufferView(const char* str, const size_t len);
        ~BufferView();
        BufferView(const BufferView& other);

        const char* data() const;
        size_t 	size() const;

        const char& operator[](size_t startPos) const;
        const char& at(size_t startPos) const;

        BufferView substr(size_t startPos, size_t targetLength);

        size_t find(char ch, size_t startPos = 0) const;
        size_t find(const char* target, size_t targetLength, size_t startPos = 0) const;
        size_t find(const BufferView& sv, size_t startPos = 0) const;
        
        void    to_string(std::string& placeResult) const;

        static const size_t npos;
        
    private:
        const char*         m_data;
        const size_t        m_size;

        BufferView& operator=(const BufferView& other);

};

#endif // MUTABLE_STRING_VIEW_HPP


