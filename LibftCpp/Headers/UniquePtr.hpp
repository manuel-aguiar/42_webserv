/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UniquePtr.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 07:45:08 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/12 13:34:31 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UNIQUEPTR_HPP

# define UNIQUEPTR_HPP
#include <iostream>
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

        //will be called only between two already-constructed objects
        UniquePtr(UniquePtr& moveCopy)
        {
            _ptr = moveCopy._ptr;
            moveCopy._ptr = NULL;
        }

        //will be called only between two already-constructed objects
        UniquePtr& operator=(UniquePtr& moveAssign)
        {
            if (this != &moveAssign)
            {
                _safeDeletePtr();
                _ptr = moveAssign._ptr;
                moveAssign._ptr = NULL;
            }
            return (*this);
        }

        /*
            While this technically works, const cast is dangerous.
            A copy const reference would represent a change in ownership
            Given the constraints of c++11, best to have an explicit transfer of control


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

        T& operator*()
        {
            return (*_ptr);
        }
        const T& operator*() const
        {
            return (*_ptr);
        }

        T* operator->() const
        {
            return (_ptr);
        }

        T* release()
        {
            T* save;
            
            save = _ptr;
            _ptr = NULL;
            return (save);
        }

        void reset(T* newPtr = NULL)
        {
            if (_ptr != newPtr)
            {
                _safeDeletePtr();
                _ptr = newPtr;
            }
        }


        //in the absense of move constructors...
        void transfer(UniquePtr& other)
        {
            if (this != &other)
            {
                reset(other.release());
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

        /*

            While this technically works, const cast is dangerous.
            A copy const reference would represent a change in ownership
            Given the constraints of c++11, best to have an explicit transfer of control



            uses const_cast to allow move semantics (DANGEROUS)
            true copying cannot be done, must release the argument from owning the resource   
            not really missing out on anything     
        

        UniquePtr(const UniquePtr& moveCopy)
        {
            UniquePtr<T>* NonConstPtr;
            NonConstPtr = &(const_cast<UniquePtr&>(moveCopy));
            _ptr = NonConstPtr->_ptr;
            NonConstPtr->_ptr = NULL;
        }

        
            uses const_cast to allow move semantics (DANGEROUS)
            true copying cannot be done, must release the argument from owning the resource   
            not really missing out on anything     
        

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
        */

#endif
