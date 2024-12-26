/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   TaskQueue.hpp								:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: manuel <manuel@student.42.fr>			  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/08/26 13:33:56 by mmaria-d		  #+#	#+#			 */
/*   Updated: 2024/08/29 12:12:12 by manuel		   ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#ifndef THREADTASKQUEUE_HPP

# define THREADTASKQUEUE_HPP

# include "../../List/List.hpp"
# include "../ThreadTask/IThreadTask.hpp"

// C headers
# include <pthread.h>
# include <semaphore.h>


class TaskQueue
{
	public:
		TaskQueue();
		~TaskQueue();

		void				addTask(IThreadTask* newTask);
		IThreadTask*		getTask();
		void				clear();
		void				waitForCompletion();		
		void				finishTask(IThreadTask* delTask);

		int				 	taskCount();

	private:
		
		List<IThreadTask*>								m_tasks;
		unsigned int									m_tasksExecuting;
		pthread_mutex_t									m_taskAccess;
		pthread_cond_t									m_newTaskSignal;								   
		pthread_cond_t									m_allTasksDone;										 

		TaskQueue(const TaskQueue& copy);
		TaskQueue& operator=(const TaskQueue& assign);
};



#endif