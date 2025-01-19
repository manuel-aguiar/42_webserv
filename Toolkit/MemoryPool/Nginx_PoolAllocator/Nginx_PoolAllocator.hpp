

#ifndef NGINX_POOLALLOCATOR_HPP

# define NGINX_POOLALLOCATOR_HPP

# include "../Nginx_MemoryPool/Nginx_MemoryPool.hpp"

template <typename T, typename MemoryPool = Nginx_MemoryPool>
class Nginx_PoolAllocator;

# include "Nginx_PoolAllocator.tpp"

#endif