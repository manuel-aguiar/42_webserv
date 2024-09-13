/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UniqueObj.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 07:44:55 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/13 11:19:03 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UNIQUEOBJ_HPP

# define UNIQUEOBJ_HPP

# include <cstdlib>
// for "NULL" definition

template <typename T>
class UniqueObj
{
    public:

        UniqueObj(T* newPtr = NULL) : _ptr(newPtr) {}

        ~UniqueObj()
        {
            _safeDeletePtr();
        }

        //declared but not defined, compiler will link with the non-const version
        UniqueObj(const UniqueObj& moveCopy);
        UniqueObj& operator=(const UniqueObj& moveAssign);

        UniqueObj(UniqueObj& moveCopy)
        {
            _ptr = moveCopy._ptr;
            moveCopy._ptr = NULL;
        }

        //will be called only between two already-constructed objects
        UniqueObj& operator=(UniqueObj& moveAssign)
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
        void transfer(UniqueObj& other)
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
    Oh yeah make_UniqueObj specializations for constructors with up to 5 arguments
*/

//template <typename T>
//UniqueObj<T> make_UniqueObj()
//{
//    return (UniqueObj<T>(new T()));
//}
//
//template <typename T, typename Arg1>
//UniqueObj<T> make_UniqueObj(Arg1 arg1)
//{
//    return (UniqueObj<T>(new T(arg1)));
//}
//
//template <typename T, typename Arg1, typename Arg2>
//UniqueObj<T> make_UniqueObj(Arg1 arg1, Arg2 arg2)
//{
//    return (UniqueObj<T>(new T(arg1, arg2)));
//}
//
//template <typename T, typename Arg1, typename Arg2, typename Arg3>
//UniqueObj<T> make_UniqueObj(Arg1 arg1, Arg2 arg2, Arg3 arg3)
//{
//    return (UniqueObj<T>(new T(arg1, arg2, arg3)));
//}
//
//template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
//UniqueObj<T> make_UniqueObj(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4)
//{
//    return (UniqueObj<T>(new T(arg1, arg2, arg3, arg4)));
//}
//
//template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
//UniqueObj<T> make_UniqueObj(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5)
//{
//    return (UniqueObj<T>(new T(arg1, arg2, arg3, arg4, arg5)));
//}


//Template Specialization for arrays
template <typename T>
class UniqueObj<T[]>
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
        UniqueObj(T* newPtr = NULL) : _ptr(newPtr) {}

        ~UniqueObj()
        {
            _safeDeletePtr();
        }

        //declared but not defined, compiler will link with the move constructor/assignment
        UniqueObj(const UniqueObj& copy);
        UniqueObj& operator=(const UniqueObj& assign);

        // Move constructor
        UniqueObj(UniqueObj& moveCopy)
        {
            _ptr = moveCopy._ptr;
            moveCopy._ptr = NULL;
        }

        // Move assignment
        UniqueObj& operator=(UniqueObj& moveAssign)
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

        void transfer(UniqueObj& other)
        {
            if (this != &other)
            {
                reset(other.release());
            }
        }
};


//template <typename T>
//UniqueObj<T[]> make_UniqueArray(size_t size)
//{
//    return (UniqueObj<T[]>(new T[size]));
//}

/*

    History:

    While this technically works, const cast is dangerous.
    A copy const reference would represent a change in ownership
    Given the constraints of c++11, best to have an explicit transfer of control



    uses const_cast to allow move semantics (DANGEROUS)
    true copying cannot be done, must release the argument from owning the resource   
    not really missing out on anything     


        UniqueObj(const UniqueObj& moveCopy)
        {
            UniqueObj<T>* NonConstPtr;
            NonConstPtr = &(const_cast<UniqueObj&>(moveCopy));
            _ptr = NonConstPtr->_ptr;
            NonConstPtr->_ptr = NULL;
        }


    uses const_cast to allow move semantics (DANGEROUS)
    true copying cannot be done, must release the argument from owning the resource   
    not really missing out on anything     


        UniqueObj& operator=(const UniqueObj& moveAssign)
        {
            UniqueObj<T>* NonConstPtr;

            if (this != &moveAssign)
            {
                NonConstPtr = &(const_cast<UniqueObj&>(moveAssign));
                _safeDeletePtr();
                _ptr = NonConstPtr->_ptr;
                NonConstPtr->_ptr = NULL;
            }
            return (*this);
        }
        
*/

#endif