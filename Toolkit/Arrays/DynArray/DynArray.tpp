/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DynArray.tpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 08:14:03 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/14 11:41:12 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DYNARRAY_TPP

# define DYNARRAY_TPP

// C++ headers
# include <cstdlib>
# include <cassert>
# include <memory>
# include <cstring>

template <typename T, typename Allocator>
class DynArray
{
    public:
        template <typename U>
        class ArrayIterator;
        
        typedef ArrayIterator<T>         iterator;
        typedef ArrayIterator<const T>   const_iterator;
        typedef ArrayIterator<T>         reverse_iterator;
        typedef ArrayIterator<const T>   const_reverse_iterator;
    

    
        DynArray(const Allocator& allocator = Allocator()) : m_array(NULL), m_size(0), m_capacity(0), m_allocator(allocator) {};
        DynArray(size_t size, const Allocator& allocator = Allocator()) : m_array(NULL), m_size(size), m_capacity(size), m_allocator(allocator)
        {
            if (size)
                m_array = m_allocator.allocate(size, NULL);
            for (size_t i = 0; i < size; i++)
                m_allocator.construct(m_array + i, T());
        }

        DynArray(size_t size, T& value, const Allocator& allocator = Allocator())
        {
            m_array = m_allocator.allocate(size, NULL);
            for (size_t i = 0; i < size; i++)
                new (m_array + i) T(value);
        }

        DynArray(const DynArray &other) : m_array(NULL), m_size(0), m_capacity(0), m_allocator(other.m_allocator)
        {
            *this = other;
        }

        ~DynArray()
        {
            for (size_t i = 0; i < m_size; i++)
                m_allocator.destroy(m_array + i);
            m_allocator.deallocate(m_array, m_capacity);
        }
        DynArray &operator=(const DynArray &other)
        {
            if (this == &other)
                return (*this);
                
            if (m_allocator != other.m_allocator)
            {
                clear();
                m_allocator.deallocate(m_array, m_capacity);
                m_allocator = other.m_allocator;
            }
                
            if (m_array)
            {
                for (size_t i = 0; i < m_size; i++)
                    m_allocator.destroy(m_array + i);
                if (m_capacity != other.m_capacity)
                {
                    m_allocator.deallocate(m_array, m_capacity);
                    m_array = m_allocator.allocate(other.m_capacity);
                }
            }
            else
                m_array = m_allocator.allocate(other.m_capacity);
                
            m_size = other.m_size;
            m_capacity = other.m_capacity;
            for (size_t i = 0; i < m_size; i++)
                m_allocator.construct(m_array + i, other.m_array[i]);
            return (*this);
        }

        void move(DynArray& from)
        {
            if (m_array)
            {
                for (size_t i = 0; i < m_size; i++)
                    m_allocator.destroy(m_array + i);
                m_allocator.deallocate(m_array, m_capacity);
            }
            m_array = from.m_array;
            m_size = from.m_size;
            m_capacity = from.m_capacity;
            from.m_array = NULL;
            from.m_size = 0;
            from.m_capacity = 0;
        }

        T& operator[](size_t index)
        {
            return (m_array[index]);
        }
        const T& operator[](size_t index) const
        {
            return (m_array[index]);
        }

        size_t size() const
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

        T* getArray() const {return (m_array);}


        T& at(size_t index)
        {
            assert (m_size != 0 && index < m_size);
            return (m_array[index]);
        }

		T& front()
		{
            assert (m_size != 0);
			return (m_array[0]);
		}

		T& back()
		{
            assert (m_size != 0);
			return (m_array[m_size - 1]);
		}

        void pop_back()
        {
            if (m_size)
                m_allocator.destroy(m_array + --m_size);
        }

        void clear()
        {
            if (m_array)
            {
                for (size_t i = 0; i < m_size; i++)
                    m_allocator.destroy(m_array + i);
            }
            m_size = 0;
        }

        const Allocator& getAllocator() const
        {
            return (m_allocator);
        }



        void reserve(size_t size)
        {
            if (size <= m_capacity)
                return;
            T* new_array = m_allocator.allocate(size);
            m_capacity = size;
            
            if (m_array && m_array != new_array)
            {
                for (size_t i = 0; i < m_size; ++i)
                {
                    m_allocator.construct(new_array + i, m_array[i]);
                    m_allocator.destroy(m_array + i);
                }
                    
                m_allocator.deallocate(m_array, m_size);
            }
            m_array = new_array;
        }

        void emplace_back()
        {
			T* new_array = NULL;
			size_t newCap = m_capacity ? m_capacity * 2 : 1;

            if (m_size == m_capacity)
				new_array = m_allocator.allocate(newCap);

			new ((new_array ? new_array : m_array) + m_size) T();

			if (new_array && m_array != new_array)
            {
                for (size_t i = 0; i < m_size; ++i)
                {
                    m_allocator.construct(new_array + i, m_array[i]);
                    m_allocator.destroy(m_array + i);
                }
                m_allocator.deallocate(m_array, m_size);
                m_capacity = newCap;
                m_array = new_array;
            }
			++m_size;
        }

        template <typename Arg1 >
        void emplace_back(Arg1& arg1)
        {
			T* new_array = NULL;
			size_t newCap = m_capacity ? m_capacity * 2 : 1;

            if (m_size == m_capacity)
				new_array = m_allocator.allocate(newCap);

			new ((new_array ? new_array : m_array) + m_size) T(arg1);

			if (new_array && m_array != new_array)
            {
                for (size_t i = 0; i < m_size; ++i)
                {
                    m_allocator.construct(new_array + i, m_array[i]);
                    m_allocator.destroy(m_array + i);
                }
                m_allocator.deallocate(m_array, m_size);
                m_capacity = newCap;
                m_array = new_array;
            }
			++m_size;
        }

        template <typename Arg1, typename Arg2 >
        void emplace_back(Arg1& arg1, Arg2& arg2)
        {
			T* new_array = NULL;
			size_t newCap = m_capacity ? m_capacity * 2 : 1;

            if (m_size == m_capacity)
				new_array = m_allocator.allocate(newCap);

			new ((new_array ? new_array : m_array) + m_size) T(arg1, arg2);

			if (new_array && m_array != new_array)
            {
                for (size_t i = 0; i < m_size; ++i)
                {
                    m_allocator.construct(new_array + i, m_array[i]);
                    m_allocator.destroy(m_array + i);
                }
                m_allocator.deallocate(m_array, m_size);
                m_capacity = newCap;
                m_array = new_array;
            }
			++m_size;
        }

        template <typename Arg1, typename Arg2 , typename Arg3 >
        void emplace_back(Arg1& arg1, Arg2& arg2, Arg3& arg3)
        {
			T* new_array = NULL;
			size_t newCap = m_capacity ? m_capacity * 2 : 1;

            if (m_size == m_capacity)
				new_array = m_allocator.allocate(newCap);

			new ((new_array ? new_array : m_array) + m_size) T(arg1, arg2, arg3);

			if (new_array && m_array != new_array)
            {
                for (size_t i = 0; i < m_size; ++i)
                {
                    m_allocator.construct(new_array + i, m_array[i]);
                    m_allocator.destroy(m_array + i);
                }
                m_allocator.deallocate(m_array, m_size);
                m_capacity = newCap;
                m_array = new_array;
            }
            ++m_size;
        }

        template <typename Arg1, typename Arg2 , typename Arg3, typename Arg4 >
        void emplace_back(Arg1& arg1, Arg2& arg2, Arg3& arg3, Arg4& arg4)
        {
			T* new_array = NULL;
			size_t newCap = m_capacity ? m_capacity * 2 : 1;

            if (m_size == m_capacity)
				new_array = m_allocator.allocate(newCap);

			new ((new_array ? new_array : m_array) + m_size) T(arg1, arg2, arg3, arg4);

			if (new_array && m_array != new_array)
            {
                for (size_t i = 0; i < m_size; ++i)
                {
                    m_allocator.construct(new_array + i, m_array[i]);
                    m_allocator.destroy(m_array + i);
                }
                m_allocator.deallocate(m_array, m_size);
                m_capacity = newCap;
                m_array = new_array;
            }
            ++m_size;
        }

        template <typename Arg1, typename Arg2 , typename Arg3, typename Arg4, typename Arg5 >
        void emplace_back(Arg1& arg1, Arg2& arg2, Arg3& arg3, Arg3& arg4, Arg3& arg5)
        {
			T* new_array = NULL;
			size_t newCap = m_capacity ? m_capacity * 2 : 1;

            if (m_size == m_capacity)
				new_array = m_allocator.allocate(newCap);

			new ((new_array ? new_array : m_array) + m_size) T(arg1, arg2, arg3, arg4, arg5);

			if (new_array && m_array != new_array)
            {
                for (size_t i = 0; i < m_size; ++i)
                {
                    m_allocator.construct(new_array + i, m_array[i]);
                    m_allocator.destroy(m_array + i);
                }
                m_allocator.deallocate(m_array, m_size);
                m_capacity = newCap;
                m_array = new_array;
            }
            ++m_size;
        }

        template <typename Arg1 >
        void emplace_back(const Arg1& arg1)
        {
			T* new_array = NULL;
			size_t newCap = m_capacity ? m_capacity * 2 : 1;

            if (m_size == m_capacity)
				new_array = m_allocator.allocate(newCap);

			new ((new_array ? new_array : m_array) + m_size) T(arg1);

			if (new_array && m_array != new_array)
            {
                for (size_t i = 0; i < m_size; ++i)
                {
                    m_allocator.construct(new_array + i, m_array[i]);
                    m_allocator.destroy(m_array + i);
                }
                m_allocator.deallocate(m_array, m_size);
                m_capacity = newCap;
                m_array = new_array;
            }
			++m_size;
        }

        template <typename Arg1, typename Arg2 >
        void emplace_back(const Arg1& arg1, const Arg2& arg2)
        {
			T* new_array = NULL;
			size_t newCap = m_capacity ? m_capacity * 2 : 1;

            if (m_size == m_capacity)
				new_array = m_allocator.allocate(newCap);

			new ((new_array ? new_array : m_array) + m_size) T(arg1, arg2);

			if (new_array && m_array != new_array)
            {
                for (size_t i = 0; i < m_size; ++i)
                {
                    m_allocator.construct(new_array + i, m_array[i]);
                    m_allocator.destroy(m_array + i);
                }
                m_allocator.deallocate(m_array, m_size);
                m_capacity = newCap;
                m_array = new_array;
            }
			++m_size;
        }

        template <typename Arg1, typename Arg2 , typename Arg3 >
        void emplace_back(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3)
        {
			T* new_array = NULL;
			size_t newCap = m_capacity ? m_capacity * 2 : 1;

            if (m_size == m_capacity)
				new_array = m_allocator.allocate(newCap);

			new ((new_array ? new_array : m_array) + m_size) T(arg1, arg2, arg3);

			if (new_array && m_array != new_array)
            {
                for (size_t i = 0; i < m_size; ++i)
                {
                    m_allocator.construct(new_array + i, m_array[i]);
                    m_allocator.destroy(m_array + i);
                }
                m_allocator.deallocate(m_array, m_size);
                m_capacity = newCap;
                m_array = new_array;
            }
            ++m_size;
        }

        template <typename Arg1, typename Arg2 , typename Arg3, typename Arg4 >
        void emplace_back(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3, const Arg4& arg4)
        {
			T* new_array = NULL;
			size_t newCap = m_capacity ? m_capacity * 2 : 1;

            if (m_size == m_capacity)
				new_array = m_allocator.allocate(newCap);

			new ((new_array ? new_array : m_array) + m_size) T(arg1, arg2, arg3, arg4);

			if (new_array && m_array != new_array)
            {
                for (size_t i = 0; i < m_size; ++i)
                {
                    m_allocator.construct(new_array + i, m_array[i]);
                    m_allocator.destroy(m_array + i);
                }
                m_allocator.deallocate(m_array, m_size);
                m_capacity = newCap;
                m_array = new_array;
            }
            ++m_size;
        }

        template <typename Arg1, typename Arg2 , typename Arg3, typename Arg4, typename Arg5 >
        void emplace_back(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3, const Arg3& arg4, const Arg3& arg5)
        {
			T* new_array = NULL;
			size_t newCap = m_capacity ? m_capacity * 2 : 1;

            if (m_size == m_capacity)
				new_array = m_allocator.allocate(newCap);

			new ((new_array ? new_array : m_array) + m_size) T(arg1, arg2, arg3, arg4, arg5);

			if (new_array && m_array != new_array)
            {
                for (size_t i = 0; i < m_size; ++i)
                {
                    m_allocator.construct(new_array + i, m_array[i]);
                    m_allocator.destroy(m_array + i);
                }
                m_allocator.deallocate(m_array, m_size);
                m_capacity = newCap;
                m_array = new_array;
            }
            ++m_size;
        }

        template <typename U>
        class ArrayIterator
        {
            public:
                typedef U                                   value_type;
                typedef U*                                  pointer;
                typedef U&                                  reference;
                typedef std::ptrdiff_t                      difference_type;
                typedef std::random_access_iterator_tag     iterator_category;

                ArrayIterator(pointer ptr) : m_ptr(ptr) {}
                ArrayIterator(const ArrayIterator& other) : m_ptr(other.m_ptr) {}
                ~ArrayIterator() {}
                ArrayIterator& operator=(const ArrayIterator& other) { m_ptr = other.m_ptr; return *this; }


                reference operator*() const { return *m_ptr; }
                pointer operator->() const { return m_ptr; }

                ArrayIterator& operator++()
                {
                    ++m_ptr;
                    return *this;
                }

                ArrayIterator operator++(int)
                {
                    ArrayIterator tmp = *this;
                    ++m_ptr;
                    return tmp;
                }

                ArrayIterator& operator--()
                {
                    --m_ptr;
                    return *this;
                }

                ArrayIterator operator--(int)
                {
                    ArrayIterator tmp = *this;
                    --m_ptr;
                    return tmp;
                }

                bool operator==(const ArrayIterator& other) const { return m_ptr == other.m_ptr; }
                bool operator!=(const ArrayIterator& other) const { return m_ptr != other.m_ptr; }


                reference operator[](size_t index) const { return *(m_ptr + index); }
                ArrayIterator operator+(size_t n) const { return ArrayIterator(m_ptr + n); }
                ArrayIterator operator-(size_t n) const { return ArrayIterator(m_ptr - n); }
                difference_type operator-(const ArrayIterator& other) const { return m_ptr - other.m_ptr; }

                ArrayIterator& operator+=(size_t n)
                {
                    m_ptr += n;
                    return *this;
                }
                ArrayIterator& operator-=(size_t n)
                {
                    m_ptr -= n;
                    return *this;
                }

            private:
                pointer m_ptr;
        };

        iterator begin()						 	{ return (iterator				(&m_array[0])); }
        iterator end() 								{ return (iterator				(&m_array[m_size])); }

        const_iterator begin() const 				{ return (const_iterator		(&m_array[0])); }
        const_iterator end() const 					{ return (const_iterator		(&m_array[m_size])); }

        reverse_iterator rbegin() 					{ return (reverse_iterator		(&m_array[m_size - 1])); }
        reverse_iterator rend() 					{ return (reverse_iterator		(&m_array[-1])); }

        const_reverse_iterator rbegin() const 		{ return (const_reverse_iterator(&m_array[m_size - 1])); }
        const_reverse_iterator rend() const 		{ return (const_reverse_iterator(&m_array[-1])); }

        void erase(iterator iter)
        {
            if (iter == end())
                return;
            for (iterator i = iter; i != end(); ++i)
                *i = *(i + 1);
            pop_back();
        }

    private:
        T*          m_array;
        size_t      m_size;
        size_t      m_capacity;
        Allocator   m_allocator;
};


#endif
