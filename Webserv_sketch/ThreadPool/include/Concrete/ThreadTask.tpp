/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ThreadTask.tpp									 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com	+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/08/26 09:09:46 by mmaria-d		  #+#	#+#			 */
/*   Updated: 2024/08/29 16:11:35 by mmaria-d		 ###   ########.fr	   */
/*																			*/
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


/*

	Attempt at generalizing a ThreadTask that can take functions returning, non returning
	and Classs member functions, returning and non returning, both up to a single argument
	No access to c++11 variadic functions.

	Then chatgpt did all the prototypes up to 5 arguments, saved me hours
*/

//Forward Declaration: a function pointer, whether it is a non-member or a member function
template <
	typename FunctionPointer
>
class ThreadTask;


// Specialization for a non-member function which takes arguments and returns
# include "ThreadTask_NonMemb_Args_Return.tpp"


// Specialization for a non-member function which takes arguments and does NOT return
# include "ThreadTask_NonMemb_Args_NoReturn.tpp"

// specialization for non-member function with no args, but with a return
template <
	typename Return
>
class ThreadTask<Return (*)(void)> : public IThreadTask
{
	public:
		ThreadTask(Return (*function)(void), Return* placeReturn = NULL) :
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

		void			execute() const
		{
			if (!_function)
				return ;
			if (_placeReturn)
				*_placeReturn = (*_function)();
			else
				(*_function)();
		};
		IThreadTask*	clone() const
		{
			return (new ThreadTask(*this));
		};

	private:
		Return		  (*_function)(void);
		Return*		 _placeReturn;
};



// specialization for non-member function with no args, and no return
template <>
class ThreadTask<void (*)(void)> : public IThreadTask
{
	public:
		ThreadTask(void (*function)(void)) :
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

		void			execute() const
		{
			if (!_function)
				return ;
			(*_function)();
		};
		IThreadTask*	clone() const
		{
			return (new ThreadTask(*this));
		};

	private:
		void		  (*_function)(void);
};



// Specialization for a member function, with one argument and that returns
# include "ThreadTask_Memb_Args_Return.tpp"

// Specialization for a member function, with one argument and no return
# include "ThreadTask_Memb_Args_NoReturn.tpp"

// Specialization for a member function, with no argument and that returns
template <
	typename Class,
	typename Return
>
class ThreadTask<Return (Class::*)(void)> : public IThreadTask
{
	public:
		ThreadTask(Class& instance, Return (Class::*function)(void), Return* placeReturn = NULL) :
			_instance(instance),
			_function(function),
			_placeReturn(placeReturn)
		{};
		~ThreadTask() {};
		ThreadTask(const ThreadTask& copy) :
			_instance(copy._instance),
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

		void			execute() const
		{
			if (!_function)
				return ;
			if (_placeReturn)
				*_placeReturn = (_instance.*_function)();
			else
				(_instance.*_function)();
		};
		IThreadTask*	clone() const
		{
			return (new ThreadTask(*this));
		};

	private:
		Class&		  _instance;
		Return		  (Class::*_function)(void);
		Return*		 _placeReturn;
};

// Specialization for a member function, with no argument and no return
template <
	typename Class
>
class ThreadTask<void (Class::*)(void)> : public IThreadTask
{
	public:
		ThreadTask(Class& instance, void (Class::*function)(void)) :
			_instance(instance),
			_function(function)
		{};
		~ThreadTask() {};
		ThreadTask(const ThreadTask& copy) :
			_instance(copy._instance),
			_function(copy._function)
		{};
		ThreadTask& operator=(const ThreadTask& assign)
		{
			if (this == &assign)
				return (*this);
			*this = assign;
			return (*this);
		}

		void			execute() const
		{
			if (!_function)
				return ;
			(_instance.*_function)();
		};
		IThreadTask*	clone() const
		{
			return (new ThreadTask(*this));
		};

	private:
		Class&		  _instance;
		void			(Class::*_function)();
};



/*


	Const BS


*/

// Specialization for a member function, with one argument and that returns
# include "ThreadTask_Memb_Args_Return_Const.tpp"

// Specialization for a member function, with one argument and no return
# include "ThreadTask_Memb_Args_NoReturn_Const.tpp"


// Specialization for a member function, with no argument and that returns
template <
	typename Class,
	typename Return
>
class ThreadTask<Return (Class::*)(void) const> : public IThreadTask
{
	public:
		ThreadTask(const Class& instance, Return (Class::*function)(void) const, Return* placeReturn = NULL) :
			_instance(instance),
			_function(function),
			_placeReturn(placeReturn)
		{};
		~ThreadTask() {};
		ThreadTask(const ThreadTask& copy) :
			_instance(copy._instance),
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

		void			execute() const
		{
			if (!_function)
				return ;
			if (_placeReturn)
				*_placeReturn = (_instance.*_function)();
			else
				(_instance.*_function)();
		};
		IThreadTask*	clone() const
		{
			return (new ThreadTask(*this));
		};

	private:
		const Class&		  _instance;
		Return		  (Class::*_function)(void) const;
		Return*		 _placeReturn;
};

// Specialization for a member function, with no argument and no return
template <
	typename Class
>
class ThreadTask<void (Class::*)(void) const> : public IThreadTask
{
	public:
		ThreadTask(const Class& instance, void (Class::*function)(void) const) :
			_instance(instance),
			_function(function)
		{};
		~ThreadTask() {};
		ThreadTask(const ThreadTask& copy) :
			_instance(copy._instance),
			_function(copy._function)
		{};
		ThreadTask& operator=(const ThreadTask& assign)
		{
			if (this == &assign)
				return (*this);
			*this = assign;
			return (*this);
		}

		void			execute() const
		{
			if (!_function)
				return ;
			(_instance.*_function)();
		};
		IThreadTask*	clone() const
		{
			return (new ThreadTask(*this));
		};

	private:
		const Class&	_instance;
		void			(Class::*_function)(void) const;
};

#endif