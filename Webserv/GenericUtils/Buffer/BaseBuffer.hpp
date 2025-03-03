

#ifndef WEBSERV_BASEBUFFER_HPP

# define WEBSERV_BASEBUFFER_HPP

// Project Headers
# include "../BufferView/BufferView.hpp"

// C++ headers
# include <cstddef> // size_t
# include <string> // std::string

// forward declarations
namespace Ws {typedef int fd;}

class BaseBuffer
{
	public:

		int			read(const Ws::fd fd, const int startIndex = 0);
		int			write(const Ws::fd fd, const int startIndex = 0);

		int			readAppend(const Ws::fd fd, size_t maxBytes);

		const unsigned char*	data() const;
		unsigned char&			operator[](size_t index);
		unsigned const char& 	operator[](size_t index) const;
		
		size_t		available() const;
		size_t		size() const;
		size_t		capacity() const;
		BufferView 	view() const;
		size_t		writeOffset() const;

		void		clear();

		void		push(const char* data);
		void		push(const char* data, size_t size);
		void		push(const std::string& data);
		void		push(const BufferView& data);

		// push target to the beginning of the buffer, effectively "clearing it"
		void		truncatePush(const char* data);
		void		truncatePush(const char* data, size_t size);
		void		truncatePush(const std::string& data);
		void		truncatePush(const BufferView& data);

	protected:
		BaseBuffer();
		BaseBuffer(unsigned char* begin, unsigned char* end);
		~BaseBuffer();
		BaseBuffer(const BaseBuffer& copy);
		BaseBuffer& operator=(const BaseBuffer& assign);

		void reset(unsigned char* begin, unsigned char* end);

	private:
		unsigned char*	m_begin;
		unsigned char*	m_end;

		size_t			m_writeOffset;
		size_t			m_size;

		// private


};

#endif
