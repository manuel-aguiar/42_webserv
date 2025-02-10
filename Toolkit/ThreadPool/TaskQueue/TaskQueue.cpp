

#include "TaskQueue.hpp"
# include "../../Arrays/_FixedCircularQueueImpl/FixedCircularQueueImpl.hpp"

TaskQueue::TaskQueue() : 
    m_tasks(NULL),
    m_tasksExecuting(0)
{
    pthread_mutex_init(&m_taskAccess, NULL);
    pthread_cond_init(&m_newTaskSignal, NULL);
    pthread_cond_init(&m_allTasksDone, NULL);
	pthread_cond_init(&m_fullQueue, NULL);
}

TaskQueue::~TaskQueue()
{
	pthread_mutex_destroy(&m_taskAccess);
	pthread_cond_destroy(&m_newTaskSignal);
	pthread_cond_destroy(&m_allTasksDone);
	pthread_cond_destroy(&m_fullQueue);
}

TaskQueue::TaskQueue(const TaskQueue& copy)  {(void)copy;}

TaskQueue& 
TaskQueue::operator=(const TaskQueue& assign)  {(void)assign; return (*this);}

void    TaskQueue::init(FixedCircularQueueImpl<IThreadTask*>& tasks)
{
    m_tasks = &tasks;
}

void	TaskQueue::finishTask(IThreadTask* delTask)
{
	(void)delTask;
	pthread_mutex_lock(&m_taskAccess);
	m_tasksExecuting--;
	if (m_tasks->size() == 0 && m_tasksExecuting == 0)
		pthread_cond_signal(&m_allTasksDone);
	pthread_mutex_unlock(&m_taskAccess);
}

bool	TaskQueue::addTask(IThreadTask* newTask, bool waitForSlot)
{
	pthread_mutex_lock(&m_taskAccess);

	if (m_tasks->size() == m_tasks->capacity())
	{
		if (!waitForSlot)
		{
			pthread_mutex_unlock(&m_taskAccess);
			return (false);
		}
		else
			pthread_cond_wait(&m_fullQueue, &m_taskAccess);
	}

	m_tasks->push_back(newTask);
	pthread_cond_signal(&m_newTaskSignal);
	pthread_mutex_unlock(&m_taskAccess);

	return (true);
}

IThreadTask*	 TaskQueue::acquireTask()
{
	IThreadTask *toExecute;

	pthread_mutex_lock(&m_taskAccess);
	while (m_tasks->size() == 0)
		pthread_cond_wait(&m_newTaskSignal, &m_taskAccess);
	toExecute = m_tasks->front();
	m_tasks->pop_front();
	m_tasksExecuting += (toExecute != NULL);
	if (m_tasks->size() == m_tasks->capacity() - 1)
		pthread_cond_signal(&m_fullQueue);
	pthread_mutex_unlock(&m_taskAccess);
	return (toExecute);
}

void	TaskQueue::clear()
{
	pthread_mutex_lock(&m_taskAccess);
	m_tasks->clear();
	pthread_mutex_unlock(&m_taskAccess);
}

void	TaskQueue::waitForCompletion()
{   
	pthread_mutex_lock(&m_taskAccess);
	while (m_tasks->size() != 0 || m_tasksExecuting)
		pthread_cond_wait(&m_allTasksDone, &m_taskAccess);
	pthread_mutex_unlock(&m_taskAccess);
}

size_t	TaskQueue::getTaskCount()
{
	size_t result;
	
	pthread_mutex_lock(&m_taskAccess);
	result = m_tasks->size() + m_tasksExecuting;
	pthread_mutex_unlock(&m_taskAccess);
	return (result);
}
