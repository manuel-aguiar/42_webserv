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

# include <vector>

# include "../Abstract/IThreadPool.hpp"
# include "../Abstract/IThreadTask.hpp"

# include "../Concrete/ThreadTask.tpp"
# include "../Concrete/ThreadTaskQueue.hpp"
# include "../Concrete/ThreadPoolWorker.hpp"

// must know about threads, tasks, and queues
// can handle any task that complies with the IThreadTask interface
// must take a predefined queue and threadworker, both complying 
// with IThreadTaskQueue and IThreadPoolWorker interfaces, respectively
// queue and thread are tightely linked

class ThreadPool : public IThreadPool
{
	public:
		ThreadPool(unsigned int InitialNumberOfThreads);
		~ThreadPool();

		void	addTask(IThreadTask* task);
		void	addTask(IThreadTask& task);
		void	waitForCompletion();
		void	destroy(bool waitForCompletion);
		void	addThread();
		void	removeThread();

		//not implemented
		//void	removeThread();
		
		int	 threadCount() const;

	private:
		ThreadTaskQueue						m_taskQueue;
		std::vector<ThreadPoolWorker *>		m_threads;
		pthread_mutex_t						m_statusLock;
		pthread_cond_t						m_exitSignal;



	//public template specializations for ThreadTask
	public:
			template<typename Return, typename Args>
			void addTask(Return (*function)(Args), Args args, Return* placeReturn = NULL)
			{
				ThreadTask<Return (*)(Args)> task(function, args, placeReturn);
				m_taskQueue.addTask(&task);
			}
					template<typename Return, typename Arg1, typename Arg2>
					void addTask(Return (*function)(Arg1, Arg2), Arg1 arg1, Arg2 arg2, Return* placeReturn = NULL)
					{
						ThreadTask<Return (*)(Arg1, Arg2)> task(function, arg1, arg2, placeReturn);
						m_taskQueue.addTask(&task);
					}
					template<typename Return, typename Arg1, typename Arg2, typename Arg3>
					void addTask(Return (*function)(Arg1, Arg2, Arg3), Arg1 arg1, Arg2 arg2, Arg3 arg3, Return* placeReturn = NULL)
					{
						ThreadTask<Return (*)(Arg1, Arg2, Arg3)> task(function, arg1, arg2, arg3, placeReturn);
						m_taskQueue.addTask(&task);
					}
					template<typename Return, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
					void addTask(Return (*function)(Arg1, Arg2, Arg3, Arg4), Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Return* placeReturn = NULL)
					{
						ThreadTask<Return (*)(Arg1, Arg2, Arg3, Arg4)> task(function, arg1, arg2, arg3, arg4, placeReturn);
						m_taskQueue.addTask(&task);
					}
					template<typename Return, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
					void addTask(Return (*function)(Arg1, Arg2, Arg3, Arg4, Arg5), Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Return* placeReturn = NULL)
					{
						ThreadTask<Return (*)(Arg1, Arg2, Arg3, Arg4, Arg5)> task(function, arg1, arg2, arg3, arg4, arg5, placeReturn);
						m_taskQueue.addTask(&task);
					}






		template<typename Return>
		void addTask(Return (*function)(void), Return* placeReturn = NULL)
		{
			ThreadTask<Return (*)(void)> task(function, placeReturn);
			m_taskQueue.addTask(&task);
		}

		template<typename Arg1>
		void addTask(void (*function)(Arg1), Arg1 arg1)
		{
			ThreadTask<void (*)(Arg1)> task(function, arg1);
			m_taskQueue.addTask(&task);
		}

					// Specialization for 2 arguments
					template<typename Arg1, typename Arg2>
					void addTask(void (*function)(Arg1, Arg2), Arg1 arg1, Arg2 arg2)
					{
						ThreadTask<void (*)(Arg1, Arg2)> task(function, arg1, arg2);
						m_taskQueue.addTask(&task);
					}

					// Specialization for 3 arguments
					template<typename Arg1, typename Arg2, typename Arg3>
					void addTask(void (*function)(Arg1, Arg2, Arg3), Arg1 arg1, Arg2 arg2, Arg3 arg3)
					{
						ThreadTask<void (*)(Arg1, Arg2, Arg3)> task(function, arg1, arg2, arg3);
						m_taskQueue.addTask(&task);
					}

					// Specialization for 4 arguments
					template<typename Arg1, typename Arg2, typename Arg3, typename Arg4>
					void addTask(void (*function)(Arg1, Arg2, Arg3, Arg4), Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4)
					{
						ThreadTask<void (*)(Arg1, Arg2, Arg3, Arg4)> task(function, arg1, arg2, arg3, arg4);
						m_taskQueue.addTask(&task);
					}

					// Specialization for 5 arguments
					template<typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
					void addTask(void (*function)(Arg1, Arg2, Arg3, Arg4, Arg5), Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5)
					{
						ThreadTask<void (*)(Arg1, Arg2, Arg3, Arg4, Arg5)> task(function, arg1, arg2, arg3, arg4, arg5);
						m_taskQueue.addTask(&task);
					}

		void addTask(void (*function)(void))
		{
			ThreadTask<void (*)(void)> task(function);
			m_taskQueue.addTask(&task);
		}




		template<typename Class, typename Arg1, typename Return>
		void addTask(Class& instance, Return (Class::*function)(Arg1), Arg1 arg1, Return* placeReturn = NULL)
		{
			ThreadTask<Return (Class::*)(Arg1)> task(instance, function, arg1, placeReturn);
			m_taskQueue.addTask(&task);
		}

					// Specialization for member functions with 2 arguments
					template<typename Class, typename Arg1, typename Arg2, typename Return>
					void addTask(Class& instance, Return (Class::*function)(Arg1, Arg2), Arg1 arg1, Arg2 arg2, Return* placeReturn = NULL)
					{
						ThreadTask<Return (Class::*)(Arg1, Arg2)> task(instance, function, arg1, arg2, placeReturn);
						m_taskQueue.addTask(&task);
					}

					// Specialization for member functions with 3 arguments
					template<typename Class, typename Arg1, typename Arg2, typename Arg3, typename Return>
					void addTask(Class& instance, Return (Class::*function)(Arg1, Arg2, Arg3), Arg1 arg1, Arg2 arg2, Arg3 arg3, Return* placeReturn = NULL)
					{
						ThreadTask<Return (Class::*)(Arg1, Arg2, Arg3)> task(instance, function, arg1, arg2, arg3, placeReturn);
						m_taskQueue.addTask(&task);
					}

					// Specialization for member functions with 4 arguments
					template<typename Class, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Return>
					void addTask(Class& instance, Return (Class::*function)(Arg1, Arg2, Arg3, Arg4), Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Return* placeReturn = NULL)
					{
						ThreadTask<Return (Class::*)(Arg1, Arg2, Arg3, Arg4)> task(instance, function, arg1, arg2, arg3, arg4, placeReturn);
						m_taskQueue.addTask(&task);
					}

					// Specialization for member functions with 5 arguments
					template<typename Class, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Return>
					void addTask(Class& instance, Return (Class::*function)(Arg1, Arg2, Arg3, Arg4, Arg5), Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Return* placeReturn = NULL)
					{
						ThreadTask<Return (Class::*)(Arg1, Arg2, Arg3, Arg4, Arg5)> task(instance, function, arg1, arg2, arg3, arg4, arg5, placeReturn);
						m_taskQueue.addTask(&task);
					}







		// Specialization for member functions with 1 argument
		template<typename Class, typename Arg1>
		void addTask(Class& instance, void (Class::*function)(Arg1), Arg1 arg1)
		{
			ThreadTask<void (Class::*)(Arg1)> task(instance, function, arg1);
			m_taskQueue.addTask(&task);
		}

				// Specialization for member functions with 2 arguments
				template<typename Class, typename Arg1, typename Arg2>
				void addTask(Class& instance, void (Class::*function)(Arg1, Arg2), Arg1 arg1, Arg2 arg2)
				{
					ThreadTask<void (Class::*)(Arg1, Arg2)> task(instance, function, arg1, arg2);
					m_taskQueue.addTask(&task);
				}

				// Specialization for member functions with 3 arguments
				template<typename Class, typename Arg1, typename Arg2, typename Arg3>
				void addTask(Class& instance, void (Class::*function)(Arg1, Arg2, Arg3), Arg1 arg1, Arg2 arg2, Arg3 arg3)
				{
					ThreadTask<void (Class::*)(Arg1, Arg2, Arg3)> task(instance, function, arg1, arg2, arg3);
					m_taskQueue.addTask(&task);
				}

				// Specialization for member functions with 4 arguments
				template<typename Class, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
				void addTask(Class& instance, void (Class::*function)(Arg1, Arg2, Arg3, Arg4), Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4)
				{
					ThreadTask<void (Class::*)(Arg1, Arg2, Arg3, Arg4)> task(instance, function, arg1, arg2, arg3, arg4);
					m_taskQueue.addTask(&task);
				}

				// Specialization for member functions with 5 arguments
				template<typename Class, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
				void addTask(Class& instance, void (Class::*function)(Arg1, Arg2, Arg3, Arg4, Arg5), Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5)
				{
					ThreadTask<void (Class::*)(Arg1, Arg2, Arg3, Arg4, Arg5)> task(instance, function, arg1, arg2, arg3, arg4, arg5);
					m_taskQueue.addTask(&task);
				}


		template <typename Class, typename Return>
		void addTask(Class& instance, Return (Class::*function)(void), Return* placeReturn = NULL)
		{
			ThreadTask<Return (Class::*)(void)> task(instance, function, placeReturn);
			m_taskQueue.addTask(&task);
		}

		template<typename Class>
		void addTask(Class& instance, void (Class::*function)(void))
		{
			ThreadTask<void (Class::*)(void)> task(instance, function);
			m_taskQueue.addTask(&task);
		}

		// Specialization for member functions with 1 argument
		template<typename Class, typename Arg1, typename Return>
		void addTask(const Class& instance, Return (Class::*function)(Arg1) const, Arg1 arg1, Return* placeReturn = NULL)
		{
			ThreadTask<Return (Class::*)(Arg1) const> task(instance, function, arg1, placeReturn);
			m_taskQueue.addTask(&task);
		}

				// Specialization for member functions with 2 arguments
				template<typename Class, typename Arg1, typename Arg2, typename Return>
				void addTask(const Class& instance, Return (Class::*function)(Arg1, Arg2) const, Arg1 arg1, Arg2 arg2, Return* placeReturn = NULL)
				{
					ThreadTask<Return (Class::*)(Arg1, Arg2) const> task(instance, function, arg1, arg2, placeReturn);
					m_taskQueue.addTask(&task);
				}

				// Specialization for member functions with 3 arguments
				template<typename Class, typename Arg1, typename Arg2, typename Arg3, typename Return>
				void addTask(const Class& instance, Return (Class::*function)(Arg1, Arg2, Arg3) const, Arg1 arg1, Arg2 arg2, Arg3 arg3, Return* placeReturn = NULL)
				{
					ThreadTask<Return (Class::*)(Arg1, Arg2, Arg3) const> task(instance, function, arg1, arg2, arg3, placeReturn);
					m_taskQueue.addTask(&task);
				}

				// Specialization for member functions with 4 arguments
				template<typename Class, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Return>
				void addTask(const Class& instance, Return (Class::*function)(Arg1, Arg2, Arg3, Arg4) const, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Return* placeReturn = NULL)
				{
					ThreadTask<Return (Class::*)(Arg1, Arg2, Arg3, Arg4) const> task(instance, function, arg1, arg2, arg3, arg4, placeReturn);
					m_taskQueue.addTask(&task);
				}

				// Specialization for member functions with 5 arguments
				template<typename Class, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Return>
				void addTask(const Class& instance, Return (Class::*function)(Arg1, Arg2, Arg3, Arg4, Arg5) const, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Return* placeReturn = NULL)
				{
					ThreadTask<Return (Class::*)(Arg1, Arg2, Arg3, Arg4, Arg5) const> task(instance, function, arg1, arg2, arg3, arg4, arg5, placeReturn);
					m_taskQueue.addTask(&task);
				}


		// Specialization for member functions with 1 argument
		template<typename Class, typename Arg1>
		void addTask(const Class& instance, void (Class::*function)(Arg1) const, Arg1 arg1)
		{
			ThreadTask<void (Class::*)(Arg1) const> task(instance, function, arg1);
			m_taskQueue.addTask(&task);
		}

				// Specialization for member functions with 2 arguments
				template<typename Class, typename Arg1, typename Arg2>
				void addTask(const Class& instance, void (Class::*function)(Arg1, Arg2) const, Arg1 arg1, Arg2 arg2)
				{
					ThreadTask<void (Class::*)(Arg1, Arg2) const> task(instance, function, arg1, arg2);
					m_taskQueue.addTask(&task);
				}

				// Specialization for member functions with 3 arguments
				template<typename Class, typename Arg1, typename Arg2, typename Arg3>
				void addTask(const Class& instance, void (Class::*function)(Arg1, Arg2, Arg3) const, Arg1 arg1, Arg2 arg2, Arg3 arg3)
				{
					ThreadTask<void (Class::*)(Arg1, Arg2, Arg3) const> task(instance, function, arg1, arg2, arg3);
					m_taskQueue.addTask(&task);
				}

				// Specialization for member functions with 4 arguments
				template<typename Class, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
				void addTask(const Class& instance, void (Class::*function)(Arg1, Arg2, Arg3, Arg4) const, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4)
				{
					ThreadTask<void (Class::*)(Arg1, Arg2, Arg3, Arg4) const> task(instance, function, arg1, arg2, arg3, arg4);
					m_taskQueue.addTask(&task);
				}

				// Specialization for member functions with 5 arguments
				template<typename Class, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
				void addTask(const Class& instance, void (Class::*function)(Arg1, Arg2, Arg3, Arg4, Arg5) const, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5)
				{
					ThreadTask<void (Class::*)(Arg1, Arg2, Arg3, Arg4, Arg5) const> task(instance, function, arg1, arg2, arg3, arg4, arg5);
					m_taskQueue.addTask(&task);
				}


		template <typename Class, typename Return>
		void addTask(const Class& instance, Return (Class::*function)(void) const, Return* placeReturn = NULL)
		{
			ThreadTask<Return (Class::*)(void) const> task(instance, function, placeReturn);
			m_taskQueue.addTask(&task);
		}

		template<typename Class>
		void addTask(const Class& instance, void (Class::*function)(void) const)
		{
			ThreadTask<void (Class::*)(void) const> task(instance, function);
			m_taskQueue.addTask(&task);
		}		 

};




#endif