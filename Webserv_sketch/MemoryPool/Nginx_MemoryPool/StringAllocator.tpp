/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringAllocator.tpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 10:24:02 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/06 13:02:38 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRING_ALLOCATOR_TPP

# define STRING_ALLOCATOR_TPP

# include "../Nginx_MemoryPool/Nginx_MemoryPool.hpp"

template <typename T>
class StringAllocator
{
    public:

        typedef T value_type;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef T& reference;
        typedef const T& const_reference;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;

        template <typename U>
        struct rebind
        {
            typedef StringAllocator<U> other;
        };

        StringAllocator(Nginx_MemoryPool& pool) : memoryPool(pool) {}

        template <typename U>
        StringAllocator(const StringAllocator<U>& other) : memoryPool(other.memoryPool) {}

        pointer allocate(size_type n)
        {
            //std::cout << "allocated called" << std::endl;
            if (n == 0)
                return 0;
            if (n > std::numeric_limits<size_type>::max() / sizeof(T))
                throw std::bad_alloc();
            return static_cast<pointer>(memoryPool.allocate(n * sizeof(T), false));
        }

        void deallocate(pointer p, size_type n)
        {
            (void)p;
            (void)n;
        }

        bool operator==(const StringAllocator& other) const { return &memoryPool == &other.memoryPool; }
        bool operator!=(const StringAllocator& other) const { return &memoryPool != &other.memoryPool; }

        size_type max_size() const
        {
            return std::numeric_limits<size_type>::max() / sizeof(T);
        }

    private:
        Nginx_MemoryPool& memoryPool;
};




#endif