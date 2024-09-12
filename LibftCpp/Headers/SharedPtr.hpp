/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SharedPtr.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 07:45:21 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/12 13:24:16 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHAREDPTR_HPP

# define SHAREDPTR_HPP

template <typename T>
class SharedPtr
{
    public:
        SharedPtr(T* ptr = NULL) : _ptr(ptr), _refCount(new int(1)) {}

        SharedPtr(const SharedPtr& other) : _ptr(other._ptr), _refCount(other._refCount)
        {
            ++(*_refCount);
        }

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

        T* release()
        {
            T* temp = _ptr;
            if (temp)
            {
                _ptr = NULL;
                // Do not decrement ref count here, because we are transferring ownership
            }
            return temp;
        }

        // Reset the managed resource to a new one
        void reset(T* newPtr = NULL)
        {
            if (_ptr != newPtr)
            {
                _decrementRefCount();
                _ptr = newPtr;
                _refCount = new int(1);
            }
        }

        // Accessors
        T* get() const { return (_ptr); }
        T& operator*() { return *_ptr; }
        const T& operator*() const { return *_ptr; }
        T* operator->() const { return (_ptr); }

    private:
        T*      _ptr;
        int*    _refCount;

        void _decrementRefCount()
        {
            if (--(*_refCount) == 0)
            {
                if (_ptr)
                {
                    delete _ptr;
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


#endif