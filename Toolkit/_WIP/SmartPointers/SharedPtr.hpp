
#ifndef SHAREDPTR_HPP

# define SHAREDPTR_HPP

# include <cstdlib>
# include <stdexcept>

#include "UniquePtr.hpp"
#include "DefaultDeleters.hpp"


template <typename T, typename Allocator = std::allocator<T> >
class SharedPtr
{
    public:
        SharedPtr(T* ptr = NULL, Allocator alloc = Allocator()) : _ptr(ptr), _refCount(new int(1)), _allocator(alloc) {}
        SharedPtr(const SharedPtr& other) : _ptr(other._ptr), _refCount(other._refCount), _allocator(other._allocator) { ++(*_refCount); }

        SharedPtr& operator=(const SharedPtr& other)
        {
            if (this != &other)
            {
                _decrementRefCount();
                _ptr = other._ptr;
                _refCount = other._refCount;
                ++(*_refCount);
            }
            return (*this);
        }

        SharedPtr& operator=(UniquePtr<T>& unique)
        {
            _decrementRefCount();
            _ptr = unique.release();
            _refCount = new int(1);
            return (*this);
        }

        ~SharedPtr()
        {
            _decrementRefCount();
        }

        void    transfer(SharedPtr& other)
        {
            if (this != &other)
            {
                _decrementRefCount();
                _ptr = other._ptr;
                _refCount = other._refCount;
                ++(*_refCount);
            }
        }

        void    transfer(UniquePtr<T>& other)
        {
            if (this != &other)
            {
                _decrementRefCount();
                _ptr = other.release();
                _refCount = new int(1);
                ++(*_refCount);
            }
        }

        T* release()
        {
            T* temp = _ptr;
            if (temp)
                _ptr = NULL;
            return temp;
        }


        // Reset the managed resource to a new one
        void reset(T* newPtr = NULL, Allocator alloc = Allocator())
        {
            if (_ptr != newPtr)
            {
                _decrementRefCount();
                _ptr = newPtr;
                _allocator = alloc;
                _refCount = new int(1);
            }
        }

        // Accessors
        T*          get() const { return (_ptr); }

        T&          operator*()
        {
            assert(_ptr != NULL);
            return (*_ptr);
        }

        const T&    operator*() const
        {
            assert(_ptr != NULL);
            return (*_ptr);
        }

        T*          operator->() const
        {
            assert(_ptr != NULL);
            return (_ptr);
        }

    private:
        T*     			_ptr;
        int*    		_refCount;
        Allocator     	_allocator;

        void _decrementRefCount()
        {
            if (--(*_refCount) == 0)
            {
                if (_ptr)
                {
                    _allocator.destroy(_ptr);
					_allocator.deallocate(_ptr, 1);
                    _ptr = NULL;
                }
                if (_refCount)
                {
                    delete _refCount;
                    _refCount = NULL;
                }
            }
        }
};

// Function to create a SharedPtr with no arguments
template <typename T, typename Allocator>
SharedPtr<T> make_SharedPtr(Allocator alloc = std::allocator<T>()) {
    T* ptr = alloc.allocate(1);
    new (ptr) T(); // Default constructor
    return SharedPtr<T>(ptr);
}

// Function to create a SharedPtr with one argument
template <typename T, typename Arg1, typename Allocator>
SharedPtr<T> make_SharedPtr(Arg1 arg1, Allocator alloc = std::allocator<T>()) {
    T* ptr = alloc.allocate(1);
    new (ptr) T(arg1); // Constructor with one argument
    return SharedPtr<T>(ptr);
}

// Function to create a SharedPtr with two arguments
template <typename T, typename Arg1, typename Arg2, typename Allocator>
SharedPtr<T> make_SharedPtr(Arg1 arg1, Arg2 arg2, Allocator alloc = std::allocator<T>()) {
    T* ptr = alloc.allocate(1);
    new (ptr) T(arg1, arg2); // Constructor with two arguments
    return SharedPtr<T>(ptr);
}

// Function to create a SharedPtr with three arguments
template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Allocator>
SharedPtr<T> make_SharedPtr(Arg1 arg1, Arg2 arg2, Arg3 arg3, Allocator alloc = std::allocator<T>()) {
    T* ptr = alloc.allocate(1);
    new (ptr) T(arg1, arg2, arg3); // Constructor with three arguments
    return SharedPtr<T>(ptr);
}

// Function to create a SharedPtr with four arguments
template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Allocator>
SharedPtr<T> make_SharedPtr(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Allocator alloc = std::allocator<T>()) {
    T* ptr = alloc.allocate(1);
    new (ptr) T(arg1, arg2, arg3, arg4); // Constructor with four arguments
    return SharedPtr<T>(ptr);
}

// Function to create a SharedPtr with five arguments
template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Allocator>
SharedPtr<T> make_SharedPtr(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Allocator alloc = std::allocator<T>()) {
    T* ptr = alloc.allocate(1);
    new (ptr) T(arg1, arg2, arg3, arg4, arg5); // Constructor with five arguments
    return SharedPtr<T>(ptr);
}

// Template specialization for arrays
template <typename T, typename Del>
class SharedPtr<T[], Del>
{
    public:
        SharedPtr(T* ptr = NULL, Del deleter = Del()) : _ptr(ptr), _refCount(new int(1)), _allocator(deleter) {}
        SharedPtr (UniquePtr<T[]>& unique) : _ptr(unique.release()), _refCount(new int(1)), _allocator(unique.getDeleter()) {}
        SharedPtr(const SharedPtr& other) : _ptr(other._ptr), _refCount(other._refCount), _allocator(other._allocator) { ++(*_refCount); }

        SharedPtr& operator=(const SharedPtr& other)
        {
            if (this != &other)
            {
                _decrementRefCount();
                _ptr = other._ptr;
                _refCount = other._refCount;
                ++(*_refCount);
            }
            return (*this);
        }

        SharedPtr& operator=(UniquePtr<T[]>& unique)
        {
            _decrementRefCount();
            _ptr = unique.release();
            _refCount = new int(1);
            return (*this);
        }

        ~SharedPtr()
        {
            _decrementRefCount();
        }

        void    transfer(SharedPtr& other)
        {
            if (this != &other)
                *this = other;
        }

        void    transfer(UniquePtr<T>& other)
        {
            if (this != &other)
            {
                _decrementRefCount();
                _ptr = other.release();
                _refCount = new int(1);
                _allocator = other.getDeleter();
                ++(*_refCount);
            }
        }

        T* release()
        {
            T* temp = _ptr;
            if (temp)
                _ptr = NULL;
            return temp;
        }

        Del getDeleter() const { return (_allocator); }

        // Reset the managed resource to a new one
        void reset(T* newPtr = NULL, Del deleter = Del())
        {
            if (_ptr != newPtr)
            {
                _decrementRefCount();
                _ptr = newPtr;
                _allocator = deleter;
                _refCount = new int(1);
            }
        }

        // Accessors
        T*          get() const { return (_ptr); }

        T&          operator[](const std::size_t index)
        {
            assert(_ptr != NULL);
            return _ptr[index];
        }
        const T&    operator[](const std::size_t index) const
        {
            assert(_ptr != NULL);
            return _ptr[index];
        }

    private:
        T*      _ptr;
        int*    _refCount;
        Del     _allocator;

        void _decrementRefCount()
        {
            if (--(*_refCount) == 0)
            {
                if (_ptr)
                {
                    delete[] _ptr;  //no choice
                    _ptr = NULL;
                }
                if (_refCount)
                {
                    delete _refCount;
                    _refCount = NULL;
                }
            }
        }
};

template <typename T>
SharedPtr<T[]> make_SharedArray(size_t size)
{
    return (SharedPtr<T[]>(new T[size]));
}

#endif
