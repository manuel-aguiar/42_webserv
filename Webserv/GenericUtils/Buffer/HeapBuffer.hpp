

#ifndef WEBSERV_BUFFERHEAP_HPP

# define WEBSERV_BUFFERHEAP_HPP

# include "BaseBuffer.hpp"

class HeapBuffer : public BaseBuffer
{
	public:
		HeapBuffer(size_t size);
		HeapBuffer(const HeapBuffer& copy);
		HeapBuffer& operator=(const HeapBuffer& assign);
		~HeapBuffer();

	private:
		unsigned char* m_data;

};

#endif