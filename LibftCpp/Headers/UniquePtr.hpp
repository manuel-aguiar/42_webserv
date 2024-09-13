/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UniquePtr.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 07:45:08 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/13 09:50:54 by mmaria-d         ###   ########.fr       */
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

# include <iostream>

template <typename T>
class UniquePtr
{
    private:


    public:

        UniquePtr(T* newPtr = NULL) : _ptr(newPtr) {}

        ~UniquePtr()
        {
            _safeDeletePtr();
        }

        //declared but not defined
        UniquePtr(const UniquePtr& moveCopy); // {std::cout << "                copy" << std::endl;(void)moveCopy;}
        UniquePtr& operator=(const UniquePtr& moveAssign); // {std::cout << "               assign" << std::endl;(void)moveAssign; return *this;}

        UniquePtr(UniquePtr& moveCopy)
        {
            //std::cout << "              move copy" << std::endl;
            _ptr = moveCopy._ptr;
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
                moveAssign._ptr = NULL;
            }
            return (*this);
        }


        T*          get() const { return (_ptr); }
        
        T&          operator*() { return *_ptr; }
        const T&    operator*() const { return *_ptr; }

        T*          operator->() const { return (_ptr); }

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
template <typename T>
class UniquePtr<T[]>
{
    private:
        T* _ptr;

        void _safeDeletePtr()
        {
            if (_ptr)
            {
                delete[] _ptr;
                _ptr = NULL;
            }
        }

    public:
        UniquePtr(T* newPtr = NULL) : _ptr(newPtr) {}

        ~UniquePtr()
        {
            _safeDeletePtr();
        }

        // Disable copy constructor and assignment for unique ownership
        UniquePtr(const UniquePtr&);
        UniquePtr& operator=(const UniquePtr&);

        // Move constructor
        UniquePtr(UniquePtr& moveCopy)
        {
            _ptr = moveCopy._ptr;
            moveCopy._ptr = NULL;
        }

        // Move assignment
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

        T*          get() const { return (_ptr); }

        T&          operator[](const std::size_t index) { return _ptr[index];}
        const T&    operator[](const std::size_t index) const { return _ptr[index];}

        T* release()
        {
            T* save = _ptr;
            _ptr = NULL;
            return save;
        }

        void reset(T* newPtr = NULL)
        {
            if (_ptr != newPtr)
            {
                _safeDeletePtr();
                _ptr = newPtr;
            }
        }

        void transfer(UniquePtr& other)
        {
            if (this != &other)
            {
                reset(other.release());
            }
        }
};


template <typename T>
UniquePtr<T[]> make_UniqueArray(size_t size)
{
    return (UniquePtr<T[]>(new T[size]));
}

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
