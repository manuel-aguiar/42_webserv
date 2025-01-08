/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeapArray.tpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 08:14:03 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/23 16:40:53 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEAPARRAY_TPP

# define HEAPARRAY_TPP

// C++ headers
# include <cstdlib>
# include <cassert>
# include <memory>
# include <cstring>

template <typename T, typename Allocator>
class HeapArray
{
	public:
		HeapArray(const size_t capacity = 0, const Allocator& allocator = Allocator()) : 
			m_allocator(allocator),
			m_array(m_allocator.allocate(capacity)), 
			m_size(0), 
			m_capacity(capacity)
		{
			assert(capacity);
		}

		HeapArray(const size_t capacity, const T& value, const Allocator& allocator = Allocator()) : 
			m_allocator(allocator),
			m_array(m_allocator.allocate(capacity)), 
			m_size(capacity), 
			m_capacity(capacity)
		{
			assert(capacity);
			for (size_t i = 0; i < size; i++)
				m_allocator.construct(&m_array[i], T(value));
		}


		HeapArray(const HeapArray &other) : 
			m_allocator(other.m_allocator),
			m_array(m_allocator.allocate(other.m_capacity)), 
			m_size(0), 
			m_capacity(other.m_capacity)
		{
			*this = other;
		}

		~HeapArray()
		{
			for (size_t i = 0; i < m_size; i++)
				m_allocator.destroy(&m_array[i]);
			m_allocator.deallocate(m_array, m_capacity);
		}

		HeapArray &operator=(const HeapArray &other)
		{
			assert(m_capacity == other.m_capacity);
			
			if (this == &other)
				return (*this);

            if (m_allocator != other.m_allocator)
            {
                clear();
               	m_allocator.deallocate(m_array, m_capacity);
                m_allocator = other.m_allocator;
				m_array = m_allocator.allocate(m_capacity);
            }

			size_t smaller = (m_size < other.m_size) ? m_size : other.m_size;
			for (size_t i = 0; i < smaller; ++i)
				m_array[i] = other.m_array[i];
			
			if (smaller == m_size)
			{
				for (size_t i = smaller; i < other.m_size; ++i)
					m_allocator.construct(&m_array[i], other.m_array[i]);
			}
			else
			{
				for (size_t i = smaller; i < m_size; ++i)
					m_allocator.destroy(&m_array[i]);
			}

			m_size = other.m_size;

			return (*this);
		}

		void move(HeapArray& from)
		{
			clear();
			m_allocator.deallocate(&m_array[0], m_capacity);

			m_array = from.m_array;
			m_size = from.m_size;
			from.m_array = NULL;
			from.m_size = 0;
		}

		T& operator[](const size_t index)
		{
			assert(index < m_capacity);
			return (m_array[index]);
		}

		const T& operator[](const size_t index) const
		{
			assert(index < m_capacity);
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

		void clear()
		{
			for (size_t i = 0; i < m_size; i++)
				m_allocator.destroy(&m_array[i]);
			m_size = 0;
		}

		const Allocator& getAllocator() const
		{
			return (m_allocator);
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
			return (m_array[(m_size - 1)]);
		}

		void push_back(const T& value)
		{
			assert(m_array && m_size < m_capacity);

			new (m_array + m_size++) T(value);
		}

        void pop_back()
        {
			assert (m_size != 0);

			m_allocator.destroy(&m_array[(m_size-- - 1)]);
        }

		void emplace_back()
        {
			assert(m_array && m_size < m_capacity);

			new (m_array + m_size++) T();
		}

		template <typename Arg1 >
		void emplace_back(Arg1& arg1)
		{
			assert(m_array && m_size < m_capacity);

			new (m_array + m_size++) T(arg1);
        }

        template <typename Arg1, typename Arg2 >
        void emplace_back(Arg1& arg1, Arg2& arg2)
        {
			assert(m_array && m_size < m_capacity);

			new (m_array + m_size++) T(arg1, arg2);
        }

        template <typename Arg1, typename Arg2 , typename Arg3 >
        void emplace_back(Arg1& arg1, Arg2& arg2, Arg3& arg3)
        {
			assert(m_array && m_size < m_capacity);

			new (m_array + m_size++) T(arg1, arg2, arg3);
        }

		template <typename Arg1 >
		void emplace_back(const Arg1& arg1)
		{
			assert(m_array && m_size < m_capacity);

			new (m_array + m_size++) T(arg1);
        }

        template <typename Arg1, typename Arg2 >
        void emplace_back(const Arg1& arg1, const Arg2& arg2)
        {
			assert(m_array && m_size < m_capacity);
			new (m_array + m_size++) T(arg1, arg2);
        }

        template <typename Arg1, typename Arg2 , typename Arg3 >
        void emplace_back(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3)
        {
			assert(m_array && m_size < m_capacity);
			new (m_array + m_size++) T(arg1, arg2, arg3);
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

    iterator begin() { return iterator(&m_array[0]); }
    iterator end() { return iterator(&m_array[m_size]); }

	private:
		Allocator					m_allocator;
		T*							m_array;
		size_t						m_size;
		const size_t				m_capacity;
};


#endif
