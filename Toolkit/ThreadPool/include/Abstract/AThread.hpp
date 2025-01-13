/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   AThread.hpp										:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com	+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/08/26 13:22:44 by mmaria-d		  #+#	#+#			 */
/*   Updated: 2024/08/27 14:07:30 by mmaria-d		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#ifndef ATHREAD_HPP

# define ATHREAD_HPP

# include <iostream>
# include <pthread.h>
# include <signal.h>

/*
	ThreadFunction must be static to allow being called by all members.
	ThreadFunction recasts to a pointer of type AThread, but it is not this
*/

class AThread
{
	public:
		virtual ~AThread();
		void	start();
		void	join();
		bool	joinable();
	
	protected:
		AThread();
		AThread(const AThread& copy);
		virtual void					run() = 0;

		enum EThreadState
		{
			EThread_Unitialized,
			EThread_Initialized,
			EThread_Joinable,
			EThread_Joined
		};

	private:

		static void*   ThreadFunction(void* args);

		EThreadState	m_state;
		pthread_t	   m_thread;

		
		AThread& operator=(const AThread& assign);
};



#endif