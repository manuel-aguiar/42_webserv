

#ifndef HEAPCIRCULARQUEUE_TPP

# define HEAPCIRCULARQUEUE_TPP

// Project headers
# include "../../Assert/AssertEqual/AssertEqual.h"

# include "../_FixedCircularQueueImpl/FixedCircularQueueImpl.hpp"

template <typename T, typename Allocator>
class HeapCircularQueue : public FixedCircularQueueImpl<T>
{
	public:
		
		HeapCircularQueue(size_t capacity = 0, const Allocator& allocator = Allocator()) : 
			FixedCircularQueueImpl<T>(NULL, NULL, -1, -1, 0),
			m_allocator(allocator), 
			m_array(0)
		{
			initialize(capacity);
		}

		HeapCircularQueue(size_t capacity, const T& value, const Allocator& allocator = Allocator()) : 
			FixedCircularQueueImpl<T>(NULL, NULL, -1, -1, 0, value),
			m_allocator(allocator), 
			m_array(0)
		{
			initialize(capacity, value);
		}

		HeapCircularQueue(const HeapCircularQueue &other) : 
			FixedCircularQueueImpl<T>(NULL, NULL, -1, -1, 0),
			m_array(0)
		{
			*this = other;
		};

		HeapCircularQueue &operator=(const HeapCircularQueue &other)
		{
			if (this == &other)
				return *this;

			
			if (m_array && (this->capacity() != other.capacity() || m_allocator != other.m_allocator))
			{
				FixedCircularQueueImpl<T>::reset(NULL, NULL, -1, -1, 0);
				m_allocator.deallocate(m_array, this->capacity());
			}
			else if (!m_array)
			{
				m_array = m_allocator.allocate(other.capacity());
				FixedCircularQueueImpl<T>::reset(m_array, m_array, -1, -1, other.capacity());
			}
				

			FixedCircularQueueImpl<T>::operator=(other);

			m_allocator = other.m_allocator;

			return *this;
		};

		~HeapCircularQueue()
		{
			//std::cout << "destroy, begin equal end? " << " begin: " << &(*this->begin()) << " end:" << &(*this->end()) << std::endl;
			size_t capacity = this->capacity();
			FixedCircularQueueImpl<T>::mf_destroyAll();
			if (m_array)
				m_allocator.deallocate(m_array, capacity);
			FixedCircularQueueImpl<T>::reset(NULL, NULL, -1, -1, 0);
		}

		void	swap(HeapCircularQueue& from)
		{
			size_t capacity = this->capacity();
			FixedCircularQueueImpl<T>::reset(NULL, NULL, -1, -1, 0);
			if (m_array)
				m_allocator.deallocate(m_array, capacity);

			m_array = from.m_array;
			FixedCircularQueueImpl<T>::reset(from.m_array, from.m_array + from.size(), from.m_frontIndex, from.m_backIndex, from.capacity());
			from.m_array = NULL;
			from.reset(NULL, NULL, -1, -1, 0);
		}

		T* getArray() { return m_array; }
		const T* getArray() const { return m_array; }

	private:
		Allocator 	m_allocator;
		T* 			m_array;

		void initialize(size_t capacity)
		{
			m_array = m_allocator.allocate(capacity);
			FixedCircularQueueImpl<T>::reset(m_array, m_array, -1, -1, capacity);
			FixedCircularQueueImpl<T>::fill();
		}

		void initialize(size_t capacity, const T& value)
		{
			m_array = m_allocator.allocate(capacity);
			FixedCircularQueueImpl<T>::reset(m_array, m_array, -1, -1, capacity);
			FixedCircularQueueImpl<T>::fill(value);
		}
};




#endif