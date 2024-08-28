/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThreadTask.tpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 09:09:46 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/28 10:18:10 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ThreadTask_HPP

# define ThreadTask_HPP

//class ThreadPool;

# include <pthread.h>
# include <iostream>
# include <cstring>
# include <cassert>

# include "../Abstract/IThreadTask.hpp"
# include <vector>

template <
    typename Func,
    typename Args,
    typename Return
>
class ThreadTask : public IThreadTask
{
    public:
        ThreadTask(Return (*function)(Args), Args& arguments, Return* placeReturn) :
            _function(function),
            _args(arguments),
            _placeReturn(placeReturn)
        {};
        ~ThreadTask() {};
        ThreadTask(const ThreadTask& copy) :
            _function(copy._function),
            _args(copy._args),
            _placeReturn(copy._placeReturn)
        {};
        ThreadTask& operator=(const ThreadTask& assign)
        {
            if (this == &assign)
                return (*this);
            *this = assign;
            return (*this);
        }

        void            execute() const
        {
            if (!_function)
                return ;
            if (_placeReturn)
                *_placeReturn = (*_function)(_args);
            else
                (*_function)(_args);
        };
        IThreadTask*    clone() const
        {
            return (new ThreadTask(*this));
        };

    private:
        Return          (*_function)(Args);
        Args&           _args;
        Return*         _placeReturn;
};


// specialization for no args, but with areturn
template <
    typename Func,
    typename Return
>
class ThreadTask<Func, void, Return> : public IThreadTask
{
    public:
        ThreadTask(Return (*function)(), Return* placeReturn) :
            _function(function),
            _placeReturn(placeReturn)
        {};
        ~ThreadTask() {};
        ThreadTask(const ThreadTask& copy) :
            _function(copy._function),
            _placeReturn(copy._placeReturn)
        {};
        ThreadTask& operator=(const ThreadTask& assign)
        {
            if (this == &assign)
                return (*this);
            *this = assign;
            return (*this);
        }

        void            execute() const
        {
            if (!_function)
                return ;
            if (_placeReturn)
                *_placeReturn = (*_function)();
            else
                (*_function)();
        };
        IThreadTask*    clone() const
        {
            return (new ThreadTask(*this));
        };

    private:
        Return          (*_function)();
        Return*         _placeReturn;
};





//specialization for returning void
template <
    typename Func,
    typename Args
>
class ThreadTask<Func, Args, void> : public IThreadTask
{
    public:
        ThreadTask(void (*function)(Args), Args& arguments) :
            _function(function),
            _args(arguments)
        {};
        ~ThreadTask() {};
        ThreadTask(const ThreadTask& copy) :
            _function(copy._function),
            _args(copy._args)
        {};
        ThreadTask& operator=(const ThreadTask& assign)
        {
            if (this == &assign)
                return (*this);
            *this = assign;
            return (*this);
        }

        void            execute() const
        {
                (*_function)(_args);
        };
        IThreadTask*    clone() const
        {
            return (new ThreadTask(*this));
        };

    private:
        void          (*_function)(Args);
        Args&           _args;
};

// specialization for no args, and no return
template <
    typename Func
>
class ThreadTask<Func, void, void> : public IThreadTask
{
    public:
        ThreadTask(void (*function)()) :
            _function(function)
        {};
        ~ThreadTask() {};
        ThreadTask(const ThreadTask& copy) :
            _function(copy._function)
        {};
        ThreadTask& operator=(const ThreadTask& assign)
        {
            if (this == &assign)
                return (*this);
            *this = assign;
            return (*this);
        }

        void            execute() const
        {
            if (!_function)
                return ;
            (*_function)();
        };
        IThreadTask*    clone() const
        {
            return (new ThreadTask(*this));
        };

    private:
        void          (*_function)();
};

#endif