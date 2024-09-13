/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UniquePtr.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 07:45:08 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/13 12:45:28 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UNIQUEPTR_HPP

# define UNIQUEPTR_HPP

/*
    Simple Implementation of c++11 std::unique_ptr

    Unlike other classes where such feature would be desired, we cannot "copy"
    a unique pointer as it would no longer be "unique"

    Copy/assignment must change the argument object, by removing its ownership
    (setting its pointer to NULL) after copying its value.

    So, forget: 
        UniquePtr<T>(const UniquePtr<T>&) and UniquePtr<T>& operator=(const UniquePtr<T>&)

    For the effect, the arguments MUST be non-const references:
        UniquePtr<T>(UniquePtr<T>&) and UniquePtr<T>& operator=(UniquePtr<T>&)

    such that we can change ownership.


    If we were in C++11, we could use move semantics to differentiate them from the copy versions.


    Downsides (are they?):
        like in C++11, a class who has a member variable that is a unique pointer, cannot be copied.
            - makes total sense, then what would one want a unique ptr for???
            - classes that have uniquePtr member variables will have to do the same construction/assignment pattern
            to the uniquePtr itself:
                - declare and not implement the copy/assignment;
                - declare and implement the move copy/assignment (non-const);
            - and fortunatelly, such classes can have members without move constructors, compiler will link with their const versions
            :)
                - best upgrade to const than downgrade to non-const, right?
        - Examples in the /Tests folder
    
    Fortunatelly we are avoiding the const_cast BS which looking back, would be a semantic mess
    (and a dangerous one at that)
        - Old attempt at the end of the file, for historic purposes

*/

# include <cstdlib>
# include <stdexcept>
// for "NULL" definition

#include "DefaultDeleters.hpp"


template <typename T, typename Del = DefaultDeleter<T> >
class UniquePtr
{
    public:

        UniquePtr(T* newPtr = NULL, Del deleter = Del()) : _ptr(newPtr), _deleter(deleter){}

        ~UniquePtr()
        {
            _safeDeletePtr();
        }

        //declared but not defined, compiler will link with the non-const version
        UniquePtr(const UniquePtr& moveCopy);
        UniquePtr& operator=(const UniquePtr& moveAssign);

        UniquePtr(UniquePtr& moveCopy)
        {
            _ptr = moveCopy._ptr;
            _deleter = moveCopy._deleter;
            moveCopy._ptr = NULL;
        }

        //will be called only between two already-constructed objects
        UniquePtr& operator=(UniquePtr& moveAssign)
        {
            //std::cout << "              move assign" << std::endl;
            if (this != &moveAssign)
            {
                _safeDeletePtr();
                _ptr = moveAssign._ptr;
                _deleter = moveAssign._deleter;
                moveAssign._ptr = NULL;
            }
            return (*this);
        }


        T*          get() const { return (_ptr); }
        
        T&          operator*()
        {
            if (!_ptr)
                throw std::runtime_error("Dereferencing a null pointer");
            return (*_ptr);
        }

        const T&    operator*() const
        {
            if (!_ptr)
                throw std::runtime_error("Dereferencing a null pointer");
            return (*_ptr);
        }

        T*          operator->() const
        {
            if (!_ptr)
                throw std::runtime_error("Accessing member functions of a null pointer");
            return (_ptr);
        }

        T* release()
        {
            T* save;
            
            save = _ptr;
            _ptr = NULL;
            return (save);
        }

        Del getDeleter() const { return (_deleter); }

        void reset(T* newPtr = NULL, Del deleter = Del())
        {
            if (_ptr != newPtr)
            {
                _safeDeletePtr();
                _ptr = newPtr;
                _deleter = deleter;
            }
        }
        //in the absense of move constructors...
        void transfer(UniquePtr& other)
        {
            if (this != &other)
                *this = other;
        }

    private:
        T*      _ptr;
        Del     _deleter;

        void _safeDeletePtr()
        {
            if (_ptr)
            {
                _deleter (_ptr);
                _ptr = NULL;
            }
        }


};

/*
    Oh yeah make_UniquePtr specializations for constructors with up to 5 arguments
*/

template <typename T>
UniquePtr<T> make_UniquePtr()
{
    return (UniquePtr<T>(new T()));
}

template <typename T, typename Arg1>
UniquePtr<T> make_UniquePtr(Arg1 arg1)
{
    return (UniquePtr<T>(new T(arg1)));
}

template <typename T, typename Arg1, typename Arg2>
UniquePtr<T> make_UniquePtr(Arg1 arg1, Arg2 arg2)
{
    return (UniquePtr<T>(new T(arg1, arg2)));
}

template <typename T, typename Arg1, typename Arg2, typename Arg3>
UniquePtr<T> make_UniquePtr(Arg1 arg1, Arg2 arg2, Arg3 arg3)
{
    return (UniquePtr<T>(new T(arg1, arg2, arg3)));
}

template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
UniquePtr<T> make_UniquePtr(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4)
{
    return (UniquePtr<T>(new T(arg1, arg2, arg3, arg4)));
}

template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
UniquePtr<T> make_UniquePtr(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5)
{
    return (UniquePtr<T>(new T(arg1, arg2, arg3, arg4, arg5)));
}


//Template Specialization for arrays
template <typename T, typename Del>
class UniquePtr<T[], Del>
{

    public:
        UniquePtr(T* newPtr = NULL, Del deleter = Del()) : _ptr(newPtr), _deleter(deleter) {}

        ~UniquePtr()
        {
            _safeDeletePtr();
        }

        //declared but not defined, compiler will link with the move constructor/assignment
        UniquePtr(const UniquePtr& copy);
        UniquePtr& operator=(const UniquePtr& assign);

        // Move constructor
        UniquePtr(UniquePtr& moveCopy)
        {
            _ptr = moveCopy._ptr;
            _deleter = moveCopy._deleter;
            moveCopy._ptr = NULL;
        }

        // Move assignment
        UniquePtr& operator=(UniquePtr& moveAssign)
        {
            if (this != &moveAssign)
            {
                _safeDeletePtr();
                _ptr = moveAssign._ptr;
                _deleter = moveAssign._deleter;
                moveAssign._ptr = NULL;
            }
            return (*this);
        }

        T*          get() const { return (_ptr); }

        T&          operator[](const std::size_t index)
        {
            if (!_ptr)
                throw std::runtime_error("Dereferencing a null pointer");
            return _ptr[index];
        }
        const T&    operator[](const std::size_t index) const
        {
            if (!_ptr)
                throw std::runtime_error("Dereferencing a null pointer");
            return _ptr[index];
        }

        
        T* release()
        {
            T* save = _ptr;
            _ptr = NULL;
            return save;
        }

        Del getDeleter() const { return (_deleter); }

        void reset(T* newPtr = NULL, Del deleter = Del())
        {
            if (_ptr != newPtr)
            {
                _safeDeletePtr();
                _ptr = newPtr;
                _deleter = deleter;
            }
        }

        void transfer(UniquePtr& other)
        {
            if (this != &other)
                *this = other;
        }

    private:
        T*      _ptr;
        Del     _deleter;


        void _safeDeletePtr()
        {
            if (_ptr)
            {
                delete[] (_ptr);    //no choice
                _ptr = NULL;
            }
        }

};


template <typename T>
UniquePtr<T[]> make_UniqueArray(size_t size)
{
    return (UniquePtr<T[]>(new T[size]));
}

/*

    History:

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
