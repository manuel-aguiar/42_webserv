/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DynArray.tpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 08:14:03 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/09 09:05:46 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_VECTOR_TPP

# define WEBSERVER_VECTOR_TPP

# include <cstdlib>

# include <memory>

template <typename T, typename Allocator>
class DynArray
{
    public:
        DynArray(Allocator allocator = Allocator()) : m_array(NULL), m_size(0), m_capacity(0), m_allocator(allocator) {};
        DynArray(int size, Allocator allocator = Allocator()) : m_array(NULL), m_size(size), m_capacity(size), m_allocator(allocator)
        {
            m_array = m_allocator.allocate(size, NULL);
            for (int i = 0; i < size; i++)
                m_allocator.construct(m_array + i, T());
        }
        
        DynArray(int size, T& value, const Allocator& allocator = Allocator())
        {
            m_array = m_allocator.allocate(size, NULL);
            for (int i = 0; i < size; i++)
                new (m_array + i) T(value);
        }

        DynArray(const DynArray &other) : m_array(NULL), m_size(0), m_capacity(0), m_allocator(other.m_allocator)
        {
            *this = other;
        }

        ~DynArray()
        {
            if (m_array)
            {
                for (size_t i = 0; i < m_size; i++)
                    m_allocator.destroy(m_array + i);
                m_allocator.deallocate(m_array, m_capacity);
            }
        }
        DynArray &operator=(const DynArray &other)
        {
            if (this == &other)
                return *this;
            if (m_array)
            {
                for (int i = 0; i < m_size; i++)
                    m_allocator.destroy(m_array + i);
                m_allocator.deallocate(m_array, m_capacity);
            }
            m_size = other.m_size;
            m_capacity = other.m_capacity;
            m_array = m_allocator.allocate(m_capacity);
            for (int i = 0; i < m_size; i++)
                m_allocator.construct(m_array + i, other.m_array[i]);
            return *this;
        }

        T& operator[](size_t index)
        {
            return (m_array[index]);
        }
        
        size_t size()
        {
            return (m_size);
        }

        size_t capacity()
        {
            return (m_capacity);
        }

        void push_back(const T& value)
        {
            if (m_size == m_capacity)
                reserve(m_capacity ? m_capacity * 2 : 1);
            m_allocator.construct(m_array + m_size++, value);
        }

        void push_front(const T& value)
        {
            if (m_size == m_capacity)
                reserve(m_capacity ? m_capacity * 2 : 1);
            std::memmove((void*)(m_array + 1), (void*)m_array, m_size * sizeof(T));
            m_allocator.construct(m_array, value);
            m_size++;
        }
        
        void pop_back()
        {
            if (m_size)
                m_allocator.destroy(m_array + --m_size);
        }

        void pop_front()
        {
            if (m_size)
            {
                m_allocator.destroy(m_array);
                std::memmove((void*)m_array, (void*)(m_array + 1), m_size * sizeof(T));
                m_size--;
            }
        }

        void clear()
        {
            if (m_array)
            {
                for (int i = 0; i < m_size; i++)
                    m_allocator.destroy(m_array + i);
                m_size = 0;
                m_allocator.deallocate(m_array, m_capacity);
                m_capacity = 0;
            }
        }

        void reserve(size_t size)
        {
            if (size <= m_capacity)
                return;
            T* new_array = m_allocator.allocate(size);
            std::cout << "                  moved to: " << new_array << std::endl;
            if (m_array != new_array)
            {
                std::memmove((void*)new_array, (void*)m_array, m_size * sizeof(T));
                m_allocator.deallocate(m_array, m_size);
                m_capacity = size;
                m_array = new_array;
            }
        }

        void emplace_back()
        {
            if (m_size == m_capacity)
                reserve(m_capacity ? m_capacity * 2 : 1);
            new (m_array + m_size++) T();
        }

        template <typename Arg1 >
        void emplace_back(const Arg1& arg1)
        {
            if (m_size == m_capacity)
                reserve(m_capacity ? m_capacity * 2 : 1);
            new (m_array + m_size++) T(arg1);
        }

        template <typename Arg1, typename Arg2 >
        void emplace_back(const Arg1& arg1, const Arg2& arg2)
        {
            if (m_size == m_capacity)
                reserve(m_capacity ? m_capacity * 2 : 1);
            new (m_array + m_size++) T(arg1, arg2);
        }

        template <typename Arg1, typename Arg2 , typename Arg3 >
        void emplace_back(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3)
        {
            if (m_size == m_capacity)
                reserve(m_capacity ? m_capacity * 2 : 1);
            new (m_array + m_size++) T(arg1, arg2, arg3);
        }

        void emplace_front()
        {
            if (m_size == m_capacity)
                reserve(m_capacity ? m_capacity * 2 : 1);
            std::memmove((void*)(m_array + 1), (void*)m_array, m_size * sizeof(T));
            m_size++;
            new (m_array) T();
        }

        template <typename Arg1 >
        void emplace_front(const Arg1& arg1)
        {
            if (m_size == m_capacity)
                reserve(m_capacity ? m_capacity * 2 : 1);
            std::memmove((void*)(m_array + 1), (void*)m_array, m_size * sizeof(T));
            m_size++;
            new (m_array) T(arg1);
        }

        template <typename Arg1, typename Arg2 >
        void emplace_front(const Arg1& arg1, const Arg2& arg2)
        {
            if (m_size == m_capacity)
                reserve(m_capacity ? m_capacity * 2 : 1);
            std::memmove((void*)(m_array + 1), (void*)m_array, m_size * sizeof(T));
            m_size++;
            new (m_array) T(arg1, arg2);
        }

        template <typename Arg1, typename Arg2 , typename Arg3 >
        void emplace_front(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3)
        {
            if (m_size == m_capacity)
                reserve(m_capacity ? m_capacity * 2 : 1);
            std::memmove((void*)(m_array + 1), (void*)m_array, m_size * sizeof(T));
            m_size++;
            new (m_array) T(arg1, arg2, arg3);
        }
        class iterator
        {
            public:
                typedef T                                   value_type;
                typedef T*                                  pointer;
                typedef T&                                  reference;
                typedef std::ptrdiff_t                      difference_type;
                typedef std::random_access_iterator_tag     iterator_category;

                iterator(pointer ptr) : m_ptr(ptr) {}
                iterator(const iterator& other) : m_ptr(other.m_ptr) {}
                ~iterator() {}
                iterator& operator=(const iterator& other) { m_ptr = other.m_ptr; return *this; }


                reference operator*() const { return *m_ptr; }
                pointer operator->() const { return m_ptr; }

                iterator& operator++()
                {
                    ++m_ptr;
                    return *this;
                }

                iterator operator++(int)
                {
                    iterator tmp = *this;
                    ++m_ptr;
                    return tmp;
                }

                iterator& operator--()
                {
                    --m_ptr;
                    return *this;
                }

                iterator operator--(int)
                {
                    iterator tmp = *this;
                    --m_ptr;
                    return tmp;
                }

                bool operator==(const iterator& other) const { return m_ptr == other.m_ptr; }
                bool operator!=(const iterator& other) const { return m_ptr != other.m_ptr; }


                reference operator[](size_t index) const { return *(m_ptr + index); }
                iterator operator+(size_t n) const { return iterator(m_ptr + n); }
                iterator operator-(size_t n) const { return iterator(m_ptr - n); }
                difference_type operator-(const iterator& other) const { return m_ptr - other.m_ptr; }

                iterator& operator+=(size_t n)
                {
                    m_ptr += n;
                    return *this;
                }
                iterator& operator-=(size_t n)
                {
                    m_ptr -= n;
                    return *this;
                }

            private:
                pointer m_ptr;
        };

    iterator begin() { return iterator(m_array); }
    iterator end() { return iterator(m_array + m_size); }

    private:
        T*          m_array;
        size_t      m_size;
        size_t      m_capacity;
        Allocator   m_allocator;
};


#endif