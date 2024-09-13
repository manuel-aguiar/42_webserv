/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SharedPtr.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 07:45:21 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/13 15:48:56 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHAREDPTR_HPP

# define SHAREDPTR_HPP

# include <cstdlib>
# include <stdexcept>   

#include "UniquePtr.hpp"
#include "DefaultDeleters.hpp"


template <typename T, typename Del = DefaultDeleter<T> >
class SharedPtr
{
    public:
        SharedPtr(T* ptr = NULL, Del deleter = Del()) : _ptr(ptr), _refCount(new int(1)), _deleter(deleter) {}
        SharedPtr (UniquePtr<T>& unique) : _ptr(unique.release()), _refCount(new int(1)), _deleter(unique.getDeleter()) {}
        SharedPtr(const SharedPtr& other) : _ptr(other._ptr), _refCount(other._refCount), _deleter(other._deleter) { ++(*_refCount); }
        
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

        Del getDeleter() const { return (_deleter); }

        // Reset the managed resource to a new one
        void reset(T* newPtr = NULL, Del deleter = Del())
        {
            if (_ptr != newPtr)
            {
                _decrementRefCount();
                _ptr = newPtr;
                _deleter = deleter;
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
        T*      _ptr;
        int*    _refCount;
        Del     _deleter;

        void _decrementRefCount()
        {
            if (--(*_refCount) == 0)
            {
                if (_ptr)
                {
                    _deleter(_ptr);
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
SharedPtr<T> make_SharedPtr()
{
    return (SharedPtr<T>(new T()));
}

template <typename T, typename Arg1>
SharedPtr<T> make_SharedPtr(Arg1 arg1)
{
    return SharedPtr<T>(new T(arg1));
}

template <typename T, typename Arg1, typename Arg2>
SharedPtr<T> make_SharedPtr(Arg1 arg1, Arg2 arg2)
{
    return SharedPtr<T>(new T(arg1, arg2));
}

template <typename T, typename Arg1, typename Arg2, typename Arg3>
SharedPtr<T> make_SharedPtr(Arg1 arg1, Arg2 arg2, Arg3 arg3)
{
    return SharedPtr<T>(new T(arg1, arg2, arg3));
}

template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
SharedPtr<T> make_SharedPtr(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4)
{
    return SharedPtr<T>(new T(arg1, arg2, arg3, arg4));
}

template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
SharedPtr<T> make_SharedPtr(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5)
{
    return SharedPtr<T>(new T(arg1, arg2, arg3, arg4, arg5));
}

// Template specialization for arrays
template <typename T, typename Del>
class SharedPtr<T[], Del>
{
    public:
        SharedPtr(T* ptr = NULL, Del deleter = Del()) : _ptr(ptr), _refCount(new int(1)), _deleter(deleter) {}
        SharedPtr (UniquePtr<T[]>& unique) : _ptr(unique.release()), _refCount(new int(1)), _deleter(unique.getDeleter()) {}
        SharedPtr(const SharedPtr& other) : _ptr(other._ptr), _refCount(other._refCount), _deleter(other._deleter) { ++(*_refCount); }
        
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
                _deleter = other.getDeleter();
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

        Del getDeleter() const { return (_deleter); }

        // Reset the managed resource to a new one
        void reset(T* newPtr = NULL, Del deleter = Del())
        {
            if (_ptr != newPtr)
            {
                _decrementRefCount();
                _ptr = newPtr;
                _deleter = deleter;
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
        Del     _deleter;

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