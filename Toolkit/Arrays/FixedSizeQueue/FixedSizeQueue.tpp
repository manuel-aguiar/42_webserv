/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FixedSizeQueue.tpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 13:26:42 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/02 18:52:57 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FIXEDSIZEQUEUE_TPP

# define FIXEDSIZEQUEUE_TPP

# include <cassert>
# include <iostream>

template <typename T, typename Allocator>
class FixedSizeQueue
{
	public:
		FixedSizeQueue(const size_t capacity = 0, const Allocator& allocator = Allocator()) : 
			m_allocator(allocator),
			m_array(m_allocator.allocate(capacity)),
			m_head(0),
			m_tail(0), 
			m_capacity(capacity),
			m_isFull(false)
		{
			assert(capacity);
		}


		FixedSizeQueue(const FixedSizeQueue &other) : 
			m_allocator(other.m_allocator),
			m_array(m_allocator.allocate(other.m_capacity)), 
			m_capacity(other.m_capacity)
		{
			*this = other;
		}

		~FixedSizeQueue()
		{
			clear();
			m_allocator.deallocate(m_array, m_capacity);
		}

		FixedSizeQueue &operator=(const FixedSizeQueue &other)
		{
			assert(m_capacity == other.m_capacity);


			std::cout << "my capacity: " << m_capacity << " other capacity: " << other.m_capacity << std::endl;
			
			std::cout << "copy assignment called" << std::endl;
			std::cout << "mySize: " << size() << " otherSize: " << other.size() << std::endl;
			if (this == &other)
				return (*this);

			if (other.size() == 0)
			{
				mf_destroyAll();
				m_head = other.m_head;
				m_tail = other.m_tail;

				std::cout << "other is empty" << std::endl;

				return (*this);
			}

			if (size() == 0)
			{
				for (size_t i = other.m_head; i != other.m_tail;)
				{
					new (m_array + i) T(other.m_array[i]);
					i = (i + 1) % m_capacity;
				}
				m_head = other.m_head;
				m_tail = other.m_tail;

				std::cout << "this is empty" << std::endl;
				return (*this);
			}

			m_head = other.m_head;
			m_tail = other.m_tail;
			std::cout << "this is not empty but hey" << std::endl;
			return (*this);
		}


		T& operator[](const size_t index)
		{
			assert(index < m_capacity);

			size_t position;

			position = (m_head + index) % m_capacity;
			return (m_array[position]);
		}

		const T& operator[](const size_t index) const
		{
			assert(index < m_capacity);
			
			size_t position;

			position = (m_head + index) % m_capacity;
			return (m_array[position]);
		}

		size_t size() const
		{
			if (m_isFull)
				return m_capacity;
			else if (m_head <= m_tail)
				return m_tail - m_head;
			else 
				return m_capacity - (m_head - m_tail);
		}

		size_t capacity()
		{
			return (m_capacity);
		}

		void clear()
		{
			mf_destroyAll();
			
			m_head = 0;
			m_tail = 0;
		}

		const Allocator& getAllocator() const
		{
			return (m_allocator);
		}

        T& at(size_t index)
        {
            assert (size() != 0 && index < size());

			size_t position;

			position = (m_head + index) % m_capacity;
			return (m_array[position]);
        }

		T& front()
		{
			assert (size() != 0);
			
			return (m_array[m_head]);
		}

		T& back()
		{
			assert (size() != 0);

			size_t position;

			position = (m_tail == 0) ? m_capacity - 1 : m_tail - 1;

			return (m_array[position]);
		}

		// m_tail is one past last (iter::end()), so the new one will be exactly there
		// so construct first, adjust after
		void push_back(const T& value)
		{
			assert(m_array && size() < m_capacity);

			new (m_array + m_tail) T(value);
			m_tail = (m_tail + 1) % m_capacity;
			m_isFull = (m_tail == m_head);
		}

		// m_tail is one past last (iter::end()), adjust first, delete after
		void pop_back()
		{
			assert (size() != 0);
			
			m_tail = (m_tail == 0) ? m_capacity - 1 : m_tail - 1;
			m_allocator.destroy(&m_array[m_tail]);
			m_isFull = false;
		}

		//m_head is already the first, so we adjust first, construct after
		void push_front(const T& value)
		{
			assert(m_array && size() < m_capacity);
			
			m_head = (m_head == 0) ? m_capacity - 1 : m_head - 1;

			new (m_array + m_head) T(value);
			m_isFull = (m_tail == m_head);
		}

		// m_head is the first, delete first, adjust after
		void pop_front()
		{
			assert (size() != 0);
			
			m_allocator.destroy(&m_array[m_head]);
			m_head = (m_head + 1) % m_capacity;
			m_isFull = (m_tail == m_head);
		}

		void emplace_back()
        {
			assert(m_array && size() < m_capacity);

			new (m_array + m_tail) T();
			m_tail = (m_tail + 1) % m_capacity;
			m_isFull = (m_tail == m_head);
		}

		template <typename Arg1 >
		void emplace_back(Arg1& arg1)
		{
			assert(m_array && size() < m_capacity);

			new (m_array + m_tail) T(arg1);
			m_tail = (m_tail + 1) % m_capacity;
			m_isFull = (m_tail == m_head);
        }

        template <typename Arg1, typename Arg2 >
        void emplace_back(Arg1& arg1, Arg2& arg2)
        {
			assert(m_array && size() < m_capacity);

			new (m_array + m_tail) T(arg1, arg2);
			m_tail = (m_tail + 1) % m_capacity;
			m_isFull = (m_tail == m_head);
        }

        template <typename Arg1, typename Arg2 , typename Arg3 >
        void emplace_back(Arg1& arg1, Arg2& arg2, Arg3& arg3)
        {
			assert(m_array && size() < m_capacity);

			new (m_array + m_tail) T(arg1, arg2, arg3);
			m_tail = (m_tail + 1) % m_capacity;
			m_isFull = (m_tail == m_head);
        }

		template <typename Arg1 >
		void emplace_back(const Arg1& arg1)
		{
			assert(m_array && size() < m_capacity);

			new (m_array + m_tail) T(arg1);
			m_tail = (m_tail + 1) % m_capacity;
			m_isFull = (m_tail == m_head);
        }

        template <typename Arg1, typename Arg2 >
        void emplace_back(const Arg1& arg1, const Arg2& arg2)
        {
			assert(m_array && size() < m_capacity);

			new (m_array + m_tail) T(arg1, arg2);
			m_tail = (m_tail + 1) % m_capacity;
			m_isFull = (m_tail == m_head);
        }

        template <typename Arg1, typename Arg2 , typename Arg3 >
        void emplace_back(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3)
        {
			assert(m_array && size() < m_capacity);

			new (m_array + m_tail) T(arg1, arg2, arg3);
			m_tail = (m_tail + 1) % m_capacity;
			m_isFull = (m_tail == m_head);
        }	

		void emplace_front()
        {
			assert(m_array && size() < m_capacity);

			m_head = (m_head == 0) ? m_capacity - 1 : m_head - 1;
			new (m_array + m_head) T();
			m_isFull = (m_tail == m_head);
		}

		template <typename Arg1 >
		void emplace_front(Arg1& arg1)
		{
			assert(m_array && size() < m_capacity);

			m_head = (m_head == 0) ? m_capacity - 1 : m_head - 1;
			new (m_array + m_head) T(arg1);
			m_isFull = (m_tail == m_head);
        }

        template <typename Arg1, typename Arg2 >
        void emplace_front(Arg1& arg1, Arg2& arg2)
        {
			assert(m_array && size() < m_capacity);

			m_head = (m_head == 0) ? m_capacity - 1 : m_head - 1;
			new (m_array + m_head) T(arg1, arg2);
			m_isFull = (m_tail == m_head);
        }

        template <typename Arg1, typename Arg2 , typename Arg3 >
        void emplace_front(Arg1& arg1, Arg2& arg2, Arg3& arg3)
        {
			assert(m_array && size() < m_capacity);

			m_head = (m_head == 0) ? m_capacity - 1 : m_head - 1;
			new (m_array + m_head) T(arg1, arg2, arg3);
			m_isFull = (m_tail == m_head);
        }

		template <typename Arg1 >
		void emplace_front(const Arg1& arg1)
		{
			assert(m_array && size() < m_capacity);

			m_head = (m_head == 0) ? m_capacity - 1 : m_head - 1;
			new (m_array + m_head) T(arg1);
			m_isFull = (m_tail == m_head);
        }

        template <typename Arg1, typename Arg2 >
        void emplace_front(const Arg1& arg1, const Arg2& arg2)
        {
			assert(m_array && size() < m_capacity);

			m_head = (m_head == 0) ? m_capacity - 1 : m_head - 1;
			new (m_array + m_head) T(arg1, arg2);
			m_isFull = (m_tail == m_head);
        }

        template <typename Arg1, typename Arg2 , typename Arg3 >
        void emplace_front(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3)
        {
			assert(m_array && size() < m_capacity);

			m_head = (m_head == 0) ? m_capacity - 1 : m_head - 1;
			new (m_array + m_head) T(arg1, arg2, arg3);
			m_isFull = (m_tail == m_head);
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

				reference operator*() const { return (m_array[m_index]); }
				pointer operator->() const { return (&m_array[m_index]); }

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
					m_index = (m_index == 0) ? m_capacity - 1 : m_index - 1; // Wrap-around
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
					return (m_array[logical_index]);
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
					m_index = (m_index + offset) % m_capacity;
					return (*this);
				}
/*
				iterator& operator-=(difference_type offset)
				{
					m_index = (m_index == 0) ? m_capacity - 1 : m_index - 1;
					return (*this);
				}
*/
				bool operator==(const iterator& other) const { return (m_index == other.m_index); }
				bool operator!=(const iterator& other) const { return (!(*this == other)); }

			private:
				pointer			m_array;       
				size_t			m_capacity;     
				size_t			m_index;
				size_t			m_tail;
		};

		iterator begin() 
		{ 
			return iterator(m_array, m_capacity, m_head); 
		}

		iterator end() 
		{ 
			return iterator(m_array, m_capacity, m_tail); 
		}

	private:
		typedef unsigned char 		t_byte;
		
		Allocator					m_allocator;
		T*							m_array;

		size_t						m_head;
		size_t						m_tail;
		size_t						m_capacity;

		bool						m_isFull;

		void	mf_destroyAll()
		{
			if (m_head == m_tail && !m_isFull)
				return ;
			for (size_t i = m_head; i != m_tail;)
			{
				m_allocator.destroy(&m_array[i]);
				i = (i + 1) % m_capacity;
			}
		}
};

#endif
