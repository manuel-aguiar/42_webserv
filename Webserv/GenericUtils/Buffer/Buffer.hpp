

#ifndef WEBSERV_BUFFER_HPP

# define WEBSERV_BUFFER_HPP

// Project Headers
# include "../BufferView/BufferView.hpp"

// C++ headers
# include <cstddef> // size_t
# include <string> // std::string

// forward declarations
namespace Ws {typedef int fd;}

class Buffer
{
	public:
		Buffer();
		~Buffer();


		int			read(Ws::fd fd, int startIndex = 0);
		int			write(Ws::fd fd, int startIndex = 0);

		size_t		size() const;
		size_t		capacity() const;
		BufferView 	view() const;
		size_t		writeOffset() const;

		void		clear();
		void		push(const char* data, size_t size);
		void		push(const std::string& data);

	private:
		char			m_buffer[1024];
		size_t			m_writeOffset;
		size_t			m_size;
		const size_t	m_capacity;


		// private
		Buffer(const Buffer& copy);
		Buffer& operator=(const Buffer& assign);

};



#endif