

#ifndef WEBSERV_BUFFER_HPP

# define WEBSERV_BUFFER_HPP

namespace Ws {typedef int fd;}

class Buffer
{
	public:
		Buffer();
		~Buffer();
		Buffer(const Buffer& copy);
		Buffer& operator=(const Buffer& assign);

		int		read(Ws::fd fd, int startIndex = 0);
		int		write(Ws::fd fd, int startIndex = 0);

		size_t	size() const;
		size_t	capacity() const;

		void	clear();
		void	push(const char* data, size_t size);
		void	push(const std::string& data);

		const char* get() const;

	private:
		char			m_buffer[1024];
		size_t			m_size;
		const size_t	m_capacity;
};



#endif