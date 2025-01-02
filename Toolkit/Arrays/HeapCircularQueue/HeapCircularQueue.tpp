/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeapCircularQueue.tpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 13:26:42 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/02 23:15:28 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEAPCIRCULARQUEUE_TPP

# define HEAPCIRCULARQUEUE_TPP

# include <cassert>
# include <iostream>

template <typename T, typename Allocator>
class HeapCircularQueue
{
	public:
		HeapCircularQueue(const size_t capacity = 0, const Allocator& allocator = Allocator()) : 
			m_allocator(allocator),
			m_array(m_allocator.allocate(capacity)),
			m_frontIndex(-1),
			m_backIndex(-1), 
			m_capacity(capacity)
		{
			assert(capacity);
		}


		HeapCircularQueue(const HeapCircularQueue &other) : 
			m_allocator(other.m_allocator),
			m_array(m_allocator.allocate(other.m_capacity)), 
			m_capacity(other.m_capacity)
		{
			*this = other;
		}

		~HeapCircularQueue()
		{
			clear();
			m_allocator.deallocate(m_array, m_capacity);
		}

		HeapCircularQueue &operator=(const HeapCircularQueue &other)
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
				m_frontIndex = other.m_frontIndex;
				m_backIndex = other.m_backIndex;

				std::cout << "other is empty" << std::endl;

				return (*this);
			}

			if (size() == 0)
			{
				for (int i = other.m_frontIndex; i != other.m_backIndex;)
				{
					new (m_array + i) T(other.m_array[i]);
					i = (i + 1) % m_capacity;
				}
				m_frontIndex = other.m_frontIndex;
				m_backIndex = other.m_backIndex;

				std::cout << "this is empty" << std::endl;
				return (*this);
			}

			m_frontIndex = other.m_frontIndex;
			m_backIndex = other.m_backIndex;
			std::cout << "this is not empty but hey" << std::endl;
			return (*this);
		}


		T& operator[](const size_t index)
		{
			assert((int)index < m_capacity);

			size_t position;

			position = (m_frontIndex + index) % m_capacity;
			return (m_array[position]);
		}

		const T& operator[](const size_t index) const
		{
			assert(index < m_capacity);
			
			size_t position;

			position = (m_frontIndex + index) % m_capacity;
			return (m_array[position]);
		}

		size_t size() const
		{
			if (isFull())
				return m_capacity;
			else if (m_frontIndex <= m_backIndex)
				return m_backIndex - m_frontIndex;
			else 
				return m_capacity - (m_frontIndex - m_backIndex);
		}

		size_t capacity()
		{
			return (m_capacity);
		}

		void clear()
		{
			mf_destroyAll();
			
			m_frontIndex = 0;
			m_backIndex = 0;
		}

		const Allocator& getAllocator() const
		{
			return (m_allocator);
		}

        T& at(size_t index)
        {
            assert (!isEmpty() && index < size());

			size_t position;

			position = (m_frontIndex + index) % m_capacity;
			return (m_array[position]);
        }

		T& front()
		{
			assert (!isEmpty());
			
			return (m_array[m_frontIndex]);
		}

		T& back()
		{
			assert (!isEmpty());

			size_t position;

			position = (m_backIndex == 0) ? m_capacity - 1 : m_backIndex - 1;

			return (m_array[position]);
		}

		// m_backIndex is one past last (iter::end()), so the new one will be exactly there
		// so construct first, adjust after
		bool push_back(const T& value)
		{
			if (mf_FrontEqualsBack())
			{
				if (m_frontIndex != -1)
					return (false);
				m_frontIndex = m_backIndex = 0;
			}
			new (m_array + m_backIndex) T(value);
			m_backIndex = (m_backIndex + 1) % m_capacity;
			return (true);
		}

		// m_backIndex is one past last (iter::end()), adjust first, delete after
		bool pop_back()
		{
			if (isEmpty())
				return (false);
			
			m_backIndex = (m_backIndex == 0) ? m_capacity - 1 : m_backIndex - 1;
			m_allocator.destroy(&m_array[m_backIndex]);
			mf_PopResetIndexes();
			return (true);
		}



		//m_frontIndex is already the first, so we adjust first, construct after
		bool push_front(const T& value)
		{
			if (mf_FrontEqualsBack())
			{
				if (m_frontIndex != -1)
					return (false);
				m_frontIndex = m_backIndex = 0;
			}
			
			m_frontIndex = (m_frontIndex == 0) ? m_capacity - 1 : m_frontIndex - 1;
			new (m_array + m_frontIndex) T(value);

			//m_isFull = (m_backIndex == m_frontIndex);

			return (true);
		}

		// m_frontIndex is the first, delete first, adjust after
		bool pop_front()
		{
			if (isEmpty())
				return (false);
			
			m_allocator.destroy(&m_array[m_frontIndex]);
			m_frontIndex = (m_frontIndex + 1) % m_capacity;
			mf_PopResetIndexes();
			return (true);
		}


		bool isEmpty() const
		{
			return (m_frontIndex == m_backIndex && m_frontIndex == -1);
		}

		bool isFull() const
		{
			return (m_frontIndex == m_backIndex && m_frontIndex != -1);
		}


		class iterator
		{
			public:
				typedef T                                   value_type;
				typedef T*                                  pointer;
				typedef T&                                  reference;
				typedef std::ptrdiff_t                      difference_type;
				typedef std::random_access_iterator_tag     iterator_category;

				iterator(pointer array, size_t capacity, size_t tail, size_t index)
					: m_array(array), m_capacity(capacity), m_backIndex(tail), m_index(index) {}

				iterator(const iterator& other)
					: m_array(other.m_array), m_capacity(other.m_capacity), m_backIndex(other.m_backIndex), m_index(other.m_index) {}

				~iterator() {}

				iterator& operator=(const iterator& other)
				{
					if (this != &other)
					{
						m_array = other.m_array;
						m_capacity = other.m_capacity;
						m_index = other.m_index;
						m_backIndex = other.m_backIndex;
					}
					return (*this);
				}

				reference operator*() const { return (m_array[m_index]); }
				pointer operator->() const { return (&m_array[m_index]); }

				iterator& operator++()
				{
					assert (m_index != -1);
					m_index = (m_index + 1) % m_capacity;
					m_index = (m_index == m_backIndex) ? -1 : m_index;
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
					m_index = (m_index == -1) ? m_backIndex - 1 : m_index; // Wrap-around
					m_index = (m_index == 0) ? m_capacity - 1 : m_index - 1; // Wrap-around
					return *(this);
				}

				iterator operator--(int)
				{
					iterator tmp = *this;
					--(*this);
					return (tmp);
				}

				bool operator==(const iterator& other) const { return (m_index == other.m_index); }
				bool operator!=(const iterator& other) const { return (!(*this == other)); }

			private:
				pointer			m_array;       
				int				m_capacity;     
				int				m_backIndex;
				int				m_index;
		};

		iterator begin() 
		{ 
			return iterator(m_array, m_capacity, m_backIndex, m_frontIndex); 
		}

		iterator end() 
		{ 
			return iterator(m_array, m_capacity, m_backIndex, -1); 
		}

	private:
		// private member variables
		Allocator					m_allocator;
		T*							m_array;

		int							m_frontIndex;
		int							m_backIndex;
		int							m_capacity;

		// helper functions
		
		void	mf_destroyAll()
		{
			if (isEmpty())
				return ;
			for (int i = m_frontIndex; i != m_backIndex;)
			{
				m_allocator.destroy(&m_array[i]);
				i = (i + 1) % m_capacity;
			}
		}

		void	mf_PopResetIndexes()
		{
			if (m_frontIndex != m_backIndex)
				return ;
			m_frontIndex = -1;
			m_backIndex = -1;
		}
		
		bool mf_FrontEqualsBack() const
		{
			return (m_frontIndex == m_backIndex);
		}
};

#endif


/*
		void emplace_back()
        {
			assert(m_array && size() < m_capacity);

			new (m_array + m_backIndex) T();
			m_backIndex = (m_backIndex + 1) % m_capacity;
			m_isFull = (m_backIndex == m_frontIndex);
		}

		template <typename Arg1 >
		void emplace_back(Arg1& arg1)
		{
			assert(m_array && size() < m_capacity);

			new (m_array + m_backIndex) T(arg1);
			m_backIndex = (m_backIndex + 1) % m_capacity;
			m_isFull = (m_backIndex == m_frontIndex);
        }

        template <typename Arg1, typename Arg2 >
        void emplace_back(Arg1& arg1, Arg2& arg2)
        {
			assert(m_array && size() < m_capacity);

			new (m_array + m_backIndex) T(arg1, arg2);
			m_backIndex = (m_backIndex + 1) % m_capacity;
			m_isFull = (m_backIndex == m_frontIndex);
        }

        template <typename Arg1, typename Arg2 , typename Arg3 >
        void emplace_back(Arg1& arg1, Arg2& arg2, Arg3& arg3)
        {
			assert(m_array && size() < m_capacity);

			new (m_array + m_backIndex) T(arg1, arg2, arg3);
			m_backIndex = (m_backIndex + 1) % m_capacity;
			m_isFull = (m_backIndex == m_frontIndex);
        }

		template <typename Arg1 >
		void emplace_back(const Arg1& arg1)
		{
			assert(m_array && size() < m_capacity);

			new (m_array + m_backIndex) T(arg1);
			m_backIndex = (m_backIndex + 1) % m_capacity;
			m_isFull = (m_backIndex == m_frontIndex);
        }

        template <typename Arg1, typename Arg2 >
        void emplace_back(const Arg1& arg1, const Arg2& arg2)
        {
			assert(m_array && size() < m_capacity);

			new (m_array + m_backIndex) T(arg1, arg2);
			m_backIndex = (m_backIndex + 1) % m_capacity;
			m_isFull = (m_backIndex == m_frontIndex);
        }

        template <typename Arg1, typename Arg2 , typename Arg3 >
        void emplace_back(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3)
        {
			assert(m_array && size() < m_capacity);

			new (m_array + m_backIndex) T(arg1, arg2, arg3);
			m_backIndex = (m_backIndex + 1) % m_capacity;
			m_isFull = (m_backIndex == m_frontIndex);
        }	

		void emplace_front()
        {
			assert(m_array && size() < m_capacity);

			m_frontIndex = (m_frontIndex == 0) ? m_capacity - 1 : m_frontIndex - 1;
			new (m_array + m_frontIndex) T();
			m_isFull = (m_backIndex == m_frontIndex);
		}

		template <typename Arg1 >
		void emplace_front(Arg1& arg1)
		{
			assert(m_array && size() < m_capacity);

			m_frontIndex = (m_frontIndex == 0) ? m_capacity - 1 : m_frontIndex - 1;
			new (m_array + m_frontIndex) T(arg1);
			m_isFull = (m_backIndex == m_frontIndex);
        }

        template <typename Arg1, typename Arg2 >
        void emplace_front(Arg1& arg1, Arg2& arg2)
        {
			assert(m_array && size() < m_capacity);

			m_frontIndex = (m_frontIndex == 0) ? m_capacity - 1 : m_frontIndex - 1;
			new (m_array + m_frontIndex) T(arg1, arg2);
			m_isFull = (m_backIndex == m_frontIndex);
        }

        template <typename Arg1, typename Arg2 , typename Arg3 >
        void emplace_front(Arg1& arg1, Arg2& arg2, Arg3& arg3)
        {
			assert(m_array && size() < m_capacity);

			m_frontIndex = (m_frontIndex == 0) ? m_capacity - 1 : m_frontIndex - 1;
			new (m_array + m_frontIndex) T(arg1, arg2, arg3);
			m_isFull = (m_backIndex == m_frontIndex);
        }

		template <typename Arg1 >
		void emplace_front(const Arg1& arg1)
		{
			assert(m_array && size() < m_capacity);

			m_frontIndex = (m_frontIndex == 0) ? m_capacity - 1 : m_frontIndex - 1;
			new (m_array + m_frontIndex) T(arg1);
			m_isFull = (m_backIndex == m_frontIndex);
        }

        template <typename Arg1, typename Arg2 >
        void emplace_front(const Arg1& arg1, const Arg2& arg2)
        {
			assert(m_array && size() < m_capacity);

			m_frontIndex = (m_frontIndex == 0) ? m_capacity - 1 : m_frontIndex - 1;
			new (m_array + m_frontIndex) T(arg1, arg2);
			m_isFull = (m_backIndex == m_frontIndex);
        }

        template <typename Arg1, typename Arg2 , typename Arg3 >
        void emplace_front(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3)
        {
			assert(m_array && size() < m_capacity);

			m_frontIndex = (m_frontIndex == 0) ? m_capacity - 1 : m_frontIndex - 1;
			new (m_array + m_frontIndex) T(arg1, arg2, arg3);
			m_isFull = (m_backIndex == m_frontIndex);
        }	
*/

/*
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

				iterator& operator-=(difference_type offset)
				{
					m_index = (m_index == 0) ? m_capacity - 1 : m_index - 1;
					return (*this);
				}
*/