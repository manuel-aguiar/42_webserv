



#ifndef MPOOL_FIXEDELEMENTS_TCC
#define MPOOL_FIXEDELEMENTS_TCC

#include <limits>
#include <stdint.h>
#include <stddef.h>
#include <cstring>
#include <cassert>

#include <vector>
#include "../../Arrays/Arrays.h"

/*
	Template to get the element size aligned at compile time for correct vector allocation
*/
template <size_t Size, size_t Alignment>
struct AlignedSize
{
    static const size_t value = (Size + Alignment - 1) & ~(Alignment - 1);
};


template <typename T>
class MPool_FixedElem
{
	public:
	
		typedef T               value_type;
		typedef T*              pointer;
		typedef T&              reference;
		typedef const T*        const_pointer;
		typedef const T&        const_reference;
		typedef size_t          size_type;
		typedef ptrdiff_t       difference_type;

		template <typename U> struct rebind {
			typedef MPool_FixedElem<U> other;
		};

		MPool_FixedElem(size_t numElems = 0) throw();

		MPool_FixedElem(const MPool_FixedElem& memoryPool) throw();
		template <class U> MPool_FixedElem(const MPool_FixedElem<U>& memoryPool) throw();

		~MPool_FixedElem() throw();

		pointer address(reference x) const throw();
		const_pointer address(const_reference x) const throw();

		pointer allocate(size_type n = 1, const_pointer hint = 0);
		void deallocate(pointer p, size_type n = 1);

		size_type max_size() const throw();

		void construct(pointer p, const_reference val);
		void destroy(pointer p);

		pointer newElement(const_reference val);
		void deleteElement(pointer p);

		union Slot_
		{
			char		  data_[AlignedSize<sizeof(value_type), __alignof__(value_type)>::value];                      
			Slot_*        next;
		};

		typedef char* data_pointer_;
		typedef Slot_ slot_type_;
		typedef Slot_* slot_pointer_;

		HeapArray<Slot_> 	_elements;
		size_t 				_elemCount;
		size_t 				_maxElems;
		slot_pointer_ 		_freeSlot;

};


template <typename T>
MPool_FixedElem<T>::MPool_FixedElem(size_t numElems) throw() :
	_elements(numElems),
	_elemCount(0),
	_maxElems(numElems),
	_freeSlot(NULL)
{
}


template <typename T>
MPool_FixedElem<T>::MPool_FixedElem(const MPool_FixedElem& memoryPool) throw() :
	_elemCount(memoryPool._elemCount),
	_maxElems(memoryPool._maxElems),
	_freeSlot(memoryPool._freeSlot)
{
	_elements = HeapArray<Slot_>(memoryPool._maxElems);
}



template <typename T>
template<class U>
MPool_FixedElem<T>::MPool_FixedElem(const MPool_FixedElem<U>& memoryPool) throw() :
	_elemCount(0),
	_maxElems(memoryPool._maxElems),
	_freeSlot(NULL)
{
	
}



template <typename T>
MPool_FixedElem<T>::~MPool_FixedElem()
throw()
{

}


template <typename T>
inline typename MPool_FixedElem<T>::pointer
MPool_FixedElem<T>::address(reference x)
const throw()
{
	return &x;
}



template <typename T>
inline typename MPool_FixedElem<T>::const_pointer
MPool_FixedElem<T>::address(const_reference x)
const throw()
{
	return &x;
}



template <typename T>
inline typename MPool_FixedElem<T>::pointer
MPool_FixedElem<T>::allocate(size_type, const_pointer)
{
	assert(_elemCount < _maxElems);
	
	if (_freeSlot != 0)
	{
		pointer result = reinterpret_cast<pointer>(_freeSlot);
		_freeSlot = _freeSlot->next;
		_elemCount++;
		return (result);
	}
	else
	{
		return reinterpret_cast<pointer>(&_elements[_elemCount++]);
	}
		
}



template <typename T>
inline void
MPool_FixedElem<T>::deallocate(pointer p, size_type)
{
	if (p != 0)
	{
		reinterpret_cast<slot_pointer_>(p)->next = _freeSlot;
		_freeSlot = reinterpret_cast<slot_pointer_>(p);
		--_elemCount;
	}
}

template <typename T>
inline typename MPool_FixedElem<T>::size_type
MPool_FixedElem<T>::max_size()
const throw()
{
	return (_maxElems);
}



template <typename T>
inline void
MPool_FixedElem<T>::construct(pointer p, const_reference val)
{
	new (p) value_type (val);
}



template <typename T>
inline void
MPool_FixedElem<T>::destroy(pointer p)
{
	p->~value_type();
}



template <typename T>
inline typename MPool_FixedElem<T>::pointer
MPool_FixedElem<T>::newElement(const_reference val)
{
	pointer result = allocate();
	construct(result, val);
	return result;
}



template <typename T>
inline void
MPool_FixedElem<T>::deleteElement(pointer p)
{
	if (p != 0)
	{
		p->~value_type();
		deallocate(p);
	}
}

#endif // MEMORY_BLOCK_TCC