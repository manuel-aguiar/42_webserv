

#ifndef TASKQUEUE_HPP

# define TASKQUEUE_HPP

// C headers
# include <pthread.h>

//forward declarations
class ThreadPoolImpl;
class IThreadTask;

template <typename T>
class FixedCircularQueueImpl;

class TaskQueue
{
    public:
        TaskQueue();
        ~TaskQueue();

        void                init(FixedCircularQueueImpl<IThreadTask*>& tasks);

        bool				addTask(IThreadTask* newTask, bool waitForSlot = false);
        void				clear();
        void				waitForCompletion();		
        void				finishTask(IThreadTask* delTask);

        IThreadTask*		acquireTask();

        size_t			 	getTaskCount();

    private:
        FixedCircularQueueImpl<IThreadTask*>*        	m_tasks;
        unsigned int									m_tasksExecuting;
        pthread_mutex_t									m_taskAccess;
        pthread_cond_t									m_newTaskSignal;								   
        pthread_cond_t									m_allTasksDone;
        pthread_cond_t									m_fullQueue;									 

        TaskQueue(const TaskQueue& copy);
        TaskQueue& operator=(const TaskQueue& assign);
};

#endif
