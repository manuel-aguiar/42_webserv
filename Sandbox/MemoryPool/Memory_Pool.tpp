/*-
 * Copyright (c) 2013 Cosku Acay, http://www.coskuacay.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

/*

  This is the same code as defined in the license above

  I took it and adapted it to create a SharedMemoryPool class that uses a reference count to manage the memory pool
  and allow sharing between different containers. (see main)

  This pool only "works" for objects without random access iterators:
    elemets will be scattered everywhere......, so this works with std::list std::map and std::set

    Anyways, vectors are arrays already, so they don't need this kind of memory pool

*/



#ifndef MEMORY_BLOCK_TCC
#define MEMORY_BLOCK_TCC

#include <limits.h>
#include <stddef.h>
#include <cstring>


template <typename T, size_t BlockSize>
class MemoryPool
{
  public:
    /* Member types */
    typedef T               value_type;
    typedef T*              pointer;
    typedef T&              reference;
    typedef const T*        const_pointer;
    typedef const T&        const_reference;
    typedef size_t          size_type;
    typedef ptrdiff_t       difference_type;

    template <typename U> struct rebind {
      typedef MemoryPool<U> other;
    };

    /* Member functions */
    MemoryPool() throw();
    MemoryPool(const MemoryPool& memoryPool) throw();
    template <class U> MemoryPool(const MemoryPool<U, BlockSize>& memoryPool) throw();

    ~MemoryPool() throw();

    pointer address(reference x) const throw();
    const_pointer address(const_reference x) const throw();

    // Can only allocate one object at a time. n and hint are ignored
    pointer allocate(size_type n = 1, const_pointer hint = 0);
    void deallocate(pointer p, size_type n = 1);

    size_type max_size() const throw();

    void construct(pointer p, const_reference val);
    void destroy(pointer p);

    pointer newElement(const_reference val);
    void deleteElement(pointer p);

  //private:
    union Slot_ {
      value_type*   element;
      char          data_[sizeof(value_type)];
      Slot_*        next;
    };

    typedef char* data_pointer_;
    typedef Slot_ slot_type_;
    typedef Slot_* slot_pointer_;

    slot_pointer_ currentBlock_;
    slot_pointer_ currentSlot_;
    slot_pointer_ lastSlot_;
    slot_pointer_ freeSlots_;

    size_type padPointer(data_pointer_ p, size_type align) const throw();
    void allocateBlock();
   /*
    static_assert(BlockSize >= 2 * sizeof(slot_type_), "BlockSize too small.");
    */

    //MemoryPool(const MemoryPool<T, BlockSize>& memoryPool) {(void)memoryPool;}
    //MemoryPool& operator=(const MemoryPool<T, BlockSize>& memoryPool) {(void)memoryPool; return (*this);};

};

template <typename T, size_t BlockSize>
inline typename MemoryPool<T, BlockSize>::size_type
MemoryPool<T, BlockSize>::padPointer(data_pointer_ p, size_type align)
const throw()
{
  size_t result = reinterpret_cast<size_t>(p);
  return ((align - result) % align);
}



template <typename T, size_t BlockSize>
MemoryPool<T, BlockSize>::MemoryPool()
throw()
{
  std::cout << "memory pool constructed" << std::endl;
  currentBlock_ = 0;
  currentSlot_ = 0;
  lastSlot_ = 0;
  freeSlots_ = 0;
}



template <typename T, size_t BlockSize>
MemoryPool<T, BlockSize>::MemoryPool(const MemoryPool& memoryPool)
throw()
{
    (void)memoryPool;
    MemoryPool();

}



template <typename T, size_t BlockSize>
template<class U>
MemoryPool<T, BlockSize>::MemoryPool(const MemoryPool<U, BlockSize>& memoryPool)
throw()
{
    (void)memoryPool;   
    MemoryPool();
}



template <typename T, size_t BlockSize>
MemoryPool<T, BlockSize>::~MemoryPool()
throw()
{
  slot_pointer_ curr = currentBlock_;
  while (curr != 0) {
    slot_pointer_ prev = curr->next;
    operator delete(reinterpret_cast<void*>(curr));
    curr = prev;
  }
}



template <typename T, size_t BlockSize>
inline typename MemoryPool<T, BlockSize>::pointer
MemoryPool<T, BlockSize>::address(reference x)
const throw()
{
  return &x;
}



template <typename T, size_t BlockSize>
inline typename MemoryPool<T, BlockSize>::const_pointer
MemoryPool<T, BlockSize>::address(const_reference x)
const throw()
{
  return &x;
}



template <typename T, size_t BlockSize>
void
MemoryPool<T, BlockSize>::allocateBlock()
{
  // Allocate space for the new block and store a pointer to the previous one
  data_pointer_ newBlock = reinterpret_cast<data_pointer_>
                           (operator new(BlockSize));
    //std::memset(newBlock, 0, BlockSize);
  reinterpret_cast<slot_pointer_>(newBlock)->next = currentBlock_;
  currentBlock_ = reinterpret_cast<slot_pointer_>(newBlock);
  // Pad block body to staisfy the alignment requirements for elements
  data_pointer_ body = newBlock + sizeof(slot_pointer_);
  size_type bodyPadding = padPointer(body, sizeof(slot_type_));
  currentSlot_ = reinterpret_cast<slot_pointer_>(body + bodyPadding);
  lastSlot_ = reinterpret_cast<slot_pointer_>
              (newBlock + BlockSize - sizeof(slot_type_) + 1);
}



template <typename T, size_t BlockSize>
inline typename MemoryPool<T, BlockSize>::pointer
MemoryPool<T, BlockSize>::allocate(size_type, const_pointer)
{
  //std::cout << "pool allocating: " << sizeof(T) << std::endl; 
  if (freeSlots_ != 0) {
    pointer result = reinterpret_cast<pointer>(freeSlots_);
    freeSlots_ = freeSlots_->next;
    return result;
  }
  else {
    if (currentSlot_ >= lastSlot_)
      allocateBlock();
    return reinterpret_cast<pointer>(currentSlot_++);
  }
}



template <typename T, size_t BlockSize>
inline void
MemoryPool<T, BlockSize>::deallocate(pointer p, size_type)
{
  if (p != 0) {
    reinterpret_cast<slot_pointer_>(p)->next = freeSlots_;
    freeSlots_ = reinterpret_cast<slot_pointer_>(p);
  }
}



template <typename T, size_t BlockSize>
inline typename MemoryPool<T, BlockSize>::size_type
MemoryPool<T, BlockSize>::max_size()
const throw()
{
  size_type maxBlocks = -1 / BlockSize;
  return (BlockSize - sizeof(data_pointer_)) / sizeof(slot_type_) * maxBlocks;
}



template <typename T, size_t BlockSize>
inline void
MemoryPool<T, BlockSize>::construct(pointer p, const_reference val)
{
    //std::cout << "pool CONSTRUCTING: " << sizeof(T) << std::endl; 
  new (p) value_type (val);
  //std::cout << "MemoryPool::construct" << std::endl;    
}



template <typename T, size_t BlockSize>
inline void
MemoryPool<T, BlockSize>::destroy(pointer p)
{
  p->~value_type();
  //std::cout << "MemoryPool::destroy" << std::endl;
}



template <typename T, size_t BlockSize>
inline typename MemoryPool<T, BlockSize>::pointer
MemoryPool<T, BlockSize>::newElement(const_reference val)
{
  pointer result = allocate();
  construct(result, val);
  return result;
}



template <typename T, size_t BlockSize>
inline void
MemoryPool<T, BlockSize>::deleteElement(pointer p)
{
  if (p != 0) {
    p->~value_type();
    deallocate(p);
  }
}

template <typename T, size_t BlockSize = 4096>
class SharedMemoryPool;

template <typename T, size_t BlockSize >
class SharedMemoryPool
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
          typedef SharedMemoryPool<U> other;
        };

        SharedMemoryPool(MemoryPool<T, BlockSize>* ptr = NULL) : _pool(ptr), _refCount(new int(1))
        {

        }

        SharedMemoryPool(const SharedMemoryPool& other) : _pool(other._pool), _refCount(other._refCount)
         {
           ++(*_refCount); 
        }
        
        SharedMemoryPool& operator=(const SharedMemoryPool& other)
        {
            if (this != &other)
            {
                _decrementRefCount();
                _pool = other._pool;
                _refCount = other._refCount;
                ++(*_refCount);
            }
            return (*this);
        }
        template <class U> SharedMemoryPool(const SharedMemoryPool<U, BlockSize>& memoryPool) throw()
        {
          //std::cout << "size of T: " << sizeof(T) << " , sizeof U" << sizeof(U) << std::endl;
          // Use the rebind mechanism to create a new pool for type T
          _pool = reinterpret_cast<MemoryPool<T, BlockSize>*>(memoryPool._pool);

          // Share the reference count, ensuring both share the same pool
          _refCount = memoryPool._refCount;
          
          // Increment reference count since we're sharing the same pool
          ++(*_refCount);
          //std::cout << "rebind, refcount: " << (*_refCount) << std::endl;
        }
        ~SharedMemoryPool()
        {
            _decrementRefCount();
        }

        pointer address(reference x) const throw() {return _pool->address(x);}
        const_pointer address(const_reference x) const throw() {return _pool->address(x);}

        // Can only allocate one object at a time. n and hint are ignored
        pointer allocate(size_type n = 1, const_pointer hint = 0){return _pool->allocate(n, hint);}
        void deallocate(pointer p, size_type n = 1) {return _pool->deallocate(p, n);}

        size_type max_size() const throw() {return _pool->max_size();}

        void construct(pointer p, const_reference val) {return _pool->construct(p, val);}
        void destroy(pointer p) {return _pool->destroy(p);}

        pointer newElement(const_reference val) {return _pool->newElement(val);}
        void deleteElement(pointer p) {return _pool->deleteElement(p);}

    //private:
        MemoryPool<T, BlockSize>*      _pool;
        int*                           _refCount;

        void _decrementRefCount()
        {
            if (--(*_refCount) == 0)
            {
                if (_pool)
                {
                    delete (_pool);
                    _pool = NULL;
                }
                if (_refCount)
                {
                    delete _refCount;
                    _refCount = NULL;
                }   
            }
        }
};

template <typename T, size_t BlockSize>
SharedMemoryPool<T, BlockSize> make_SharedPool()
{
    return (SharedMemoryPool<T, BlockSize>(new MemoryPool<T, BlockSize>()));
}

template <typename T, size_t BlockSize, typename Arg1>
SharedMemoryPool<T, BlockSize> make_SharedPool(Arg1 arg1)
{
    return SharedMemoryPool<T, BlockSize>(new MemoryPool<T, BlockSize>(arg1));
}


#endif // MEMORY_BLOCK_TCC