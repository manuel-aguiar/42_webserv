

#ifndef NGINX_MEMORYPOOL_HPP

# define NGINX_MEMORYPOOL_HPP

# include <memory>

typedef unsigned char t_byte;

template < typename T = t_byte, typename Allocator = std::allocator<t_byte> >
class Impl_Nginx_MemoryPool;

typedef Impl_Nginx_MemoryPool<> Nginx_MemoryPool;

# include "Nginx_MemoryPool.tpp"


#endif