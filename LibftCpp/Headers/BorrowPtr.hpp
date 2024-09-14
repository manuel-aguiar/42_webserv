/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BorrowPtr.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 07:45:08 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/14 11:52:36 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BorrowPtr_HPP

# define BorrowPtr_HPP

/*
    This is nothing more than an actual, fan favourite, raw pointer.

    BorrowPtr name just to make sure to the caller that the user has no ownership
    or responsibility for dealocation of the resource.
*/

# include <cstdlib>
# include <cassert>
// for "NULL" definition

#include "DefaultDeleters.hpp"

#include "UniquePtr.hpp"

template <typename T>
class BorrowPtr
{
    public:

        //can be made out a raw pointer
        BorrowPtr(T* newPtr = NULL) : _ptr(newPtr) {}

        //spawns from uniquePtr
        BorrowPtr(const UniquePtr<T>& unique) : _ptr(unique.get()) {}
        BorrowPtr& operator=(const UniquePtr<T>& unique)
        {
            _ptr = unique.get();
            return (*this);
        }
        ~BorrowPtr() {}

        //hardcopied safely
        BorrowPtr(const BorrowPtr& hardCopy) : _ptr(hardCopy._ptr) {}
        BorrowPtr& operator=(const BorrowPtr& hardAssign)
        {
            if (this != &hardAssign)
                _ptr = hardAssign._ptr;
            return (*this);
        }
        
        bool        operator==(const BorrowPtr& other) const { return (_ptr == other._ptr); }

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

        //in the absense of move constructors...
        void borrow(const BorrowPtr& other)
        {
            if (this != &other)
                *this = other;
        }

        void borrow(const UniquePtr<T>& other)
        {
            *this = other;
        }

    private:
        T*      _ptr;

};


//Template Specialization for arrays
template <typename T>
class BorrowPtr<T[]>
{

    public:
        BorrowPtr(T* newPtr = NULL) : _ptr(newPtr) {}
        ~BorrowPtr() {}
        BorrowPtr(const UniquePtr<T[]>& unique) : _ptr(unique.get()) {}
        BorrowPtr& operator=(const UniquePtr<T[]>& unique)
        {
            _ptr = unique.get();
            return (*this);
        }

        BorrowPtr(const BorrowPtr& hardCopy) : _ptr(hardCopy._ptr) {}
        BorrowPtr& operator=(const BorrowPtr& hardAssign)
        {
            if (this != &hardAssign)
                _ptr = hardAssign._ptr;
            return (*this);
        }

        bool        operator==(const BorrowPtr& other) const { return (_ptr == other._ptr); }

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

        void reset(T* newPtr = NULL)
        {
            if (_ptr != newPtr)
                _ptr = newPtr;
        }

        void borrow(BorrowPtr& other)
        {
            *this = other;
        }

        void borrow(UniquePtr<T[]>& other)
        {
            *this = other;
        }

    private:
        T*      _ptr;
};


/*

    History:

    While this technically works, const cast is dangerous.
    A copy const reference would represent a change in ownership
    Given the constraints of c++11, best to have an explicit transfer of control



    uses const_cast to allow move semantics (DANGEROUS)
    true copying cannot be done, must release the argument from owning the resource   
    not really missing out on anything     


        BorrowPtr(const BorrowPtr& hardCopy)
        {
            BorrowPtr<T>* NonConstPtr;
            NonConstPtr = &(const_cast<BorrowPtr&>(hardCopy));
            _ptr = NonConstPtr->_ptr;
            NonConstPtr->_ptr = NULL;
        }


    uses const_cast to allow move semantics (DANGEROUS)
    true copying cannot be done, must release the argument from owning the resource   
    not really missing out on anything     


        BorrowPtr& operator=(const BorrowPtr& hardAssign)
        {
            BorrowPtr<T>* NonConstPtr;

            if (this != &hardAssign)
            {
                NonConstPtr = &(const_cast<BorrowPtr&>(hardAssign));
                _safeDeletePtr();
                _ptr = NonConstPtr->_ptr;
                NonConstPtr->_ptr = NULL;
            }
            return (*this);
        }
        
*/

#endif
