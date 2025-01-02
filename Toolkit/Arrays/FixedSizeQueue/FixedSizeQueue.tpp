/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FixedSizeQueue.tpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 13:26:42 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/02 14:48:18 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FIXEDSIZEQUEUE_TPP

# define FIXEDSIZEQUEUE_TPP

# include "../DynArray/DynArray.hpp"


template <typename T, typename Allocator>
class FixedSizeQueue
{
	public:
		FixedSizeQueue(const size_t capacity = 0, const Allocator& allocator = Allocator()) : 
			m_allocator(allocator),
			m_array(reinterpret_cast<t_byte*>(m_allocator.allocate(capacity))),
			m_head(0),
			m_tail(0), 
			m_size(0), 
			m_capacity(capacity)
		{
			assert(capacity);
		}


		FixedSizeQueue(const FixedSizeQueue &other) : 
			m_allocator(other.m_allocator),
			m_array(reinterpret_cast<t_byte*>(m_allocator.allocate(other.m_capacity))), 
			m_size(0), 
			m_capacity(other.m_capacity)
		{
			*this = other;
		}

		~FixedSizeQueue()
		{
			for (size_t i = 0; i < m_size; i++)
				m_allocator.destroy(reinterpret_cast<T*>(&m_array[i * sizeof(T)]));
			m_allocator.deallocate(reinterpret_cast<T*>(m_array), m_capacity);
		}

		FixedSizeQueue &operator=(const FixedSizeQueue &other)
		{
			assert(m_capacity == other.m_capacity);

			if (this == &other)
				return (*this);

			
/*
			We'll see......

            if (m_allocator != other.m_allocator)
            {
                clear();
               	m_allocator.deallocate(reinterpret_cast<T*>(m_array), m_capacity);
                m_allocator = other.m_allocator;
				m_array = reinterpret_cast<t_byte*>(m_allocator.allocate(m_capacity));
            }

			size_t smaller = (m_size < other.m_size) ? m_size : other.m_size;
			for (size_t i = 0; i < smaller; ++i)
				*reinterpret_cast<T*>(&m_array[i * sizeof(T)]) = *reinterpret_cast<const T*>(other.m_array[i * sizeof(T)]);
			if (smaller == m_size)
			{
				for (size_t i = smaller; i < other.m_size; ++i)
					m_allocator.construct(
						reinterpret_cast<T*>(&m_array[i * sizeof(T)]), 
						*reinterpret_cast<T*>(&other.m_array[i * sizeof(T)])
					);
			}
			else
			{
				for (size_t i = smaller; i < m_size; ++i)
					m_allocator.destroy(reinterpret_cast<T*>(&m_array[i * sizeof(T)]));
			}

			m_size = other.m_size;
*/
			return (*this);
		}


		T& operator[](const size_t index)
		{
			assert(index < m_capacity);

			size_t position;

			position = (m_head + index) % m_capacity;
			return *(reinterpret_cast<T*>(&m_array[position * sizeof(T)]));
		}

		const T& operator[](const size_t index) const
		{
			assert(index < m_capacity);
			
			size_t position;

			position = (m_head + index) % m_capacity;
			return *(reinterpret_cast<T*>(&m_array[position * sizeof(T)]));
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
			if (!m_size)
				return ;
			

			/* there must be a cleaner way to do this..... */
			if (m_head == m_tail)
			{
				// buffer is full
				for (size_t i = 0; i < m_capacity; i++)
					m_allocator.destroy(reinterpret_cast<T*>(&m_array[i * sizeof(T)]));
			}
			else if (m_head < m_tail)
			{
				// buffer is not wrapped
				for (size_t i = m_head; i < m_tail; i++)
					m_allocator.destroy(reinterpret_cast<T*>(&m_array[i * sizeof(T)]));
			}
			else
			{
				// buffer is wrapped
				for (size_t i = 0; i < m_tail; i++)
					m_allocator.destroy(reinterpret_cast<T*>(&m_array[i * sizeof(T)]));
				for (size_t i = m_head; i < m_capacity; i++)
					m_allocator.destroy(reinterpret_cast<T*>(&m_array[i * sizeof(T)]));

			}
			
			m_size = 0;
			m_head = 0;
			m_tail = 0;
		}

		const Allocator& getAllocator() const
		{
			return (m_allocator);
		}

        T& at(size_t index)
        {
            assert (m_size != 0 && index < m_size);

			size_t position;

			position = (m_head + index) % m_capacity;
			return *(reinterpret_cast<T*>(&m_array[position * sizeof(T)]));
        }

		T& front()
		{
			assert (m_size != 0);
			
			return *(reinterpret_cast<T*>(&m_array[m_head * sizeof(T)]));
		}

		T& back()
		{
			assert (m_size != 0);
			
			return *(reinterpret_cast<T*>(&m_array[m_tail * sizeof(T)]));
		}

		// m_tail is one past last (iter::end()), so the new one will be exactly there
		// so construct first, adjust after
		void push_back(const T& value)
		{
			assert(m_array && m_size < m_capacity);

			new (m_array + m_tail * sizeof(T)) T(value);
			m_tail = (m_tail + 1) % m_capacity;
			m_size++;

		}

		// m_tail is one past last (iter::end()), adjust first, delete after
		void pop_back()
		{
			assert (m_size != 0);
			
			m_tail = ((m_tail - 1) % m_capacity + m_capacity) % m_capacity;
			m_allocator.destroy(reinterpret_cast<T*>(&m_array[m_tail * sizeof(T)]));
		}

		//m_head is already the first, so we adjust first, construct after
		void push_front(const T& value)
		{
			assert(m_array && m_size < m_capacity);

			m_head = ((m_head - 1) % m_capacity + m_capacity) % m_capacity;
			new (m_array + m_head * sizeof(T)) T(value);
			m_size++;
		}

		// m_head is the first, delete first, adjust after
		void pop_front()
		{
			assert (m_size != 0);
			
			m_allocator.destroy(reinterpret_cast<T*>(&m_array[m_head * sizeof(T)]));
			m_head = (m_head + 1) % m_capacity;
			m_size--;
		}

		void emplace_back()
        {
			assert(m_array && m_size < m_capacity);

			new (m_array + m_tail * sizeof(T)) T();
			m_tail = (m_tail + 1) % m_capacity;
			m_size++;
		}

		template <typename Arg1 >
		void emplace_back(Arg1& arg1)
		{
			assert(m_array && m_size < m_capacity);

			new (m_array + m_tail * sizeof(T)) T(arg1);
			m_tail = (m_tail + 1) % m_capacity;
			m_size++;
        }

        template <typename Arg1, typename Arg2 >
        void emplace_back(Arg1& arg1, Arg2& arg2)
        {
			assert(m_array && m_size < m_capacity);

			new (m_array + m_tail * sizeof(T)) T(arg1, arg2);
			m_tail = (m_tail + 1) % m_capacity;
			m_size++;
        }

        template <typename Arg1, typename Arg2 , typename Arg3 >
        void emplace_back(Arg1& arg1, Arg2& arg2, Arg3& arg3)
        {
			assert(m_array && m_size < m_capacity);

			new (m_array + m_tail * sizeof(T)) T(arg1, arg2, arg3);
			m_tail = (m_tail + 1) % m_capacity;
			m_size++;
        }

		template <typename Arg1 >
		void emplace_back(const Arg1& arg1)
		{
			assert(m_array && m_size < m_capacity);

			new (m_array + m_tail * sizeof(T)) T(arg1);
			m_tail = (m_tail + 1) % m_capacity;
			m_size++;
        }

        template <typename Arg1, typename Arg2 >
        void emplace_back(const Arg1& arg1, const Arg2& arg2)
        {
			assert(m_array && m_size < m_capacity);

			new (m_array + m_tail * sizeof(T)) T(arg1, arg2);
			m_tail = (m_tail + 1) % m_capacity;
			m_size++;
        }

        template <typename Arg1, typename Arg2 , typename Arg3 >
        void emplace_back(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3)
        {
			assert(m_array && m_size < m_capacity);

			new (m_array + m_tail * sizeof(T)) T(arg1, arg2, arg3);
			m_tail = (m_tail + 1) % m_capacity;
			m_size++;
        }	

		void emplace_front()
        {
			assert(m_array && m_size < m_capacity);

			m_head = ((m_head - 1) % m_capacity + m_capacity) % m_capacity;
			new (m_array + m_head * sizeof(T)) T();
			m_size++;
		}

		template <typename Arg1 >
		void emplace_front(Arg1& arg1)
		{
			assert(m_array && m_size < m_capacity);

			m_head = ((m_head - 1) % m_capacity + m_capacity) % m_capacity;
			new (m_array + m_head * sizeof(T)) T(arg1);
			m_size++;
        }

        template <typename Arg1, typename Arg2 >
        void emplace_front(Arg1& arg1, Arg2& arg2)
        {
			assert(m_array && m_size < m_capacity);

			m_head = ((m_head - 1) % m_capacity + m_capacity) % m_capacity;
			new (m_array + m_head * sizeof(T)) T(arg1, arg2);
			m_size++;
        }

        template <typename Arg1, typename Arg2 , typename Arg3 >
        void emplace_front(Arg1& arg1, Arg2& arg2, Arg3& arg3)
        {
			assert(m_array && m_size < m_capacity);

			m_head = ((m_head - 1) % m_capacity + m_capacity) % m_capacity;
			new (m_array + m_head * sizeof(T)) T(arg1, arg2, arg3);
			m_size++;
        }

		template <typename Arg1 >
		void emplace_front(const Arg1& arg1)
		{
			assert(m_array && m_size < m_capacity);

			m_head = ((m_head - 1) % m_capacity + m_capacity) % m_capacity;
			new (m_array + m_head * sizeof(T)) T(arg1);
			m_size++;
        }

        template <typename Arg1, typename Arg2 >
        void emplace_front(const Arg1& arg1, const Arg2& arg2)
        {
			assert(m_array && m_size < m_capacity);

			m_head = ((m_head - 1) % m_capacity + m_capacity) % m_capacity;
			new (m_array + m_head * sizeof(T)) T(arg1, arg2);
			m_size++;
        }

        template <typename Arg1, typename Arg2 , typename Arg3 >
        void emplace_front(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3)
        {
			assert(m_array && m_size < m_capacity);

			m_head = ((m_head - 1) % m_capacity + m_capacity) % m_capacity;
			new (m_array + m_head * sizeof(T)) T(arg1, arg2, arg3);
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

				iterator(pointer array, size_t capacity, size_t index)
					: m_array(array), m_capacity(capacity), m_index(index) {}

				iterator(const iterator& other)
					: m_array(other.m_array), m_capacity(other.m_capacity), m_index(other.m_index) {}

				~iterator() {}

				iterator& operator=(const iterator& other)
				{
					if (this != &other)
					{
						m_array = other.m_array;
						m_capacity = other.m_capacity;
						m_index = other.m_index;
					}
					return (*this);
				}

				reference operator*() const { return (m_array[m_index * sizeof(T)]); }
				pointer operator->() const { return (&m_array[m_index * sizeof(T)]); }

				iterator& operator++()
				{
					m_index = (m_index + 1) % m_capacity;
					return (*this);
				}

				iterator operator++(int)
				{
					iterator tmp = *this;
					++(*this);
					return (tmp);
				}

				iterator& operator--()
				{
					m_index = ((m_index - 1) % m_capacity + m_capacity) % m_capacity;
					return *(this);
				}

				iterator operator--(int)
				{
					iterator tmp = *this;
					--(*this);
					return (tmp);
				}

				reference operator[](difference_type offset) const
				{
					size_t logical_index = (m_index + offset) % m_capacity;
					return (m_array[logical_index * sizeof(T)]);
				}

				iterator operator+(difference_type offset) const
				{
					size_t logical_index = (m_index + offset) % m_capacity;
					return (iterator(m_array, m_capacity, logical_index));
				}

				iterator operator-(difference_type offset) const
				{
					size_t logical_index = ((m_index - offset) % m_capacity + m_capacity) % m_capacity; // Wrap-around
					return (iterator(m_array, m_capacity, logical_index));
				}

				difference_type operator-(const iterator& other) const
				{
					return static_cast<difference_type>(m_index) - static_cast<difference_type>(other.m_index);
				}

				iterator& operator+=(difference_type offset)
				{
					m_index = (m_index + offset) % m_capacity; // Wrap-around
					return (*this);
				}

				iterator& operator-=(difference_type offset)
				{
					m_index = ((m_index - offset) % m_capacity + m_capacity) % m_capacity; // Wrap-around
					return (*this);
				}

				bool operator==(const iterator& other) const { return (m_index == other.m_index); }
				bool operator!=(const iterator& other) const { return (!(*this == other)); }

			private:
				pointer m_array;       // Pointer to the array
				size_t m_capacity;     // Capacity of the Circular Buffer
				size_t m_index;        // Logical index within the Circular Buffer
		};

		iterator begin() 
		{ 
			return iterator(reinterpret_cast<T*>(m_array), m_capacity, m_head); 
		}

		iterator end() 
		{ 
			return iterator(reinterpret_cast<T*>(m_array), m_capacity, m_tail); 
		}

	private:
		typedef unsigned char 		t_byte;
		
		Allocator					m_allocator;
		t_byte*						m_array;

		size_t						m_head;
		size_t						m_tail;
		size_t						m_size;
		const size_t				m_capacity;
};


#endif
