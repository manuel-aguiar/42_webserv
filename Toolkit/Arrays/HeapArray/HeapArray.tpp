

#ifndef HEAPARRAY_TPP

# define HEAPARRAY_TPP

// Project headers
# include "../../Assert/AssertEqual/AssertEqual.h"

// Base class
# include "../_ArrayFixedSizeImpl/ArrayFixedSizeImpl.hpp"

template <typename T, typename Allocator>
class HeapArray : public ArrayFixedSizeImpl<T>
{
	public:
		
		HeapArray(size_t capacity = 0, const Allocator& allocator = Allocator()) : 
			ArrayFixedSizeImpl<T>(NULL, NULL, 0),
			m_allocator(allocator), 
			m_array(0)
		{
			initialize(capacity);
		}

		HeapArray(size_t capacity, const T& value, const Allocator& allocator = Allocator()) : 
			ArrayFixedSizeImpl<T>(0, 0, 0),
			m_allocator(allocator), 
			m_array(0)
		{
			initialize(capacity, value);
		}

		HeapArray(const HeapArray &other) : 
			ArrayFixedSizeImpl<T>(0, 0, 0),
			m_allocator(other.m_allocator), 
			m_array(0)
		{
			*this = other;
		};

		HeapArray &operator=(const HeapArray &other)
		{
			if (this == &other)
				return *this;

			
			if (m_array && (this->capacity() != other.capacity() || m_allocator != other.m_allocator))
			{
				ArrayFixedSizeImpl<T>::reset(NULL, NULL, 0);
				m_allocator.deallocate(m_array, this->capacity());
			}
			else if (!m_array)
			{
				m_array = m_allocator.allocate(other.capacity());
				ArrayFixedSizeImpl<T>::reset(m_array, m_array, other.capacity());
			}
				

			ArrayFixedSizeImpl<T>::operator=(other);

			return *this;
		};

		~HeapArray()
		{
			//std::cout << "destroy, begin equal end? " << " begin: " << &(*this->begin()) << " end:" << &(*this->end()) << std::endl;
			size_t capacity = this->capacity();
			ArrayFixedSizeImpl<T>::clear();
			if (m_array)
				m_allocator.deallocate(m_array, capacity);
		}

		void	swap(HeapArray& from)
		{
			size_t capacity = this->capacity();
			ArrayFixedSizeImpl<T>::reset(NULL, NULL, 0);
			if (m_array)
				m_allocator.deallocate(m_array, capacity);

			m_array = from.m_array;
			ArrayFixedSizeImpl<T>::reset(from.m_array, from.m_array + from.size(), from.capacity());
			from.m_array = NULL;
			from.reset(NULL, NULL, 0);
		}

		T* getArray() { return m_array; }
		const T* getArray() const { return m_array; }

	private:
		Allocator 	m_allocator;
		T* 			m_array;

		void initialize(size_t capacity)
		{
			m_array = m_allocator.allocate(capacity);
			ArrayFixedSizeImpl<T>::reset(m_array, m_array, capacity);
		}

		void initialize(size_t capacity, const T& value)
		{
			m_array = m_allocator.allocate(capacity);
			ArrayFixedSizeImpl<T>::reset(m_array, m_array, capacity);
			for (size_t i = 0; i < capacity; ++i)
				ArrayFixedSizeImpl<T>::push_back(value);
		}
};

#endif
