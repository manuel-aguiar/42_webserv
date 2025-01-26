
#ifndef IMPLBASE_HPP

# define IMPLBASE_HPP

# include <vector>

class Worker;

class ImplBase
{
    public:
        void publicMethod();
        void workerCalls(Worker& worker);

    protected:
        ImplBase();
        ~ImplBase();

    private:
        std::vector<Worker> m_workers;
};


#endif