/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UniquePtr.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 07:45:08 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/12 08:48:27 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UNIQUEPTR_HPP

# define UNIQUEPTR_HPP

/*
    Simple Implementation of c++11 std::unique_ptr
*/

# include <cstdlib>
// for "NULL" definition

template <
    typename T
>
class UniquePtr
{
    public:
        UniquePtr(T* newPtr = NULL);
        ~UniquePtr();

        //uses const_cast to allow move semantics (DANGEROUS)
        UniquePtr(const UniquePtr& moveCopy);
        UniquePtr& operator=(const UniquePtr& moveAssign);

        T*          get() const;
        T&          operator*() const;
        T*          operator->() const;
        T*          release();
        void        reset(T* newPtr = NULL);
        
    private:
        T* _ptr;
        
        void        _safeDeletePtr();


};

template <typename T>
UniquePtr<T>::UniquePtr(T* newPtr) : _ptr(newPtr) {}

template <typename T>
UniquePtr<T>::~UniquePtr()
{
    _safeDeletePtr();
}

template <typename T>
UniquePtr<T>::UniquePtr(const UniquePtr& moveCopy)
{
    UniquePtr<T> unConstRef = const_cast<UniquePtr&>(moveCopy);
    _ptr = unConstRef._ptr;
    unConstRef._ptr = NULL;
}

template <typename T>
UniquePtr<T>& UniquePtr<T>::operator=(const UniquePtr& moveAssign)
{
    if (this != &moveAssign)
    {
        UniquePtr<T> unConstRef = const_cast<UniquePtr&>(moveAssign);
        _safeDeletePtr();
        _ptr = unConstRef._ptr;
        unConstRef._ptr = NULL;
    }
    return (*this);
}

template<typename T>
void UniquePtr<T>::_safeDeletePtr()
{
    if (_ptr)
    {
        delete (_ptr);
        _ptr = NULL;
    }
}

template <typename T>
T* UniquePtr<T>::get() const
{
    return (_ptr);
}

template <typename T>
T& UniquePtr<T>::operator*() const
{
    return (*_ptr);
}

template <typename T>
T* UniquePtr<T>::operator->() const
{
    return (_ptr);
}

template <typename T>
T* UniquePtr<T>::release()
{
    T* temp;
    
    temp = _ptr;
    _ptr = NULL;
    return (temp);
}

template <typename T>
void UniquePtr<T>::reset(T* newPtr)
{
    if (_ptr != newPtr)
    {
        _safeDeletePtr();
        _ptr = newPtr;
    }
}



#endif
