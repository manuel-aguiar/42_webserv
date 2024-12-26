/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ThreadWorker.hpp							   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com	+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/08/26 13:29:44 by mmaria-d		  #+#	#+#			 */
/*   Updated: 2024/08/27 14:44:19 by mmaria-d		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#ifndef THREADWORKER_HPP

# define THREADWORKER_HPP

// Project headers
# include "../ThreadPool/ThreadPool.hpp"

// C headers
# include <pthread.h>

class IThreadTask;
class TaskQueue;

class ThreadPool::ThreadWorker
{
	public:
		ThreadWorker(ThreadPool& pool, TaskQueue& queue, pthread_mutex_t& statusLock, pthread_cond_t& exitSignal);
		~ThreadWorker();
		ThreadWorker(const ThreadWorker& copy);
		ThreadWorker& operator=(const ThreadWorker& assign);

		const List<ThreadWorker, MPool_FixedElem<ThreadWorker> >::iterator&
				getLocation() const;
		void	setLocation(List<ThreadWorker, MPool_FixedElem<ThreadWorker> >::iterator location);

		void	start();
		void	finish();
		
	private:
		void	run();

		static void*   ThreadFunction(void* args);

		enum EThreadState
		{
			EThread_Unitialized,
			EThread_Initialized,
			EThread_Joinable,
			EThread_Joined
		};

		EThreadState			m_state;
		pthread_t	   			m_thread;

		ThreadPool&				m_pool;
		TaskQueue&				m_queue;
		IThreadTask*		 	m_curTask;
		pthread_mutex_t&		m_statusLock;
		pthread_cond_t&			m_exitSignal;

		List<ThreadWorker, MPool_FixedElem<ThreadWorker> >::iterator m_location;
};


#endif