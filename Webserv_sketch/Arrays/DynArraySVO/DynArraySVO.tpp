/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DynArraySVO.tpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 08:14:03 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/09 13:17:02 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_DYNARRAY_SVO_TPP

# define WEBSERVER_DYNARRAY_SVO_TPP

# include <cstdlib>

# include <memory>

# include "../StackArray/StackArray.hpp"


template <typename T, size_t stackBuf, typename Allocator>
class DynArraySVO
{
	public:
		DynArraySVO(Allocator allocator = Allocator()) : m_heapArray(NULL), m_size(0), m_capacity(stackBuf), m_allocator(allocator)
		{
			//std::cout << "m_size: " << m_size << " m_capacity: " << m_capacity << std::endl;
		};

		DynArraySVO(const DynArraySVO &other) : m_heapArray(NULL), m_size(0), m_capacity(0), m_allocator(other.m_allocator)
		{
			*this = other;
		}

		~DynArraySVO()
		{
			if (m_heapArray)
			{
				//std::cout << "                  heaparray has: " << m_heapArray << std::endl;
				for (size_t i = 0; i < m_size; i++)
					m_allocator.destroy(m_heapArray + i);
				m_allocator.deallocate(m_heapArray, m_capacity);
			}
			else
			{
				for (size_t i = 0; i < m_size; i++)
					m_stackArray[i].~T();
			}
		}
		DynArraySVO &operator=(const DynArraySVO &other)
		{
			if (this == &other)
				return *this;

			if (m_heapArray)
			{
				for (int i = 0; i < m_size; i++)
					m_allocator.destroy(m_heapArray + i);
				m_allocator.deallocate(m_heapArray, m_capacity);
				m_heapArray = NULL;
			}
			else
			{
				for (int i = 0; i < m_size; i++)
					m_stackArray[i].~T();
			}

			
			if (other.m_heapArray)
			{
				m_heapArray = m_allocator.allocate(other.m_capacity);
				for (int i = 0; i < other.m_size; i++)
					m_allocator.construct(m_heapArray + i, other.m_heapArray[i]);
			}
			else
			{
				for (int i = 0; i < other.m_size; i++)
					new (&m_stackArray[i]) T(other.m_stackArray[i]);
			}


			m_size = other.m_size;
			m_capacity = other.m_capacity;

			return *this;
		}

		T& operator[](size_t index)
		{
			if (m_heapArray)
				return (m_heapArray[index]);
			return (m_stackArray[index]);
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
			if (m_heapArray)
				m_allocator.construct(m_heapArray + m_size++, value);
			else
				new (&m_stackArray[m_size++]) T(value);

		}

		void push_front(const T& value)
		{
			if (m_size == m_capacity)
				reserve(m_capacity ? m_capacity * 2 : 1);
			if (m_heapArray)
			{
				std::memmove((void*)(m_heapArray + 1), (void*)m_heapArray, m_size * sizeof(T));
				m_allocator.construct(m_heapArray, value);
			}
			else 
			{
				if (m_size > 0)
					std::memmove((void*)(&m_stackArray[1]), (void*)&m_stackArray[0], m_size * sizeof(T));
				new (&m_stackArray[0]) T(value);
			}
			m_size++;
		}

        T& at(size_t index)
        {
            assert (m_size != 0 && index < m_size);
			if (m_heapArray)
				return (m_heapArray[index]);
			return (m_stackArray[index]);
        }

		T& front()
		{
            assert (m_size != 0);
			if (m_heapArray)
				return (m_heapArray[0]);
			return (m_stackArray[0]);
		}   

		T& back()
		{
			assert (m_size != 0);
			if (m_heapArray)
				return (m_heapArray[m_size - 1]);
			return (m_stackArray[m_size - 1]);
		}


		void pop_back()
		{	
			if (!m_size)
				return ;
			if (m_heapArray)
				m_allocator.destroy(m_heapArray + --m_size);
			else
				m_stackArray[--m_size].~T();
		}

		void pop_front()
		{
			if (!m_size)
				return ;
			if (m_heapArray)
			{
				m_allocator.destroy(m_heapArray);
				std::memmove((void*)m_heapArray, (void*)(m_heapArray + 1), m_size * sizeof(T));
				
			}
			else
			{
				m_stackArray[0].~T();
				std::memmove((void*)(&m_stackArray[0]), (void*)&m_stackArray[1], m_size * sizeof(T));
			}
			m_size--;
		}

		void clear()
		{
			if (m_heapArray)
			{
				for (int i = 0; i < m_size; i++)
					m_allocator.destroy(m_heapArray + i);
				m_allocator.deallocate(m_heapArray, m_capacity);
				m_heapArray = NULL;
			}
			else
			{
				for (int i = 0; i < m_size; i++)
					m_stackArray[i].~T();
			}
			m_size = 0;
			m_capacity = 0;
		}

		void reserve(size_t size)
		{
			//std::cout << "reserve called with size: " << size << std::endl;
			if (size <= m_capacity)
				return;
			T* new_array = m_allocator.allocate(size);
			
			//std::cout << "                  moved to: " << new_array << std::endl;

			if (!m_heapArray)
				std::memmove((void*)new_array, (void*)&m_stackArray[0], m_size * sizeof(T));
			else if (m_heapArray != new_array)
			{
				std::memmove((void*)new_array, (void*)m_heapArray, m_size * sizeof(T));
				m_allocator.deallocate(m_heapArray, m_size);
			}
			m_heapArray = new_array;
			m_capacity = size;
		}

		void emplace_back()
		{
			if (m_size == m_capacity)
			{
				reserve(m_capacity ? m_capacity * 2 : 1);
				new (m_heapArray + m_size++) T();
			}
			else
				new (&m_stackArray[m_size++]) T();
		}

		template <typename Arg1 >
		void emplace_back(const Arg1& arg1)
		{
			//std::cout << "size : " << m_size << " capacity: " << m_capacity << std::endl;
			if (m_size == m_capacity)
				reserve(m_capacity ? m_capacity * 2 : 1);
			if (m_heapArray)
				new (m_heapArray + m_size++) T(arg1);
			else
				new (&m_stackArray[m_size++]) T(arg1);
		}

		template <typename Arg1, typename Arg2 >
		void emplace_back(const Arg1& arg1, const Arg2& arg2)
		{
			if (m_size == m_capacity)
				reserve(m_capacity ? m_capacity * 2 : 1);
			if (m_heapArray)
				new (m_heapArray + m_size++) T(arg1, arg2);
			else
				new (&m_stackArray[m_size++]) T(arg1, arg2);
		}

		template <typename Arg1, typename Arg2 , typename Arg3 >
		void emplace_back(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3)
		{
			if (m_size == m_capacity)
				reserve(m_capacity ? m_capacity * 2 : 1);
			if (m_heapArray)
				new (m_heapArray + m_size++) T(arg1, arg2, arg3);
			else
				new (&m_stackArray[m_size++]) T(arg1, arg2, arg3);
		}

		void emplace_front()
		{
			if (m_size == m_capacity)
				reserve(m_capacity ? m_capacity * 2 : 1);
			if (m_heapArray)
			{
				std::memmove((void*)(m_heapArray + 1), (void*)m_heapArray, m_size * sizeof(T));
				new (m_heapArray) T();
			}
			else
			{	
				if (m_size > 0)
					std::memmove((void*)(&m_stackArray[1]), (void*)&m_stackArray[0], m_size * sizeof(T));
				new (&m_stackArray[0]) T();
			}
			m_size++;
		}

		template <typename Arg1 >
		void emplace_front(const Arg1& arg1)
		{
			//std::cout << "emplace front: m_size: " << m_size << " m_capacity: " << m_capacity << std::endl;
			if (m_size == m_capacity)
				reserve(m_capacity ? m_capacity * 2 : 1);
			if (m_heapArray)
			{
				std::memmove((void*)(m_heapArray + 1), (void*)m_heapArray, m_size * sizeof(T));
				new (m_heapArray) T(arg1);
			}
			else
			{	
				if (m_size > 0)
					std::memmove((void*)(&m_stackArray[1]), (void*)&m_stackArray[0], m_size * sizeof(T));
				new (&m_stackArray[0]) T(arg1);
			}
			m_size++;
			//std::cout << "emplace front: done: " << std::endl;
		}

		template <typename Arg1, typename Arg2 >
		void emplace_front(const Arg1& arg1, const Arg2& arg2)
		{
			if (m_size == m_capacity)
				reserve(m_capacity ? m_capacity * 2 : 1);
			if (m_heapArray)
			{
				std::memmove((void*)(m_heapArray + 1), (void*)m_heapArray, m_size * sizeof(T));
				new (m_heapArray) T(arg1, arg2);
			}
			else
			{	
				if (m_size > 0)
					std::memmove((void*)(&m_stackArray[1]), (void*)&m_stackArray[0], m_size * sizeof(T));
				new (&m_stackArray[0]) T(arg1, arg2);
			}
			m_size++;
		}

		template <typename Arg1, typename Arg2 , typename Arg3 >
		void emplace_front(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3)
		{
			if (m_size == m_capacity)
				reserve(m_capacity ? m_capacity * 2 : 1);
			if (m_heapArray)
			{
				std::memmove((void*)(m_heapArray + 1), (void*)m_heapArray, m_size * sizeof(T));
				new (m_heapArray) T(arg1, arg2, arg3);
			}
			else
			{	
				if (m_size > 0)
					std::memmove((void*)(&m_stackArray[1]), (void*)&m_stackArray[0], m_size * sizeof(T));
				new (&m_stackArray[0]) T(arg1, arg2, arg3);
			}
			m_size++;
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

	iterator begin() 
	{ 
		if (m_heapArray)
			return iterator(m_heapArray);
		return iterator(&m_stackArray[0]); 
	}
	iterator end() 
	{ 
		if (m_heapArray)
			return iterator(m_heapArray + m_size);
		return iterator(&m_stackArray[m_size]); 
	}

	private:
		T*                          m_heapArray;
		StackArray<T, stackBuf>     m_stackArray;
		size_t      				m_size;
		size_t      				m_capacity;
		Allocator   				m_allocator;
};


#endif