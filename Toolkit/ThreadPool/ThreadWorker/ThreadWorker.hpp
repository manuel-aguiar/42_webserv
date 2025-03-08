

#ifndef THREADWORKER_HPP

# define THREADWORKER_HPP


// C headers
# include <pthread.h>

//forward declarations
class ThreadPoolImpl;

class ThreadWorker
{
    public:
        ThreadWorker(ThreadPoolImpl& pool);
        ~ThreadWorker();


        void		start();
        void		finish();

        pthread_t	getThreadID() const;

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

        ThreadPoolImpl&			m_pool;

        ThreadWorker(const ThreadWorker& copy);
        ThreadWorker& operator=(const ThreadWorker& assign);

};



#endif
