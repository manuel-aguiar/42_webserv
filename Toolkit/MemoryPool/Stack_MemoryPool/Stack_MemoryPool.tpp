

#ifndef Stack_MemoryPool_TPP

# define Stack_MemoryPool_TPP

// Project headers
# include "../../Assert/AssertEqual/AssertEqual.h"

// C++ headers
# include <cstddef>

// Base class
# include "../FixedMemoryPoolImpl/FixedMemoryPoolImpl.hpp"

template <size_t BlockSize, typename T>
class Stack_MemoryPoolGeneric : public FixedMemoryPoolImpl<T>
{
    public:
        Stack_MemoryPoolGeneric() : 
            FixedMemoryPoolImpl<T>(m_array, BlockSize) {}

        ~Stack_MemoryPoolGeneric() {}


    private:
        T                           m_array[BlockSize];

        Stack_MemoryPoolGeneric(const Stack_MemoryPoolGeneric& pool) {(void)pool;}
        Stack_MemoryPoolGeneric& operator=(const Stack_MemoryPoolGeneric& pool){(void)pool; return (*this);}

};


#endif
