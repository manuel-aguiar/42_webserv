

#ifndef WEBSERV_BUFFERSTACK_HPP

# define WEBSERV_BUFFERSTACK_HPP

# include "BaseBuffer.hpp"

template<size_t Size>
class Buffer : public BaseBuffer
{
	public:
		Buffer() : BaseBuffer(m_data, m_data + Size) {}
		~Buffer() {}

	private:
		unsigned char m_data[Size];
};

#endif
