/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   AThread.cpp										:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com	+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/08/26 13:26:47 by mmaria-d		  #+#	#+#			 */
/*   Updated: 2024/08/27 15:21:38 by mmaria-d		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "../../include/Abstract/AThread.hpp"

AThread::AThread() : 
	m_state(EThread_Unitialized),
	m_thread(0)
{
	#ifdef DEBUG_CONSTRUCTOR
		std::cout << "AThread Constructor Called" << std::endl;
	#endif
}

AThread::~AThread()
{
	#ifdef DEBUG_CONSTRUCTOR
		std::cout << "AThread Destructor Called" << std::endl;
	#endif   
}

AThread::AThread(const AThread& copy) : 
	m_state(copy.m_state),
	m_thread(copy.m_thread)
{
	#ifdef DEBUG_CONSTRUCTOR
		std::cout << "AThread Copy Constructor Called" << std::endl;
	#endif	   
}

AThread& AThread::operator=(const AThread& assign)
{
	if (&assign == this)
		return (*this);
	*this = assign;
	return (*this);
}

void	AThread::start()
{
	if (m_state == EThread_Initialized)
	{
		std::cerr << "Thread already initialized" << std::endl;
		return ;
	}
	if (pthread_create(&m_thread, NULL, ThreadFunction, this))
	{
		std::cerr << "Thread failed to create" << std::endl;
		return ;
	}
	m_state = EThread_Initialized;
}

void	AThread::join()
{
	pthread_join(m_thread, NULL);
	m_state = EThread_Joined;
}

void*   AThread::ThreadFunction(void* args)
{
	AThread* thread = (AThread *)args;
	thread->run();
	return (NULL);
}