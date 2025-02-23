


#ifndef Stack_ObjectPool_TPP
#define Stack_ObjectPool_TPP

// Project headers
# include "../../Assert/AssertEqual/AssertEqual.h"
#include "../../Arrays/StackArray/StackArray.hpp"

// C++ headers
#include <limits>
#include <cstddef>

/*
	Template to get the element size aligned at compile time for correct vector allocation
*/

//Base class
#include "../_ObjectPoolImpl/ObjectPoolImpl.hpp"

template <typename T, size_t Size>
class Stack_ObjectPool : public ObjectPoolImpl<T>
{
	public:
		Stack_ObjectPool() : 
			ObjectPoolImpl<T>(NULL, Size) { this->assignArray(m_storage.getArray());}

		Stack_ObjectPool(const Stack_ObjectPool &other) :
			ObjectPoolImpl<T>(other)
		{
			*this = other;
		}

		~Stack_ObjectPool() {}

		Stack_ObjectPool &operator=(const Stack_ObjectPool &other)
		{
			if (this == &other)
				return (*this);
			
			ObjectPoolImpl<T>::operator=(other);

			return (*this);
		}
	
	private:
		StackArray<typename ObjectPoolImpl<T>::t_slot_type, Size> m_storage;
};



#endif
