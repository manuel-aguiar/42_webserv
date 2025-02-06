

#ifndef STACKARRAY_TPP

# define STACKARRAY_TPP

// Project headers
# include "../../Assert/AssertEqual/AssertEqual.h"

// Base class
# include "../ArrayFixedSizeImpl/ArrayFixedSizeImpl.hpp"

template <typename T, size_t ElemCount>
class StackArray : public ArrayFixedSizeImpl<T>
{
	public:
		StackArray() : 
            ArrayFixedSizeImpl<T>(reinterpret_cast<T*>(m_array), reinterpret_cast<T*>(m_array), ElemCount) {}


        StackArray(const T& value) : 
            ArrayFixedSizeImpl<T>(reinterpret_cast<T*>(m_array), reinterpret_cast<T*>(m_array) + ElemCount, ElemCount, value) {}

		~StackArray() {};

		StackArray(const StackArray &other) : 
            ArrayFixedSizeImpl<T>(other)
		{
			*this = other;
		};

		StackArray &operator=(const StackArray &other)
		{
            if (this == &other)
                return (*this);

            ArrayFixedSizeImpl<T>::operator=(other);

            return (*this);
		};


        T* getArray() {return (reinterpret_cast<T*>(m_array));}


	private:
		typedef unsigned char 		t_byte;
		t_byte  					m_array[sizeof(T) * ElemCount];
};




#endif