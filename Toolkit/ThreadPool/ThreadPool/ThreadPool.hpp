/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ThreadPool.hpp									 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com	+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/08/27 09:30:02 by mmaria-d		  #+#	#+#			 */
/*   Updated: 2024/08/30 08:58:09 by mmaria-d		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#ifndef THREADPOOL_HPP

# define THREADPOOL_HPP

// Project headers

# include "../../Arrays/DynArray/DynArray.hpp"
# include "../ThreadTask/ThreadTask.tpp"
# include "../TaskQueue/TaskQueue.hpp"

// C++ headers
# include <vector>

class ThreadPool
{
	public:
		ThreadPool(unsigned int InitialNumberOfThreads);
		~ThreadPool();

		void	waitForCompletion();
		void	destroy(bool waitForCompletion);

		void	addThread();
		void	removeThread();
		
		int		threadCount() const;

	private:

		class ThreadWorker;

		TaskQueue							m_taskQueue;
		DynArray<ThreadWorker>				m_threads;
		pthread_mutex_t						m_statusLock;
		pthread_cond_t						m_exitSignal;



	//public template specializations for ThreadTask
	public:
			template<typename Return, typename Args>
			void addTask(Return (*function)(Args), Args args, Return* placeReturn = NULL)
			{
				IThreadTask* task = new ThreadTask<Return (*)(Args)>(function, args, placeReturn);
				m_taskQueue.addTask(task);
			}
					template<typename Return, typename Arg1, typename Arg2>
					void addTask(Return (*function)(Arg1, Arg2), Arg1 arg1, Arg2 arg2, Return* placeReturn = NULL)
					{
						IThreadTask* task = new ThreadTask<Return (*)(Arg1, Arg2)>(function, arg1, arg2, placeReturn);
						m_taskQueue.addTask(task);
					}
					template<typename Return, typename Arg1, typename Arg2, typename Arg3>
					void addTask(Return (*function)(Arg1, Arg2, Arg3), Arg1 arg1, Arg2 arg2, Arg3 arg3, Return* placeReturn = NULL)
					{
						IThreadTask* task = new ThreadTask<Return (*)(Arg1, Arg2, Arg3)>(function, arg1, arg2, arg3, placeReturn);
						m_taskQueue.addTask(task);
					}
					template<typename Return, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
					void addTask(Return (*function)(Arg1, Arg2, Arg3, Arg4), Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Return* placeReturn = NULL)
					{
						IThreadTask* task = new ThreadTask<Return (*)(Arg1, Arg2, Arg3, Arg4)>(function, arg1, arg2, arg3, arg4, placeReturn);
						m_taskQueue.addTask(task);
					}
					template<typename Return, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
					void addTask(Return (*function)(Arg1, Arg2, Arg3, Arg4, Arg5), Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Return* placeReturn = NULL)
					{
						IThreadTask* task = new ThreadTask<Return (*)(Arg1, Arg2, Arg3, Arg4, Arg5)>(function, arg1, arg2, arg3, arg4, arg5, placeReturn);
						m_taskQueue.addTask(task);
					}






		template<typename Return>
		void addTask(Return (*function)(void), Return* placeReturn = NULL)
		{
			IThreadTask* task = new ThreadTask<Return (*)(void)>(function, placeReturn);
			m_taskQueue.addTask(task);
		}

		template<typename Arg1>
		void addTask(void (*function)(Arg1), Arg1 arg1)
		{
			IThreadTask* task = new ThreadTask<void (*)(Arg1)>(function, arg1);
			m_taskQueue.addTask(task);
		}

					// Specialization for 2 arguments
					template<typename Arg1, typename Arg2>
					void addTask(void (*function)(Arg1, Arg2), Arg1 arg1, Arg2 arg2)
					{
						IThreadTask* task = new ThreadTask<void (*)(Arg1, Arg2)>(function, arg1, arg2);
						m_taskQueue.addTask(task);
					}

					// Specialization for 3 arguments
					template<typename Arg1, typename Arg2, typename Arg3>
					void addTask(void (*function)(Arg1, Arg2, Arg3), Arg1 arg1, Arg2 arg2, Arg3 arg3)
					{
						IThreadTask* task = new ThreadTask<void (*)(Arg1, Arg2, Arg3)>(function, arg1, arg2, arg3);
						m_taskQueue.addTask(task);
					}

					// Specialization for 4 arguments
					template<typename Arg1, typename Arg2, typename Arg3, typename Arg4>
					void addTask(void (*function)(Arg1, Arg2, Arg3, Arg4), Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4)
					{
						IThreadTask* task = new ThreadTask<void (*)(Arg1, Arg2, Arg3, Arg4)>(function, arg1, arg2, arg3, arg4);
						m_taskQueue.addTask(task);
					}

					// Specialization for 5 arguments
					template<typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
					void addTask(void (*function)(Arg1, Arg2, Arg3, Arg4, Arg5), Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5)
					{
						IThreadTask* task = new ThreadTask<void (*)(Arg1, Arg2, Arg3, Arg4, Arg5)>(function, arg1, arg2, arg3, arg4, arg5);
						m_taskQueue.addTask(task);
					}

		void addTask(void (*function)(void))
		{
			IThreadTask* task = new ThreadTask<void (*)(void)>(function);
			m_taskQueue.addTask(task);
		}




		template<typename Class, typename Arg1, typename Return>
		void addTask(Class& instance, Return (Class::*function)(Arg1), Arg1 arg1, Return* placeReturn = NULL)
		{
			IThreadTask* task = new ThreadTask<Return (Class::*)(Arg1)>(instance, function, arg1, placeReturn);
			m_taskQueue.addTask(task);
		}

					// Specialization for member functions with 2 arguments
					template<typename Class, typename Arg1, typename Arg2, typename Return>
					void addTask(Class& instance, Return (Class::*function)(Arg1, Arg2), Arg1 arg1, Arg2 arg2, Return* placeReturn = NULL)
					{
						IThreadTask* task = new ThreadTask<Return (Class::*)(Arg1, Arg2)>(instance, function, arg1, arg2, placeReturn);
						m_taskQueue.addTask(task);
					}

					// Specialization for member functions with 3 arguments
					template<typename Class, typename Arg1, typename Arg2, typename Arg3, typename Return>
					void addTask(Class& instance, Return (Class::*function)(Arg1, Arg2, Arg3), Arg1 arg1, Arg2 arg2, Arg3 arg3, Return* placeReturn = NULL)
					{
						IThreadTask* task = new ThreadTask<Return (Class::*)(Arg1, Arg2, Arg3)>(instance, function, arg1, arg2, arg3, placeReturn);
						m_taskQueue.addTask(task);
					}

					// Specialization for member functions with 4 arguments
					template<typename Class, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Return>
					void addTask(Class& instance, Return (Class::*function)(Arg1, Arg2, Arg3, Arg4), Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Return* placeReturn = NULL)
					{
						IThreadTask* task = new ThreadTask<Return (Class::*)(Arg1, Arg2, Arg3, Arg4)>(instance, function, arg1, arg2, arg3, arg4, placeReturn);
						m_taskQueue.addTask(task);
					}

					// Specialization for member functions with 5 arguments
					template<typename Class, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Return>
					void addTask(Class& instance, Return (Class::*function)(Arg1, Arg2, Arg3, Arg4, Arg5), Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Return* placeReturn = NULL)
					{
						IThreadTask* task = new ThreadTask<Return (Class::*)(Arg1, Arg2, Arg3, Arg4, Arg5)>(instance, function, arg1, arg2, arg3, arg4, arg5, placeReturn);
						m_taskQueue.addTask(task);
					}







		// Specialization for member functions with 1 argument
		template<typename Class, typename Arg1>
		void addTask(Class& instance, void (Class::*function)(Arg1), Arg1 arg1)
		{
			IThreadTask* task = new ThreadTask<void (Class::*)(Arg1)>(instance, function, arg1);
			m_taskQueue.addTask(task);
		}

				// Specialization for member functions with 2 arguments
				template<typename Class, typename Arg1, typename Arg2>
				void addTask(Class& instance, void (Class::*function)(Arg1, Arg2), Arg1 arg1, Arg2 arg2)
				{
					IThreadTask* task = new ThreadTask<void (Class::*)(Arg1, Arg2)>(instance, function, arg1, arg2);
					m_taskQueue.addTask(task);
				}

				// Specialization for member functions with 3 arguments
				template<typename Class, typename Arg1, typename Arg2, typename Arg3>
				void addTask(Class& instance, void (Class::*function)(Arg1, Arg2, Arg3), Arg1 arg1, Arg2 arg2, Arg3 arg3)
				{
					IThreadTask* task = new ThreadTask<void (Class::*)(Arg1, Arg2, Arg3)>(instance, function, arg1, arg2, arg3);
					m_taskQueue.addTask(task);
				}

				// Specialization for member functions with 4 arguments
				template<typename Class, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
				void addTask(Class& instance, void (Class::*function)(Arg1, Arg2, Arg3, Arg4), Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4)
				{
					IThreadTask* task = new ThreadTask<void (Class::*)(Arg1, Arg2, Arg3, Arg4)>(instance, function, arg1, arg2, arg3, arg4);
					m_taskQueue.addTask(task);
				}

				// Specialization for member functions with 5 arguments
				template<typename Class, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
				void addTask(Class& instance, void (Class::*function)(Arg1, Arg2, Arg3, Arg4, Arg5), Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5)
				{
					IThreadTask* task = new ThreadTask<void (Class::*)(Arg1, Arg2, Arg3, Arg4, Arg5)>(instance, function, arg1, arg2, arg3, arg4, arg5);
					m_taskQueue.addTask(task);
				}


		template <typename Class, typename Return>
		void addTask(Class& instance, Return (Class::*function)(void), Return* placeReturn = NULL)
		{
			IThreadTask* task = new ThreadTask<Return (Class::*)(void)>(instance, function, placeReturn);
			m_taskQueue.addTask(task);
		}

		template<typename Class>
		void addTask(Class& instance, void (Class::*function)(void))
		{
			IThreadTask* task = new ThreadTask<void (Class::*)(void)>(instance, function);
			m_taskQueue.addTask(task);
		}

		// Specialization for member functions with 1 argument
		template<typename Class, typename Arg1, typename Return>
		void addTask(const Class& instance, Return (Class::*function)(Arg1) const, Arg1 arg1, Return* placeReturn = NULL)
		{
			IThreadTask* task = new ThreadTask<Return (Class::*)(Arg1) const>(instance, function, arg1, placeReturn);
			m_taskQueue.addTask(task);
		}

				// Specialization for member functions with 2 arguments
				template<typename Class, typename Arg1, typename Arg2, typename Return>
				void addTask(const Class& instance, Return (Class::*function)(Arg1, Arg2) const, Arg1 arg1, Arg2 arg2, Return* placeReturn = NULL)
				{
					IThreadTask* task = new ThreadTask<Return (Class::*)(Arg1, Arg2) const>(instance, function, arg1, arg2, placeReturn);
					m_taskQueue.addTask(task);
				}

				// Specialization for member functions with 3 arguments
				template<typename Class, typename Arg1, typename Arg2, typename Arg3, typename Return>
				void addTask(const Class& instance, Return (Class::*function)(Arg1, Arg2, Arg3) const, Arg1 arg1, Arg2 arg2, Arg3 arg3, Return* placeReturn = NULL)
				{
					IThreadTask* task = new ThreadTask<Return (Class::*)(Arg1, Arg2, Arg3) const>(instance, function, arg1, arg2, arg3, placeReturn);
					m_taskQueue.addTask(task);
				}

				// Specialization for member functions with 4 arguments
				template<typename Class, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Return>
				void addTask(const Class& instance, Return (Class::*function)(Arg1, Arg2, Arg3, Arg4) const, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Return* placeReturn = NULL)
				{
					IThreadTask* task = new ThreadTask<Return (Class::*)(Arg1, Arg2, Arg3, Arg4) const>(instance, function, arg1, arg2, arg3, arg4, placeReturn);
					m_taskQueue.addTask(task);
				}

				// Specialization for member functions with 5 arguments
				template<typename Class, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Return>
				void addTask(const Class& instance, Return (Class::*function)(Arg1, Arg2, Arg3, Arg4, Arg5) const, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Return* placeReturn = NULL)
				{
					IThreadTask* task = new ThreadTask<Return (Class::*)(Arg1, Arg2, Arg3, Arg4, Arg5) const>(instance, function, arg1, arg2, arg3, arg4, arg5, placeReturn);
					m_taskQueue.addTask(task);
				}


		// Specialization for member functions with 1 argument
		template<typename Class, typename Arg1>
		void addTask(const Class& instance, void (Class::*function)(Arg1) const, Arg1 arg1)
		{
			IThreadTask* task = new ThreadTask<void (Class::*)(Arg1) const>(instance, function, arg1);
			m_taskQueue.addTask(task);
		}

				// Specialization for member functions with 2 arguments
				template<typename Class, typename Arg1, typename Arg2>
				void addTask(const Class& instance, void (Class::*function)(Arg1, Arg2) const, Arg1 arg1, Arg2 arg2)
				{
					IThreadTask* task = new ThreadTask<void (Class::*)(Arg1, Arg2) const>(instance, function, arg1, arg2);
					m_taskQueue.addTask(task);
				}

				// Specialization for member functions with 3 arguments
				template<typename Class, typename Arg1, typename Arg2, typename Arg3>
				void addTask(const Class& instance, void (Class::*function)(Arg1, Arg2, Arg3) const, Arg1 arg1, Arg2 arg2, Arg3 arg3)
				{
					IThreadTask* task = new ThreadTask<void (Class::*)(Arg1, Arg2, Arg3) const>(instance, function, arg1, arg2, arg3);
					m_taskQueue.addTask(task);
				}

				// Specialization for member functions with 4 arguments
				template<typename Class, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
				void addTask(const Class& instance, void (Class::*function)(Arg1, Arg2, Arg3, Arg4) const, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4)
				{
					IThreadTask* task = new ThreadTask<void (Class::*)(Arg1, Arg2, Arg3, Arg4) const>(instance, function, arg1, arg2, arg3, arg4);
					m_taskQueue.addTask(task);
				}

				// Specialization for member functions with 5 arguments
				template<typename Class, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
				void addTask(const Class& instance, void (Class::*function)(Arg1, Arg2, Arg3, Arg4, Arg5) const, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5)
				{
					IThreadTask* task = new ThreadTask<void (Class::*)(Arg1, Arg2, Arg3, Arg4, Arg5) const>(instance, function, arg1, arg2, arg3, arg4, arg5);
					m_taskQueue.addTask(task);
				}


		template <typename Class, typename Return>
		void addTask(const Class& instance, Return (Class::*function)(void) const, Return* placeReturn = NULL)
		{
			IThreadTask* task = new ThreadTask<Return (Class::*)(void) const>(instance, function, placeReturn);
			m_taskQueue.addTask(task);
		}

		template<typename Class>
		void addTask(const Class& instance, void (Class::*function)(void) const)
		{
			IThreadTask* task = new ThreadTask<void (Class::*)(void) const>(instance, function);
			m_taskQueue.addTask(task);
		}		 

};




#endif