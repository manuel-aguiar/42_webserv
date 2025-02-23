

#ifndef STACKCIRCULARQUEUE_TPP

# define STACKCIRCULARQUEUE_TPP

// Project headers
# include "../../Assert/AssertEqual/AssertEqual.h"

# include "../_FixedCircularQueueImpl/FixedCircularQueueImpl.hpp"

template <typename T, size_t queueCapacity>
class StackCircularQueue : public FixedCircularQueueImpl<T>
{
	public:
		StackCircularQueue() : 
			FixedCircularQueueImpl<T>(reinterpret_cast<T*>(m_byteArray), reinterpret_cast<T*>(m_byteArray) + queueCapacity, -1, -1, queueCapacity) {}

		// user can avoid default construction by initializing copies, buffer will stiil have "zero" elements inserted
		StackCircularQueue(const T& copy) : 
			FixedCircularQueueImpl<T>(reinterpret_cast<T*>(m_byteArray), reinterpret_cast<T*>(m_byteArray) + queueCapacity, -1, -1, queueCapacity, copy) {}

		StackCircularQueue(const StackCircularQueue &other) :
			FixedCircularQueueImpl<T>(other)
		{
			*this = other;
		}

		~StackCircularQueue() {}

		StackCircularQueue &operator=(const StackCircularQueue &other)
		{
			if (this == &other)
				return (*this);
			
			FixedCircularQueueImpl<T>::operator=(other);

			return (*this);
		}

	private:
		// private member variables
		typedef unsigned char		t_byte;	
		t_byte						m_byteArray[queueCapacity * sizeof(T)];

};

#endif
