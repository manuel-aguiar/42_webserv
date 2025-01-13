/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestAllocator.tpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 23:07:31 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/12 23:39:53 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TESTALLOCATOR_TPP

# define TESTALLOCATOR_TPP

# include <memory>
# include <new>


template <typename T>
class TestAllocator
{
    public:

        typedef T                   value_type;
        typedef T*                  pointer;
        typedef const T*            const_pointer;
        typedef T&                  reference;
        typedef const T&            const_reference;
        typedef std::size_t         size_type;
        typedef std::ptrdiff_t      difference_type;

        template <typename U>
        struct rebind
        {
            typedef TestAllocator<U> other;
        };
        
        typedef enum eCounters
        {
            E_ALLOC_COUNT,
            E_ALLOC_BYTES,
            E_DEALLOC_COUNT,
            E_DEALLOC_BYTES,
            E_TOTAL_COUNTERS
        }   tCounters;


        TestAllocator(size_t counters[E_TOTAL_COUNTERS]) : counters(counters) {}
        TestAllocator& operator=(const TestAllocator& copy) {counters = copy.counters; return (*this);}
        TestAllocator(const TestAllocator& copy) : counters(copy.counters){(void)copy;}
        template <typename U>
        TestAllocator(const TestAllocator<U>& other){counters = other.counters;}
        ~TestAllocator() {}

        size_t*     counters;

        pointer allocate(size_type n, const void* hint = 0)
        {
            (void)hint;
            if (n == 0)
                return 0;
            if (n > max_size())
                throw std::bad_alloc();
            counters[E_ALLOC_COUNT]++;
            counters[E_ALLOC_BYTES] += n * sizeof(T);


            return (pointer)(::operator new(n * sizeof(T)));
        }

        void deallocate(pointer p, size_type n)
        {
            (void)n;

            counters[E_DEALLOC_COUNT]++;
            counters[E_DEALLOC_BYTES] += n * sizeof(T);

            ::operator delete(p);
        }

        size_type max_size() const
        {
            return (size_type)(-1) / sizeof(T);
        }

        void construct(pointer p, const T& val)
        {
            new ((void*)p) T(val);
        }

        void destroy(pointer p)
        {
            p->~T();
        }
};


#endif