/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UniquePtr.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 07:45:08 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/12 09:29:53 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UNIQUEPTR_HPP

# define UNIQUEPTR_HPP

/*
    Simple Implementation of c++11 std::unique_ptr
*/

# include <cstdlib>
// for "NULL" definition

template <typename T>
class UniquePtr
{
    public:
        UniquePtr(T* newPtr = NULL) : _ptr(newPtr) {}

        ~UniquePtr()
        {
            _safeDeletePtr();
        }

        /*
            uses const_cast to allow move semantics (DANGEROUS)
            true copying cannot be done, must release the argument from owning the resource   
            not really missing out on anything     
        */

        UniquePtr(const UniquePtr& moveCopy)
        {
            UniquePtr<T>* NonConstPtr;
            NonConstPtr = &(const_cast<UniquePtr&>(moveCopy));
            _ptr = NonConstPtr->_ptr;
            NonConstPtr->_ptr = NULL;
        }

        /*
            uses const_cast to allow move semantics (DANGEROUS)
            true copying cannot be done, must release the argument from owning the resource   
            not really missing out on anything     
        */

        UniquePtr& operator=(const UniquePtr& moveAssign)
        {
            UniquePtr<T>* NonConstPtr;

            if (this != &moveAssign)
            {
                NonConstPtr = &(const_cast<UniquePtr&>(moveAssign));
                _safeDeletePtr();
                _ptr = NonConstPtr->_ptr;
                NonConstPtr->_ptr = NULL;
            }
            return (*this);
        }

        T* get() const
        {
            return (_ptr);
        }

        T& operator*() const
        {
            return (*_ptr);
        }

        T* operator->() const
        {
            return (_ptr);
        }

        T* release()
        {
            T* temp;
            temp = _ptr;
            _ptr = NULL;
            return (temp);
        }

        void reset(T* newPtr = NULL)
        {
            if (_ptr != newPtr)
            {
                _safeDeletePtr();
                _ptr = newPtr;
            }
        }

    private:
        T* _ptr;

        void _safeDeletePtr()
        {
            if (_ptr)
            {
                delete (_ptr);
                _ptr = NULL;
            }
        }
};



#endif
