/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StackArray.tpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 08:59:01 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/23 11:37:53 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STACKARRAY_TPP

# define STACKARRAY_TPP

#include <cstring>
#include <cassert>
#include <cstddef>


template <typename T, size_t ElemCount>
class StackArray
{
	public:
		StackArray(size_t size = 0) : m_size(size)
		{
			assert (size <= ElemCount);
			for (size_t i = 0; i < m_size; i++)
				new (&m_array[i * sizeof(T)]) T();
		};

		~StackArray()
		{
			for (size_t i = 0; i < m_size; i++)
				reinterpret_cast<T*>(&m_array[i * sizeof(T)])->~T();
		};

		StackArray(const StackArray &other)
		{
			std::memcpy(m_array, other.m_array, sizeof(T) * ElemCount);
		};

		StackArray &operator=(const StackArray &other)
		{
			if (this == &other)
				return (*this);
			std::memcpy(m_array, other.m_array, sizeof(T) * ElemCount);
			return (*this);
		};

		T& operator[](const size_t index)
		{
			assert(index < ElemCount);
			return *(reinterpret_cast<T*>(&m_array[index * sizeof(T)]));
		}

		
		size_t size() const
		{
			return (m_size);
		}

		size_t capacity() const
		{
				return (ElemCount);
		}

        T* getArray() const {return (m_array);}


        T& at(size_t index)
        {
            assert (m_size != 0 && index < m_size);
            return (*reinterpret_cast<T*>(m_array[index * sizeof(T)]));
        }

		T& front()
		{
            assert (m_size != 0);
			return (*reinterpret_cast<T*>(&m_array[0]));
		}

		T& back()
		{
            assert (m_size != 0);
			return (*reinterpret_cast<T*>(&m_array[(m_size - 1) * sizeof(T)]));
		}

		void push_back(const T& value)
		{
			assert(m_size < ElemCount);
			new (m_size++ * sizeof(T) + m_array) T(value);
		}

        void pop_back()
        {
			assert (m_size != 0);
            reinterpret_cast<T*>(&m_array[(m_size-- - 1) * sizeof(T)])->~T();
        }

		void emplace_back()
        {
			assert(m_size < ElemCount);
			new (m_size++ * sizeof(T) + m_array) T();
		}

		template <typename Arg1 >
		void emplace_back(Arg1& arg1)
		{
			assert(m_size < ElemCount);
			new (m_size++ * sizeof(T) + m_array) T(arg1);
        }

        template <typename Arg1, typename Arg2 >
        void emplace_back(Arg1& arg1, Arg2& arg2)
        {
			assert(m_size < ElemCount);
			new (m_size++ * sizeof(T) + m_array) T(arg1, arg2);
        }

        template <typename Arg1, typename Arg2 , typename Arg3 >
        void emplace_back(Arg1& arg1, Arg2& arg2, Arg3& arg3)
        {
			assert(m_size < ElemCount);
			new (m_size++ * sizeof(T) + m_array) T(arg1, arg2, arg3);
        }

		template <typename Arg1 >
		void emplace_back(const Arg1& arg1)
		{
			assert(m_size < ElemCount);
			new (m_size++ * sizeof(T) + m_array) T(arg1);
        }

        template <typename Arg1, typename Arg2 >
        void emplace_back(const Arg1& arg1, const Arg2& arg2)
        {
			assert(m_size < ElemCount);
			new (m_size++ * sizeof(T) + m_array) T(arg1, arg2);
        }

        template <typename Arg1, typename Arg2 , typename Arg3 >
        void emplace_back(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3)
        {
			assert(m_size < ElemCount);
			new (m_size++ * sizeof(T) + m_array) T(arg1, arg2, arg3);
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

    iterator begin() { return iterator(reinterpret_cast<T*>(&m_array)); }
    iterator end() { return iterator(reinterpret_cast<T*>(&m_array[m_size * sizeof(T)])); }

	private:
		typedef unsigned char 		t_byte;
		size_t 						m_size;
		t_byte  					m_array[sizeof(T) * ElemCount];
};




#endif