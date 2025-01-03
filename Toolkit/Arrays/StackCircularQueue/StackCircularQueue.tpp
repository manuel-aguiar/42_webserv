/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StackCircularQueue.tpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 13:26:42 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/03 13:19:17 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STACKCIRCULARQUEUE_TPP

# define STACKCIRCULARQUEUE_TPP

# include <cassert>
# include <iostream>

template <typename T, size_t queueCapacity>
class StackCircularQueue
{
	public:
		StackCircularQueue() : 
			m_typeArray(reinterpret_cast<T*>(m_byteArray)),
			m_frontIndex(-1),
			m_backIndex(-1)
		{
			for (size_t i = 0; i < queueCapacity; ++i)
			{
				new(&m_typeArray[i]) T();
			}
		}

		// user can avoid default construction by initializing copies, buffer will stiil have "zero" elements inserted
		StackCircularQueue(const T& copy) : 
			m_typeArray(reinterpret_cast<T*>(m_byteArray)),
			m_frontIndex(-1),
			m_backIndex(-1)
		{
			for (size_t i = 0; i < queueCapacity; ++i)
			{
				new(&m_typeArray[i]) T(copy);
			}
		}

		StackCircularQueue(const StackCircularQueue &other) :
			m_typeArray(reinterpret_cast<T*>(m_byteArray))
		{
			for (size_t i = 0; i < queueCapacity; ++i)
				new(&m_typeArray[i]) T(other.m_typeArray[i]);
			m_frontIndex = other.m_frontIndex;
			m_backIndex = other.m_backIndex;
		}

		~StackCircularQueue()
		{
			mf_destroyAll();
		}

		StackCircularQueue &operator=(const StackCircularQueue &other)
		{
			int srcIndex;
			int destIndex;
			size_t otherSize = other.size();

			m_frontIndex = other.m_frontIndex;
			for (size_t i = 0; i < otherSize; ++i)
			{
				srcIndex = (other.m_frontIndex + i) % queueCapacity;
				destIndex = (m_frontIndex + i) % queueCapacity;
				m_typeArray[destIndex] = other.m_typeArray[srcIndex];
			}
			
			m_frontIndex = other.m_frontIndex;
			m_backIndex = other.m_backIndex;

			return (*this);
		}


		T& operator[](const size_t index)
		{
			assert(index < queueCapacity);

			size_t position;

			position = (m_frontIndex + index) % queueCapacity;
			return (m_typeArray[position]);
		}

		const T& operator[](const size_t index) const
		{
			assert(index < queueCapacity);
			
			size_t position;

			position = (m_frontIndex + index) % queueCapacity;
			return (m_typeArray[position]);
		}

		size_t size() const
		{
			if (isFull())
				return queueCapacity;
			else if (m_frontIndex <= m_backIndex)
				return m_backIndex - m_frontIndex;
			else 
				return queueCapacity - (m_frontIndex - m_backIndex);
		}

		size_t capacity()
		{
			return (queueCapacity);
		}

		void clear()
		{
			m_frontIndex = -1;
			m_backIndex = -1;
		}
		
        T& at(size_t index)
        {
            assert (!isEmpty() && index < size());

			size_t position;

			position = (m_frontIndex + index) % queueCapacity;
			return (m_typeArray[position]);
        }

		T& front()
		{
			assert (!isEmpty());
			
			return (m_typeArray[m_frontIndex]);
		}

		T& back()
		{
			assert (!isEmpty());

			size_t position;

			position = (m_backIndex == 0) ? queueCapacity - 1 : m_backIndex - 1;

			return (m_typeArray[position]);
		}

		// m_backIndex is one past last (iter::end()), so the new one will be exactly there
		// so construct first, adjust after
		bool push_back(const T& value)
		{
			if (!mf_PushPrepare())
				return (false);
			m_typeArray[m_backIndex] = value;
			m_backIndex = (m_backIndex + 1) % queueCapacity;
			return (true);
		}

		// m_backIndex is one past last (iter::end()), adjust first, delete after
		bool pop_back()
		{
			if (isEmpty())
				return (false);
			
			m_backIndex = (m_backIndex == 0) ? queueCapacity - 1 : m_backIndex - 1;
			mf_PopResetIndexes();
			return (true);
		}



		//m_frontIndex is already the first, so we adjust first, construct after
		bool push_front(const T& value)
		{
			if (!mf_PushPrepare())
				return (false);
			
			m_frontIndex = (m_frontIndex == 0) ? queueCapacity - 1 : m_frontIndex - 1;
			m_typeArray[m_frontIndex] = value;

			return (true);
		}

		// m_frontIndex is the first, delete first, adjust after
		bool pop_front()
		{
			if (isEmpty())
				return (false);
			
			m_frontIndex = (m_frontIndex + 1) % queueCapacity;
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

		bool emplace_back()
        {
			if (!mf_PushPrepare())
				return (false);
			m_typeArray[m_backIndex].~T();
			new (m_typeArray + m_backIndex) T();
			m_backIndex = (m_backIndex + 1) % queueCapacity;
			return (true);
		}

		template <typename Arg1 >
		bool emplace_back(Arg1& arg1)
		{
			if (!mf_PushPrepare())
				return (false);
			m_typeArray[m_backIndex].~T();
			new (m_typeArray + m_backIndex) T(arg1);
			m_backIndex = (m_backIndex + 1) % queueCapacity;
			return (true);
        }

        template <typename Arg1, typename Arg2 >
        bool emplace_back(Arg1& arg1, Arg2& arg2)
        {
			if (!mf_PushPrepare())
				return (false);
			m_typeArray[m_backIndex].~T();
			new (m_typeArray + m_backIndex) T(arg1, arg2);
			m_backIndex = (m_backIndex + 1) % queueCapacity;
			return (true);
        }

        template <typename Arg1, typename Arg2 , typename Arg3 >
        bool emplace_back(Arg1& arg1, Arg2& arg2, Arg3& arg3)
        {
			if (!mf_PushPrepare())
				return (false);
			m_typeArray[m_backIndex].~T();
			new (m_typeArray + m_backIndex) T(arg1, arg2, arg3);
			m_backIndex = (m_backIndex + 1) % queueCapacity;
			return (true);
        }

		template <typename Arg1 >
		bool emplace_back(const Arg1& arg1)
		{
			if (!mf_PushPrepare())
				return (false);
			m_typeArray[m_backIndex].~T();
			new (m_typeArray + m_backIndex) T(arg1);
			m_backIndex = (m_backIndex + 1) % queueCapacity;
			return (true);
        }

        template <typename Arg1, typename Arg2 >
        bool emplace_back(const Arg1& arg1, const Arg2& arg2)
        {
			if (!mf_PushPrepare())
				return (false);
			m_typeArray[m_backIndex].~T();
			new (m_typeArray + m_backIndex) T(arg1, arg2);
			m_backIndex = (m_backIndex + 1) % queueCapacity;
			return (true);
        }

        template <typename Arg1, typename Arg2 , typename Arg3 >
        bool emplace_back(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3)
        {
			if (!mf_PushPrepare())
				return (false);
			m_typeArray[m_backIndex].~T();
			new (m_typeArray + m_backIndex) T(arg1, arg2, arg3);
			m_backIndex = (m_backIndex + 1) % queueCapacity;
			return (true);
        }	

		bool emplace_front()
        {
			if (!mf_PushPrepare())
				return (false);
			m_frontIndex = (m_frontIndex == 0) ? queueCapacity - 1 : m_frontIndex - 1;
			m_typeArray[m_frontIndex].~T();
			new (m_typeArray + m_frontIndex) T();
			return (true);
		}

		template <typename Arg1 >
		bool emplace_front(Arg1& arg1)
		{
			if (!mf_PushPrepare())
				return (false);

			m_frontIndex = (m_frontIndex == 0) ? queueCapacity - 1 : m_frontIndex - 1;
			m_typeArray[m_frontIndex].~T();
			new (m_typeArray + m_frontIndex) T(arg1);
			return (true);
        }

        template <typename Arg1, typename Arg2 >
        bool emplace_front(Arg1& arg1, Arg2& arg2)
        {
			if (!mf_PushPrepare())
				return (false);

			m_frontIndex = (m_frontIndex == 0) ? queueCapacity - 1 : m_frontIndex - 1;
			m_typeArray[m_frontIndex].~T();
			new (m_typeArray + m_frontIndex) T(arg1, arg2);
			return (true);
		}
		template <typename Arg1 >
		bool emplace_front(const Arg1& arg1)
		{
			if (!mf_PushPrepare())
				return (false);

			m_frontIndex = (m_frontIndex == 0) ? queueCapacity - 1 : m_frontIndex - 1;
			m_typeArray[m_frontIndex].~T();
			new (m_typeArray + m_frontIndex) T(arg1);
			return (true);
        }

        template <typename Arg1, typename Arg2 >
        bool emplace_front(const Arg1& arg1, const Arg2& arg2)
        {
			if (!mf_PushPrepare())
				return (false);

			m_frontIndex = (m_frontIndex == 0) ? queueCapacity - 1 : m_frontIndex - 1;
			m_typeArray[m_frontIndex].~T();
			new (m_typeArray + m_frontIndex) T(arg1, arg2);
			return (true);
        }

        template <typename Arg1, typename Arg2 , typename Arg3 >
        bool emplace_front(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3)
        {
			if (!mf_PushPrepare())
				return (false);

			m_frontIndex = (m_frontIndex == 0) ? queueCapacity - 1 : m_frontIndex - 1;
			m_typeArray[m_frontIndex].~T();
			new (m_typeArray + m_frontIndex) T(arg1, arg2, arg3);
			return (true);
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
					: m_typeArray(array), m_capacity(capacity), m_backIndex(tail), m_index(index) {}

				iterator(const iterator& other)
					: m_typeArray(other.m_typeArray), m_capacity(other.m_capacity), m_backIndex(other.m_backIndex), m_index(other.m_index) {}

				~iterator() {}

				iterator& operator=(const iterator& other)
				{
					if (this != &other)
					{
						m_typeArray = other.m_typeArray;
						m_capacity = other.m_capacity;
						m_index = other.m_index;
						m_backIndex = other.m_backIndex;
					}
					return (*this);
				}

				reference operator*() const { return (m_typeArray[m_index]); }
				pointer operator->() const { return (&m_typeArray[m_index]); }

				iterator& operator++()
				{
					assert (m_index != -1);
					m_index = (m_index + 1) % m_capacity;

					//reached the back
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
				pointer			m_typeArray;       
				int				m_capacity;     
				int				m_backIndex;
				int				m_index;
		};

		iterator begin() 
		{ 
			return iterator(m_typeArray, queueCapacity, m_backIndex, m_frontIndex); 
		}

		iterator end() 
		{ 
			return iterator(m_typeArray, queueCapacity, m_backIndex, -1); 
		}

	private:
		// private member variables
		typedef unsigned char		t_byte;	
		t_byte						m_byteArray[queueCapacity * sizeof(T)];
		T*							m_typeArray;
		int							m_frontIndex;
		int							m_backIndex;

		// helper functions		

		void	mf_destroyAll()
		{
			for (size_t i = 0; i < queueCapacity; ++i)
				m_typeArray[i].~T();
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

		bool mf_PushPrepare()
		{
			if (mf_FrontEqualsBack())
			{
				if (m_frontIndex != -1)
					return (false);
				m_frontIndex = m_backIndex = 0;
			}
			return (true);
		}
};

#endif