

#ifndef FIXEDCIRCULARQUEUEIMPL_TPP

# define FIXEDCIRCULARQUEUEIMPL_TPP


// Project headers
# include "../../Assert/AssertEqual/AssertEqual.h"

template <typename T>
class FixedCircularQueueImpl
{
	public:
		class Iterator;
		typedef FixedCircularQueueImpl::Iterator iterator;


		iterator	begin() { return iterator(m_begin, m_capacity, m_backIndex, m_frontIndex); }
		iterator	end() { return iterator(m_begin, m_capacity, m_backIndex, -1); }

		// m_backIndex is one past last (iter::end()), so the new one will be exactly there
		// so construct first, adjust after
		bool push_back(const T& value)
		{
			if (!mf_PushPrepare())
				return (false);
			m_begin[m_backIndex] = value;
			m_backIndex = (m_backIndex + 1) % m_capacity;
			return (true);
		}

		// m_backIndex is one past last (iter::end()), adjust first, delete after
		bool pop_back()
		{
			if (isEmpty())
				return (false);
			
			m_backIndex = (m_backIndex == 0) ? m_capacity - 1 : m_backIndex - 1;
			mf_PopResetIndexes();
			return (true);
		}



		//m_frontIndex is already the first, so we adjust first, construct after
		bool push_front(const T& value)
		{
			if (!mf_PushPrepare())
				return (false);
			
			m_frontIndex = (m_frontIndex == 0) ? m_capacity - 1 : m_frontIndex - 1;
			m_begin[m_frontIndex] = value;

			return (true);
		}

		// m_frontIndex is the first, delete first, adjust after
		bool pop_front()
		{
			if (isEmpty())
				return (false);
			
			m_frontIndex = (m_frontIndex + 1) % m_capacity;
			mf_PopResetIndexes();
			return (true);
		}

		T& operator[](const size_t index)
		{
			ASSERT_EQUAL(!isEmpty() && index < size(), true, "StackCircularQueue access: index out of bounds");
			size_t position;

			position = (m_frontIndex + index) % m_capacity;
			return (m_begin[position]);
		}

		const T& operator[](const size_t index) const
		{
			ASSERT_EQUAL(!isEmpty() && index < size(), true, "StackCircularQueue access: index out of bounds");
			
			size_t position;

			position = (m_frontIndex + index) % m_capacity;
			return (m_begin[position]);
		}

		
        T& at(size_t index)
        {
			ASSERT_EQUAL(!isEmpty() && index < size(), true, "StackCircularQueue access: index out of bounds");

			size_t position;

			position = (m_frontIndex + index) % m_capacity;
			return (m_begin[position]);
        }

		T& front()
		{
			ASSERT_EQUAL(!isEmpty(), true, "StackCircularQueue access: index out of bounds");
			
			return (m_begin[m_frontIndex]);
		}

		T& back()
		{
			ASSERT_EQUAL(!isEmpty(), true, "StackCircularQueue access: index out of bounds");

			size_t position;

			position = (m_backIndex == 0) ? m_capacity - 1 : m_backIndex - 1;

			return (m_begin[position]);
		}


		bool		isEmpty() const { return (m_frontIndex == m_backIndex && m_frontIndex == -1);}
		bool		isFull() const { return (m_frontIndex == m_backIndex && m_frontIndex != -1);}




		size_t size() const
		{
			if (isFull())
				return (m_capacity);
			else if (m_frontIndex <= m_backIndex)
				return (m_backIndex - m_frontIndex);
			else 
				return (m_capacity - (m_frontIndex - m_backIndex));
		}

		size_t capacity() const
		{
			return (m_capacity);
		}

		void clear()
		{
			m_frontIndex = -1;
			m_backIndex = -1;
		}

		bool emplace_back()
        {
			if (!mf_PushPrepare())
				return (false);
			m_begin[m_backIndex].~T();
			new (&m_begin[m_backIndex]) T();
			m_backIndex = (m_backIndex + 1) % m_capacity;
			return (true);
		}

		template <typename Arg1 >
		bool emplace_back(Arg1& arg1)
		{
			if (!mf_PushPrepare())
				return (false);
			m_begin[m_backIndex].~T();
			new (&m_begin[m_backIndex]) T(arg1);
			m_backIndex = (m_backIndex + 1) % m_capacity;
			return (true);
        }

        template <typename Arg1, typename Arg2 >
        bool emplace_back(Arg1& arg1, Arg2& arg2)
        {
			if (!mf_PushPrepare())
				return (false);
			m_begin[m_backIndex].~T();
			new (&m_begin[m_backIndex]) T(arg1, arg2);
			m_backIndex = (m_backIndex + 1) % m_capacity;
			return (true);
        }

        template <typename Arg1, typename Arg2 , typename Arg3 >
        bool emplace_back(Arg1& arg1, Arg2& arg2, Arg3& arg3)
        {
			if (!mf_PushPrepare())
				return (false);
			m_begin[m_backIndex].~T();
			new (&m_begin[m_backIndex]) T(arg1, arg2, arg3);
			m_backIndex = (m_backIndex + 1) % m_capacity;
			return (true);
        }

		template <typename Arg1 >
		bool emplace_back(const Arg1& arg1)
		{
			if (!mf_PushPrepare())
				return (false);
			m_begin[m_backIndex].~T();
			new (&m_begin[m_backIndex]) T(arg1);
			m_backIndex = (m_backIndex + 1) % m_capacity;
			return (true);
        }

        template <typename Arg1, typename Arg2 >
        bool emplace_back(const Arg1& arg1, const Arg2& arg2)
        {
			if (!mf_PushPrepare())
				return (false);
			m_begin[m_backIndex].~T();
			new (&m_begin[m_backIndex]) T(arg1, arg2);
			m_backIndex = (m_backIndex + 1) % m_capacity;
			return (true);
        }

        template <typename Arg1, typename Arg2 , typename Arg3 >
        bool emplace_back(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3)
        {
			if (!mf_PushPrepare())
				return (false);
			m_begin[m_backIndex].~T();
			new (&m_begin[m_backIndex]) T(arg1, arg2, arg3);
			m_backIndex = (m_backIndex + 1) % m_capacity;
			return (true);
        }	

		bool emplace_front()
        {
			if (!mf_PushPrepare())
				return (false);
			m_frontIndex = (m_frontIndex == 0) ? m_capacity - 1 : m_frontIndex - 1;
			m_begin[m_frontIndex].~T();
			new (&m_begin[m_frontIndex]) T();
			return (true);
		}

		template <typename Arg1 >
		bool emplace_front(Arg1& arg1)
		{
			if (!mf_PushPrepare())
				return (false);

			m_frontIndex = (m_frontIndex == 0) ? m_capacity - 1 : m_frontIndex - 1;
			m_begin[m_frontIndex].~T();
			new (&m_begin[m_frontIndex]) T(arg1);
			return (true);
        }

        template <typename Arg1, typename Arg2 >
        bool emplace_front(Arg1& arg1, Arg2& arg2)
        {
			if (!mf_PushPrepare())
				return (false);

			m_frontIndex = (m_frontIndex == 0) ? m_capacity - 1 : m_frontIndex - 1;
			m_begin[m_frontIndex].~T();
			new (&m_begin[m_frontIndex]) T(arg1, arg2);
			return (true);
		}
		template <typename Arg1 >
		bool emplace_front(const Arg1& arg1)
		{
			if (!mf_PushPrepare())
				return (false);

			m_frontIndex = (m_frontIndex == 0) ? m_capacity - 1 : m_frontIndex - 1;
			m_begin[m_frontIndex].~T();
			new (&m_begin[m_frontIndex]) T(arg1);
			return (true);
        }

        template <typename Arg1, typename Arg2 >
        bool emplace_front(const Arg1& arg1, const Arg2& arg2)
        {
			if (!mf_PushPrepare())
				return (false);

			m_frontIndex = (m_frontIndex == 0) ? m_capacity - 1 : m_frontIndex - 1;
			m_begin[m_frontIndex].~T();
			new (&m_begin[m_frontIndex]) T(arg1, arg2);
			return (true);
        }

        template <typename Arg1, typename Arg2 , typename Arg3 >
        bool emplace_front(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3)
        {
			if (!mf_PushPrepare())
				return (false);

			m_frontIndex = (m_frontIndex == 0) ? m_capacity - 1 : m_frontIndex - 1;
			m_begin[m_frontIndex].~T();
			new (&m_begin[m_frontIndex]) T(arg1, arg2, arg3);
			return (true);
        }			

	protected:
		FixedCircularQueueImpl(T* begin, T* end, int frontIndex, int backIndex, const size_t capacity) : 
			m_begin(begin), 
			m_end(end),
			m_frontIndex(frontIndex),
			m_backIndex(backIndex),
			m_capacity(capacity)
		{
			for (size_t i = 0; i < m_capacity; i++)
				new (&m_begin[i]) T();
		}

		FixedCircularQueueImpl(T* begin, T* end, int frontIndex, int backIndex, const size_t capacity, const T& value) : 
			m_begin(begin), 
			m_end(end),
			m_frontIndex(frontIndex),
			m_backIndex(backIndex),
			m_capacity(capacity)
		{
			for (size_t i = 0; i < m_capacity; i++)
				new (&m_begin[i]) T(value);
		}

		FixedCircularQueueImpl(const FixedCircularQueueImpl& other) : 
			m_begin(other.m_begin), 
			m_end(other.m_end), 
			m_frontIndex(other.m_frontIndex),
			m_backIndex(other.m_backIndex),
			m_capacity(other.m_capacity)
		{
			for (size_t i = 0; i < m_capacity; i++)
				new (&m_begin[i]) T(other.m_begin[i]);
		}

		FixedCircularQueueImpl& operator=(const FixedCircularQueueImpl& other)
		{
			if (this == &other)
				return (*this);

			int srcIndex;
			int destIndex;
			size_t otherSize = other.size();

			m_frontIndex = other.m_frontIndex;
			for (size_t i = 0; i < otherSize; ++i)
			{
				srcIndex = (other.m_frontIndex + i) % m_capacity;
				destIndex = (m_frontIndex + i) % m_capacity;
				m_begin[destIndex] = other.m_begin[srcIndex];
			}
			
			m_frontIndex = other.m_frontIndex;
			m_backIndex = other.m_backIndex;

			return (*this);
		}

		~FixedCircularQueueImpl()
		{
			mf_destroyAll();
		}

        void reset(T* begin, T* end, int frontIndex, int backIndex, const size_t capacity)
        {
            m_begin = begin;
            m_end = end;
            m_frontIndex = frontIndex;
            m_backIndex = backIndex;
            m_capacity = capacity;
        }

        void fill()
        {
            for (size_t i = 0; i < m_capacity; i++)
                new(&m_begin[i]) T();
        }

        void fill(const T& value)
        {
            for (size_t i = 0; i < m_capacity; i++)
                new(&m_begin[i]) T(value);
        }

		void	mf_destroyAll()
		{
			for (size_t i = 0; i < m_capacity; ++i)
			{
				m_begin[i].~T();
			}
		}

	public:
		class Iterator
		{
			public:
				typedef T                                   value_type;
				typedef T*                                  pointer;
				typedef T&                                  reference;
				typedef std::ptrdiff_t                      difference_type;
				typedef std::random_access_iterator_tag     iterator_category;

				Iterator(pointer array, size_t capacity, size_t tail, size_t index)
					: m_typeArray(array), m_capacity(capacity), m_backIndex(tail), m_index(index) {}

				Iterator(const Iterator& other)
					: m_typeArray(other.m_typeArray), m_capacity(other.m_capacity), m_backIndex(other.m_backIndex), m_index(other.m_index) {}

				~Iterator() {}

				Iterator& operator=(const Iterator& other)
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

				Iterator& operator++()
				{
					ASSERT_EQUAL(m_index != -1, true, "StackCircularQueue access: index out of bounds");
					m_index = (m_index + 1) % m_capacity;

					//reached the back
					m_index = (m_index == m_backIndex) ? -1 : m_index;
					return (*this);
				}

				Iterator operator++(int)
				{
					Iterator tmp = *this;
					++(*this);
					return (tmp);
				}

				Iterator& operator--()
				{
					m_index = (m_index == -1) ? m_backIndex - 1 : m_index; // Wrap-around
					m_index = (m_index == 0) ? m_capacity - 1 : m_index - 1; // Wrap-around
					return *(this);
				}

				Iterator operator--(int)
				{
					iterator tmp = *this;
					--(*this);
					return (tmp);
				}

				bool operator==(const Iterator& other) const { return (m_index == other.m_index); }
				bool operator!=(const Iterator& other) const { return (!(*this == other)); }

			private:
				pointer			m_typeArray;       
				int				m_capacity;     
				int				m_backIndex;
				int				m_index;
		};



	private:
		T*              m_begin;
		T*              m_end;
		int				m_frontIndex;
		int				m_backIndex;
		size_t          m_capacity;	



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
