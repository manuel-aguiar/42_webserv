/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ThreadPoolWorker.hpp							   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com	+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/08/26 13:29:44 by mmaria-d		  #+#	#+#			 */
/*   Updated: 2024/08/27 14:44:19 by mmaria-d		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#ifndef THREADPOOLWORKER_HPP

# define THREADPOOLWORKER_HPP

# include "../Abstract/AThread.hpp"
# include "../Abstract/IThreadTask.hpp"
# include "../Abstract/IThreadTaskQueue.hpp"

//knows about threads, tasks and queues

class ThreadPoolWorker : public AThread
{
	public:
		ThreadPoolWorker(IThreadTaskQueue& queue, pthread_mutex_t& statusLock, pthread_cond_t& exitSignal);
		~ThreadPoolWorker();

		bool	exitedQueue();

	protected:
		void	run();
		
	private:
		IThreadTaskQueue&		_queue;
		IThreadTask*		 	_curTask;
		pthread_mutex_t&		_statusLock;
		pthread_cond_t&			_exitSignal;
		bool					_exited;


		ThreadPoolWorker(const ThreadPoolWorker& copy);
		ThreadPoolWorker& operator=(const ThreadPoolWorker& assign);
};


#endif