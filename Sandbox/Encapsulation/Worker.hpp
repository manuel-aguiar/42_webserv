
# ifndef WORKER_HPP

# define WORKER_HPP

class ImplBase;

class Worker
{
    public:
        Worker(ImplBase& impl);
        ~Worker();

        void WorkerMethod();
    
    private:
        ImplBase& m_impl;
};



#endif


